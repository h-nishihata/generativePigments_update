#ifndef _OF_VBO_
#define _OF_VBO_

#include "ofMain.h"

#define kNumImgs 5

extern int xValue;
extern int yValue;
extern int zValue;
extern bool overdose;


class vbo{
public:

    void setup();
    void update();
    void draw(int x, int y, int z);
    
    void setID(int indentify);
    void resetImg();
    void resetVerts();
    
    int ID;
    static int sum;
    int x,y;
    int timer;
    int lastingTime = 3000;

    unsigned char * pixels;
    
    static const int WIDTH = 512;
    static const int HEIGHT = 512;
    static const int NUM_PARTICLES = WIDTH * HEIGHT;
    
    float vecLength[NUM_PARTICLES];
    float tempMax;
    int longestVec;
    float initialLength;
    
    ofImage image;
    
    ofVbo myVbo;
    ofVec3f myVerts[NUM_PARTICLES];
    ofFloatColor myColor[NUM_PARTICLES];
    
    ofVec3f vec[NUM_PARTICLES];
    
};
#endif
