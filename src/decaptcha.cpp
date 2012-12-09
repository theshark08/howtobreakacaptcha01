/*
 *  decaptcha.cpp
 *    prints the captcha digits to stdout
 *
 *
 *    Ioan - Carol Plangu
 */


#include "features.hpp"
#include <list>
#include "data.inl"
#include <cmath>


int main(int argc, char** argv){

  if(argc == 1){
    cout << "usage: tcaptcha <captcha filename>\n";
    return 0;
  }

  img in(argv[1]);
// convert to optimal b/w image
  in = preprocess_captcha(in);


// extract column scores based on feature match frequency
  vector<vector<float> > column_score(in.Width(), vector<float>(10,0));
  for(uint16 x = 0; x < in.Width();  x++){
    for(uint16 y = 0; y < in.Height(); y++){
      if(in.p(x,y,0) == 0){
// for each black point
        for(uint8 d = 0; d <= 9; d++){
          float sc = 0;
// add the matched feature scores to the column score
          for(auto p = data[d].begin(); p != data[d].end() && p-data[d].begin() < 250 ; p ++)
            sc += feature::match(*p,in, x, y) * score[d][p-data[d].begin()];
          column_score[x][d] += sc;
        }
      }
    }
  }


// extract a smooth score based on the column scores
  int zone_width = 5;
  vector<vector<float> > zone_score(in.Width() - zone_width, vector<float>(10,0));
  for(int x = 0; x < zone_score.size(); x++)
    for(int l = 0; l < zone_width; l++)
      for(int d = 0; d <= 9; d++)
        zone_score[x][d] += column_score[x+l][d];

  vector<char> digits(zone_score.size(), -1);
  for(int x = 0; x < zone_score.size(); x++){
    float max = 0; int pos = -1;
    for(int d = 0; d <= 9; d++){
      if( zone_score[x][d] > max ){
        max = zone_score[x][d];
        pos = d;
      }
    }
    digits[x] = pos;
  }


// normalize first score
  for(int x = 1; digits[0] == -1 && x < digits.size(); x++)
    if( digits[x] != -1 ) digits[0] = digits[x];

// fill smaller partitions
  int ct = 1;
  for(int x = 1; x < digits.size(); x++){
    if( digits[x] != digits[x-1] ){
      if(ct < 5){
        for(int l = x - ct; l < x; l ++)
          digits[l] = -1; //x - ct > 0 ? digits[x - ct - 1] : digits[x];
      }
      ct = 0;
    }
    ct ++;
  }

// extract captcha text (basic DFA acceptor)
  ct = 0;
  vector<uint16> out;
  for( auto p = digits.begin(); p != digits.end(); p++){
    if( *p == -1 ) continue;
    if( *p != *(p-1) ) ct = 0;
    ct ++;
    switch(ct)
      case 5: case 17: case 30:
        out.push_back( *p + ct * 256);
    if( ct >= 5 ) out.back() = out.back()%256 + (out.back()/256 + 1) * 256;
  }


  if( out.size() > 6 )
  for(int l = 0; l < out.size() - 6; l ++){
    int min = 100, posmin = 5;
    for( auto p = out.begin(); p != out.end(); p++)
      if( *p/256 < min ){ min = *p/256; posmin = p - out.begin(); }
    out[posmin] = 0;
  }


  if(out.size() >= 6){
    for( auto p = out.begin(); p != out.end(); p++ )
      if(*p) cout << *p % 256;
  }
  else
    cout << "error";

  return 0;
}
