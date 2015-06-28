#include "ofApp.h"

int xAxis;
int yAxis;
int zAxis;
int xRange;
int yRange;
int zRange;
bool xFlag, yFlag, zFlag;
int timer;
bool overdose;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofEnableDepthTest();
    
    buffer.allocate(1440, 900, GL_RGBA);
    buffer.begin();
    ofClear(255, 255, 255, 0);
    buffer.end();
    
    
    // nodes
    testNodes[0].setPosition(ofVec3f(0,0,0));
    testNodes[1].setPosition(ofVec3f(1000,0,1200));
    testNodes[1].setParent(testNodes[0]);
    
    
    // cameras
    camToView = 0;
    cam[0].setPosition(300, -40, -800);
    cam[1].setPosition(-100, 200, -500);
//    lookatIndex[1] = kNumTestNodes-1;
    for (int t=0; t<kNumCameras; t++) {
        cam[t].clearParent();
    }
    
    
    //  lights
    light[0].setPosition(0, 50, 0);
    light[1].setPosition(1000, 50, 1200);
    
    for (int s=0; s<kNumLights; s++) {
        light[s].enable();
        light[s].setSpotlight();
        light[s].lookAt(testNodes[lookatIndex[s]]);
        
        light[s].setAmbientColor(ofColor(r, g, b, 255));
        light[s].setDiffuseColor(ofColor(r+13, g+24, b+12));
        light[s].setSpecularColor(ofColor(255, 255, 255));
    }
    
    
    //  objects
    for (int s=0; s<num; s++) {
        v[s].setID(s);
        v[s].setup();
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofEnableAlphaBlending();
    float freqMult = 1;
    float amp = 15;
    
    for(int i=0; i<kNumTestNodes; i++) {
        testNodes[i].setOrientation(ofVec3f(sin(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5,
                                            cos(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5,
                                            sin(ofGetElapsedTimef() * freqMult * 0.2 * 0.7) * amp * 5));
        freqMult *= 3;
        amp *= 0.8;
    }
    
    
    //  cameras
    
    if (xFlag == false) {
        if (cam[0].getGlobalPosition().x < -700 || cam[1].getGlobalPosition().x < -700) {
            xFlag = true;
            overdose = true;
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }else if (xFlag == true) {
        if (cam[0].getGlobalPosition().x > 700 || cam[1].getGlobalPosition().x > 700) {
            xFlag = false;
            overdose = true;
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }
    
    
    if (yFlag == false) {
        if (cam[0].getGlobalPosition().y < -200 || cam[1].getGlobalPosition().y < -200) {
            yFlag = true;
            overdose = true;
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }else if (yFlag == true) {
        if (cam[0].getGlobalPosition().y > 200 || cam[1].getGlobalPosition().y > 200) {
            yFlag = false;
            overdose = true;
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }
    
    
    if (zFlag == false) {
        if (cam[0].getGlobalPosition().z < -700 || cam[1].getGlobalPosition().z < -700) {
            zFlag = true;
            overdose = true;
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }else if (zFlag == true) {
        if (cam[0].getGlobalPosition().z > 700 || cam[1].getGlobalPosition().z > 700) {
            zFlag = false;
            overdose = true;            
            switch (camToView) {
                case 0:
                    camToView = 1;
                    break;
                case 1:
                    camToView = 0;
                    break;
            }
        }
    }
    
    
    if (camToView == 0) {
        if (((int)cam[0].getGlobalPosition().x > -110 && (int)cam[0].getGlobalPosition().x < -90) ||
            ((int)cam[0].getGlobalPosition().x > 90  && (int)cam[0].getGlobalPosition().x < 110)){
            clearBuffer = true;
            if (clearBuffer) {
                bufferClearTime = 50;
            }
        }
        
    }else if (camToView == 1) {
        if (((int)cam[1].getGlobalPosition().x > -110 && (int)cam[1].getGlobalPosition().x < -90) ||
            ((int)cam[1].getGlobalPosition().x > 90  && (int)cam[1].getGlobalPosition().x < 110)){
            clearBuffer = true;
            if (clearBuffer) {
                bufferClearTime = 30;
            }
        }
    }
    

    for (int t=0; t<kNumCameras; t++) {
        ofVec3f oldP = cam[t].getGlobalPosition();
        ofQuaternion oldQ = cam[t].getGlobalOrientation();
        cam[t].setParent(testNodes[lookatIndex[t]]);
    }

    
    // lighting
    if (rFlag == false) {
        r -=0.1;
        if (r <= 10) {
            rFlag = true;
        }
    }else if (rFlag == true) {
        if (g > 80 && b < 80) {
            r +=0.1;
        }
        if (r >= 255) {
            rFlag = false;
        }
    }
    
    if (gFlag == false) {
        g -=0.1;
        if (g <= 10) {
            gFlag = true;
        }
    }else if (gFlag == true) {
        g +=0.1;
        if (g >= 255) {
            gFlag = false;
        }
    }
    
    if (bFlag == false) {
        b -=0.1;
        if (b <= 10) {
            bFlag = true;
        }
    }else if (bFlag == true) {
        if (r < 80 && g > 80) {
            b +=0.1;
        }
        if (b >= 190) {
            bFlag = false;
        }
    }
    
    for (int s=0; s<kNumLights; s++) {
        light[s].setAmbientColor(ofColor(r, g, b, 255));
        light[s].setDiffuseColor(ofColor(r+13, g+24, b+12));
        light[s].setSpecularColor(ofColor(255, 255, 255));
    }
    
    
    //  objects
    for (int s=0; s<num; s++) {
        v[s].update();
    }
    
    
    // particles
    if(points.size() < 500000) {
        points.push_back(ofVec3f(ofRandom(-2000, 2000),ofRandom(-10, 200),ofRandom(-2000, 2000)));
        speeds.push_back(ofVec3f(0,0,0));
    }

    
    for (unsigned int i=0; i<points.size(); i++) {
        speeds[i].y += yAxis*0.1;
        points[i]   += speeds[i];
        speeds[i]   *= 0.98;
        
                ofVec3f vec = v[0].myVerts[0] - points[i];
                if(vec.length() < 100) {
                    vec.normalize();
                    speeds[i] -= vec;
                }
        
        if(points[i].x > xRange*10)    points[i].x = xRange*-10;
        if(points[i].x < xRange*-10)   points[i].x = xRange*10;
        if(points[i].y > yRange)       points[i].y = yRange*-0.1;
        if(points[i].y < yRange*-0.1)  points[i].y = yRange;
        if(points[i].z > zRange*10)    points[i].z = zRange*-10;
        if(points[i].z < zRange*-10)   points[i].z = zRange*10;
    }
    
    
    buffer.begin();
    drawFboTest();
    buffer.end();
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest(){
    
    ofEnableAlphaBlending();
    
    if (clearBuffer == true && bufferClearTime > 0) {
        bufferClearTime--;
        if (bufferClearTime <= 0){
            clearBuffer = false;
        }
    }
    
    if (ofGetKeyPressed('c') || clearBuffer == true){
        ofClear(255,255,255,0);
    }
    
    
    for (int i=0; i<kNumCameras; i++) {
//        if (lookatIndex[i] >= 0) {
        cam[i].lookAt(testNodes[lookatIndex[i]]);
//        }
    }
    
    
    cam[camToView].begin();
    
    ofRotateX(180);
    
    // objects
    v[0].draw(0, 0, 0);
    v[1].draw(1000, 0, 1200);
    
    
    // particles
    ofSetColor(255);
    particles.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
    particles.draw(GL_POINTS, 0, (int)points.size());
    
    cam[camToView].end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofHideCursor();
    
    ofSetColor(255);
    buffer.draw(0,0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '0') {
        camToView = 0;
    }else if (key == '1'){
        camToView = 1;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
