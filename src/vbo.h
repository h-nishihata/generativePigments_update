#ifndef _OF_VBO_
#define _OF_VBO_

#include "ofMain.h"

extern int xAxis;
extern int yAxis;
extern int zAxis;
extern int xRange;
extern int yRange;
extern int zRange;
extern int timer;
extern bool overdose;

class vbo{
public:

    void setup();
    void update();
    void draw(float x, float y, float z);
    
    void setID(int indentify);
    void reset();
    
    int ID;
    static int sum;
    int x,y;
    
    static const int WIDTH = 512;
    static const int HEIGHT = 512;
    static const int NUM_PARTICLES = WIDTH * HEIGHT;

    unsigned char * pixels;
    
        float vecLength[NUM_PARTICLES];
    
    ofImage image;
    
    ofVbo myVbo;
    ofVec3f myVerts[NUM_PARTICLES];
    ofFloatColor myColor[NUM_PARTICLES];
    
    ofVec3f vec[NUM_PARTICLES];


    
};
#endif
