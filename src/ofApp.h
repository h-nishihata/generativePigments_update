#pragma once

#include "ofMain.h"
#include "vbo.h"

#define num 2
#define kNumTestNodes 2
#define kNumCameras 2
#define kNumLights 2

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void drawFboTest();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    int camToView;
    int lookatIndex[kNumCameras];
    
    float posX_00 =  300;
    float posY_00 =  -40;
    float posZ_00 =  -800;
    float posX_01 =  -100;
    float posY_01 =  200;
    float posZ_01 =  -500;
    
    float r = 218;
    float g = 165;
    float b = 32;
    bool rFlag, gFlag, bFlag;
    
    int count;
    bool clearBuffer;
    int bufferClearTime;
    
    ofFbo buffer;
    
    ofNode testNodes[kNumTestNodes];
    ofCamera cam[kNumCameras];
    ofLight light[kNumLights];
    
    vbo v[num];
    
    ofVbo particles;
    vector<ofVec3f>points;
    vector<ofVec3f>speeds;
    ofVec3f agent;
    
};
