/*
 *  features.cpp
 *    preprocess_captcha(...
 *       converts the captcha to an optimum size black/white image
 *    class feature
 *       describes a random set of black points in local coordinates in relationship to the first point
 *    extract_random_feature(...
 *       extracts a compatible feature to a given image
 *
 *
 *   Ioan - Carol Plangu
 */


#include "image.hpp"
#include "math.h"
#include "stdlib.h"
#include <algorithm>


img preprocess_captcha(img &i){
  return i.crop(10,0,85,30).filter( [] ( img &i, uint16 x, uint16 y, uint8 col, void* param) -> uint8 {
      switch(col){
        case 0: case 1: case 2:
          return i.p(x,y,0) < 10 ? 0 : 255;
        case 3:
          return 255;
      }
  });
}

class feature : public vector<char>{
public:
  feature(const vector<char>& c) : vector<char>(c) {}
  feature(vector<uint16> raw){
    sort(raw.begin(), raw.end());
    for(uint8 i = 1; i < raw.size(); i++){
      push_back(char(raw[i] % 256) - (raw[0] % 256));
      push_back(char(raw[i] / 256) - (raw[0] / 256));
    }
  }
  bool match(img &i, uint8 x_offset=0, uint8 y_offset=0){
    for(auto p = begin(); p != end(); p+=2)
      if(i.p(p[0] + x_offset, p[1] + y_offset, 0) != 0)
        return false;
    return true;
  }
  static bool match(vector<char> &v, img &i, uint8 x_offset=0, uint8 y_offset=0){
    for(auto p = v.begin(); p != v.end(); p+=2)
      if(i.p(p[0] + x_offset, p[1] + y_offset, 0) != 0)
        return false;
    return true;
  }
};

ostream& operator << (ostream& o, const feature& f){
  for(auto p = f.begin(); p != f.end(); p+=2)
    o << "( " << (int)p[0] << ", " << (int)p[1] << " ) ";
  return o;
}

feature extract_random_feature(img& img, uint8 min_feature_size, uint8 max_feature_size = 0){
  vector<uint16> coords, ret;

  for(uint8 x = 0; x < img.Width(); x++)
    for(uint8 y = 0; y < img.Height(); y++)
      if(img.p(x,y,0) == 0)
        coords.push_back(x + y * 256);

  if(max_feature_size == 0)
    max_feature_size = min_feature_size;
  uint8 size = min_feature_size + ( (max_feature_size - min_feature_size) ? rand() % (max_feature_size - min_feature_size) : 0 );

  while(ret.size() < size){
    uint8 pos = rand() % coords.size();
    if(coords[pos] != 0){
      ret.push_back(coords[pos]);
      coords[pos] = 0;
    }
  }

  return feature(ret);
}
