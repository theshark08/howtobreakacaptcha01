/*
 *  train_model.cpp
 *    extracts an optimum set of feature for each digits based on the scoreing
 *    system described at feature_digit_weight() and feature_score()
 *
 *    the result is printed to stdout in c++11 inline format
 *
 *  Ioan - Carol Plangu
 */


#include "features.hpp"
#include "slist.hpp"

#define MIN_FEATURE_SIZE      5
#define MAX_FEATURE_SIZE      9

#define FEATURE_COUNT         1000

#define FIELD_SIZE            100000


float feature_digit_weight(vector<img>& digit_training_images, feature &f){
//  sum of matched images over total number of images
  float ret = 0;
  for(auto p = digit_training_images.begin(); p != digit_training_images.end(); p++)
    for(uint8 x = 0; x < p->Width(); x++)
      for(uint8 y = 0; y < p->Height(); y++)
        if(p->p(x,y,0) == 0 && f.match(*p, x, y)){
          ret ++;
          break;
        }
  return ret / digit_training_images.size();
}

float feature_score(uint8 digit, feature& f, vector<vector<img> > &training_data){
// score of selected digit over 1 + sum of other digit scores
  float ret = 1;

  for(uint8 d = 0; d <= 9; d++)
    if( d != digit )
      ret += feature_digit_weight(training_data[d], f);

  return feature_digit_weight(training_data[digit], f) / ret;
}

class feature_spec{
protected:
  feature f;
  float score;
public:
  feature_spec(uint8 digit, feature& _f, vector<vector<img> > &training_data) :
    f(_f), score(feature_score(digit, _f, training_data)) {}
  feature Feature() const { return f; }
  float Score() const { return score; }
};

int main(int argc, char** argv){

  vector<vector<img> > sample(10);

  while( cin.good() ){
    string filename;
    string captcha;
    vector<uint16> slice(12);

    cin >> filename;
    cin >> captcha;
//    for(uint8 l = 0; l < 12; l++)
//      cin >> slice[l];
    if(!cin.good()) break;

    img i(filename.c_str());
    i = preprocess_captcha(i);
    sample[captcha[0] - '0'].push_back(i);

/*    for(uint8 x=0;x<6;x++){
      sample[captcha[x] - '0'].push_back(i.crop(
           slice[x*2], slice[x*2+1]-23,
           slice[x*2] + 20, slice[x*2+1]+3
      ));
    }*/
  }

  uint32 sample_size = 0;
  for(uint8 d = 0; d <= 9; d ++) sample_size += sample[d].size();
  cerr << "training base of " << sample_size << " digits created\n";


  vector<slist<feature_spec> > out(10);

  cerr << "populating field ";
  float cursor = 0;
  for(uint32 l = 0; l <= FIELD_SIZE; l++){
    while(cursor < l){
      cursor += .05f * FIELD_SIZE;
      cerr << "."; cerr.flush();
    }
    uint8 d = rand()%10;
    feature f =  extract_random_feature(sample[d][rand()%sample[d].size()], MIN_FEATURE_SIZE, MAX_FEATURE_SIZE);
    for(uint8 d = 0; d <= 9; d ++){
      auto s = feature_spec(d, f, sample);
      out[d].push_sorted(s, [](feature_spec s1, feature_spec s2) -> bool { return s1.Score() > s2.Score(); });
      if(out[d].size() > FEATURE_COUNT) out[d].pop_back();
    }
  }
  cerr << " done\n";

  cout << "// this is a file generated by train_model.py\n\n";
  cout << "vector<vector<vector<char> > > data = {\n";
  for(uint8 d = 0; d <= 9; d++){
    cout << "{" << "// " << (int) d << "\n";
    for(auto p = out[d].begin(); p != out[d].end(); p++){
      cout << "/* " << p->Score() << " */ { ";
      for(auto it = p->Feature().begin(); it != p->Feature().end(); it++)
        cout << (int)*it << (it+1 == p->Feature().end() ? "" : ", ");
      auto t = p; t++;
      cout << " }" << (t == out[d].end() ? "" : ", ") << "\n";
    }
    cout << "}" << (d==9 ? "": ",") << "\n";
  }
  cout << "};\n";
  cout << "vector<vector<float> > score = {\n";
  for(uint8 d = 0; d <= 9; d++){
    cout << "// " << (int) d << "\n" << "{ ";
    for(auto p = out[d].begin(); p != out[d].end(); p++){
      auto t = p; t++;
      cout << p->Score() << (t == out[d].end() ? " " : ", ");
    }
    cout << "}" << (d==9 ? "": ",") << "\n";
  }
  cout << "};\n";
  return 0;
}
