#pragma once

#include "ofMain.h"
#include "vbo.h"

#define kNumObjs 2
#define kNumNodes 2
#define kNumControllers 1
#define kNumCameras 2
#define kNumLights 2


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void drawFboTest();
    void switchLookat();
    
    void keyPressed (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    
    ofNode testNodes[kNumNodes];
    ofNode testControllers[kNumControllers];
    ofLight light[kNumLights];
    ofCamera cam;
    
    
    ofFbo buffer;
    vbo v[kNumObjs];
    
    
    ofVbo particles;
    vector<ofVec3f>points;
    
    
    float camPosX, camPosY, camPosZ;
    int objPosX_0, objPosY_0, objPosZ_0;
    int objPosX_1, objPosY_1, objPosZ_1;
    int lookatIndex;
    int parentIndex;
//    int camToView;    // For multiple cameras
    
    bool waiting;
    int step;
    
    float r = 218;
    float g = 165;
    float b = 32;
    bool rFlag, gFlag, bFlag;
    
    
    bool clearBuffer;
    int bufferClearTime;
        
};
