/*
 *  image.hpp
 *    this is a basic raw img class that uses imagemagick's convert
 *
 *    Ioan - Carol Plangu
 */



#include <iostream>
#include <vector>
#include <fstream>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"




using namespace std;


typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

class img : public vector<unsigned char>{
  unsigned int width;
  unsigned int height;

  public:
    img(int _width, int _height) : vector<unsigned char>(_width * _height * 4 + 1, 0), width(_width), height(_height){}
    img(string filename){
      static char geometry[] = "Geometry";
      FILE *f = popen(("identify -verbose "+filename).c_str(),"r");
      char line[1024];
      string header = "";
      while(fgets(line,sizeof(line),f)) header += string(line);
      pclose(f);
      header += '\0';
      int state = 0;
      width = 0;
      height = 0;
      for(char *p = &header[0]; *p != 0 && state < 12; p++){
        if( *p == '\n' ) state = 0;
        switch(state){
          default: state += *p == geometry[state]; break;
          case 8:  if(*p >= '0' && *p <= '9'){ p--; state++; } break;
          case 9:  if(*p >= '0' && *p <= '9'){ width = width*10 + *p - '0'; } else state ++; break;
          case 10: if(*p >= '0' && *p <= '9'){ p--; state++; } break;
          case 11: if(*p >= '0' && *p <= '9'){ height = height*10 + *p - '0'; } else state ++; break;
        }
      }

      resize(width * height * 4 + 1, 0);

      string fn = filename, tmp = fn; size_t pos = 0;

      while((pos = tmp.find("/")) != string::npos) tmp = tmp.substr(pos+1);
//      tmp = "/dev/shm/" + tmp;

      string cmd = "convert " + fn + " " + tmp + ".rgba";
      system(cmd.c_str());

      ifstream in;
      in.open((tmp+".rgba").c_str());
      if(in.is_open()){
        in.read((char*)&(*this)[0], width * height * 4);
        in.close();
      }
      system(("rm "+tmp+".rgba").c_str());
    }
    uint16 Width(){ return width; }
    uint16 Height(){ return height; }

    unsigned char& p(int x, int y, int c){
      if(x < 0 || x >= width || y < 0 || y >= height || c < 0 || c >= 4){
        (*this)[width * height * 4] = 255;
        return (*this)[width * height * 4];
      }
      return (*this)[ y * width * 4 + x * 4 + c ];
    }
    void save(string filename){
      ofstream out; out.open((filename+".rgba").c_str());
      if(! out.is_open()) return;
      out.write((char*)&(*this)[0], width * height * 4);
      out.close();
      char size[256]; sprintf(size, "%dx%d", width,height);
      string cmd = "convert -size " + string(size) + " -depth 8 rgba:" + filename + ".rgba " + filename;
      system(cmd.c_str());
      cmd = "rm "+filename+".rgba";
      system(cmd.c_str());
    }
    img crop(int x1,int y1, int x2, int y2){
       img ret(x2-x1, y2-y1);
       for(int x = 0; x < ret.Width(); x++)
        for(int y = 0; y < ret.Height(); y++){
           ret.p(x,y,0) = p(x+x1,y+y1,0);
           ret.p(x,y,1) = p(x+x1,y+y1,1);
           ret.p(x,y,2) = p(x+x1,y+y1,2);
           ret.p(x,y,3) = p(x+x1,y+y1,3);
       }
       return ret;
    }



    uint32 apply( uint32 (*f)(img&, uint16, uint16) ){
       uint32 ret = 0;
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           ret += (*f)(*this,x,y);
       return ret;
    }
    uint32 apply( uint32 (*f)(img&, uint16, uint16, void*), void* param = 0 ){
       uint32 ret = 0;
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           ret += (*f)(*this,x,y,param);
       return ret;
    }
    uint32 apply( uint32 (*f)(img&, uint16, uint16, uint8)){
       uint32 ret = 0;
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           for(uint8 c = 0; c < 4; c++)
             ret += (*f)(*this,x,y,c);
       return ret;
    }
    uint32 apply( uint32 (*f)(img&, uint16, uint16, uint8, void*), void* param = 0 ){
       uint32 ret = 0;
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           for(uint8 c = 0; c < 4; c++)
             ret += (*f)(*this,x,y,c,param);
       return ret;
    }


    img filter( uint8 (*f) (img&, uint16, uint16, uint8)){
       img ret(Width(),Height());
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           for(uint8 c = 0; c < 4; c++)
             ret.p(x,y,c) = (*f)(*this,x,y,c);
       return ret;
    }
    img filter( uint8 (*f) (img&, uint16, uint16, uint8, void*), void* param = 0 ){
       img ret(Width(),Height());
       for(uint16 x = 0; x < Width(); x++)
         for(uint16 y = 0; y < Height(); y++)
           for(uint8 c = 0; c < 4; c++)
             ret.p(x,y,c) = (*f)(*this,x,y,c,param);
       return ret;
    }
};


