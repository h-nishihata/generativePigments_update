#include "ofApp.h"

int xValue, yValue, zValue;
bool xFlag, yFlag, zFlag;
bool overdose;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
	ofEnableDepthTest();
	ofEnableLighting();
	
    
    buffer.allocate(1440, 900, GL_RGBA);
    buffer.begin();
        ofClear(255, 255, 255, 0);
    buffer.end();
    
    
    // lights
    light[0].setPosition(0,0,0);
    light[1].setPosition(1000,0,1200);
    
	for(int i = 0; i < kNumLights; i++) {
		light[i].enable();
        light[i].setSpotlight();
        light[i].lookAt(testNodes[lookatIndex]);
	}
	
    
    // cameras
    cam.resetTransform();
    cam.clearParent();

    lookatIndex = 1;
    parentIndex = 0;
//    camToView = 0;
    
    
    camPosX = 250;
    camPosY = 0;
    camPosZ = 350;
    cam.setPosition(camPosX, camPosY, camPosZ);
    cam.setParent(testControllers[parentIndex]);

	
    
    // nodes
    objPosX_0 = objPosY_0 = objPosZ_0 = 0;
    objPosX_1 = 500;
    objPosY_1 = 0;
    objPosZ_1 = 700;
    
    testNodes[0].setPosition(objPosX_0, objPosY_0, objPosZ_0);
    testNodes[1].setPosition(objPosX_1, objPosY_1, objPosZ_1);

	for(int i=0; i<kNumControllers; i++) {
		if(i>0) testControllers[i].setParent(testControllers[i-1]);
	}
    
    
    //  objects
    for (int i=0; i<kNumObjs; i++) {
        v[i].setID(i);
        v[i].setup();
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    

    for(int i=0; i<kNumControllers; i++) {
		testControllers[i].setPosition(ofVec3f(sin(ofGetElapsedTimef()*0.1),
                                               cos(ofGetElapsedTimef()*0.1),
                                               sin(ofGetElapsedTimef()*0.1)));
        
		testControllers[i].setOrientation(ofVec3f(sin(ofGetElapsedTimef()*0.2)*xValue/2,
                                                  cos(ofGetElapsedTimef()*0.2)*yValue/2,
                                                  sin(ofGetElapsedTimef()*0.4)*zValue/2));
	}
    
    
    //  cameras
    if(lookatIndex >= 0){
        cam.lookAt(testNodes[lookatIndex]);
    }

    
    if (zFlag == false) {
        camPosZ -= zValue/100;
        if (cam.getGlobalPosition().z > objPosZ_1-100) {
            zFlag = true;
        }
    }else if (zFlag == true) {
        camPosZ += zValue/100;
        if (cam.getGlobalPosition().z < objPosZ_0+100) {
            zFlag = false;
        }
    }
    
    cam.setPosition(camPosX, camPosY, camPosZ);
    

    if (testControllers[0].getRoll() > -10 && testControllers[0].getRoll() < 10){
        clearBuffer = true;
        bufferClearTime = abs(testControllers[0].getRoll() * 6);
    }
    
    if(testControllers[0].getPitch() >= 90 && testControllers[0].getPitch() <= 100){
        overdose = false;
    }
    
    if((testControllers[0].getHeading() >= 0 && testControllers[0].getHeading() <= (int)abs(5))  ||
       (testControllers[0].getHeading() >= (int)abs(15) && testControllers[0].getHeading() <= (int)abs(20)) ||
       (testControllers[0].getHeading() >= (int)abs(95) && testControllers[0].getHeading() <= (int)abs(100))){
        if(!waiting){
            switchLookat();
            waiting = true;
        }
    }
    
    if(waiting){
        if(step < 100){
            step++;
        }else{
            waiting = false;
            step = 0;
        }
    }
    
    
    // lighting
    if (rFlag == false) {
        r -=0.1;
        if (r <= 10) {
            rFlag = true;
        }
    }else if (rFlag == true) {
        if (b < 80) {
            r +=0.1;
        }
        if (r >= 180) {
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
        if (r < 80 && g < 80) {
            b +=0.1;
        }
        if (b >= 220) {
            bFlag = false;
        }
    }
    
    for (int i=0; i<kNumLights; i++) {
        light[i].setAmbientColor(ofColor(r, g, b, 255));
        light[i].setDiffuseColor(ofColor(r+13, g+24, b+12));
        light[i].setSpecularColor(ofColor(255, 255, 255));
    }
    
    
    //  objects
    for (int i=0; i<kNumObjs; i++) {
        v[i].update();
    }
    
    
    // particles
    if(points.size() < 50000) {
        points.push_back(ofVec3f(ofRandom(-5000, 5000),ofRandom(-2000, 2000),ofRandom(-5000, 5000)));
    }
    
    
    for (unsigned int i=0; i<points.size(); i++) {

        float t = ofGetElapsedTimef() * 0.9f;
        ofVec3f pos(ofSignedNoise(t, points[i].y, points[i].z),
                    ofSignedNoise(points[i].x, t, points[i].z),
                    ofSignedNoise(points[i].x, points[i].y, t));
        
        pos *= 10 * ofGetLastFrameTime();
        points[i]   += pos;

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
    
    
    cam.begin();
    
        /*
        ofSetColor(255, 0, 0);
        testNodes[0].setScale(5);
        testNodes[0].draw();
        
        ofSetColor(0, 0, 255);
        testNodes[1].setScale(5);
        testNodes[1].draw();
        */
    
    
        // objects
        v[0].draw(objPosX_0, objPosY_0, objPosZ_0);
        v[1].draw(objPosX_1, objPosY_1, objPosZ_1);
    
        
        // particles
        ofSetColor(255);
        particles.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
        particles.draw(GL_POINTS, 0, (int)points.size());
    
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofHideCursor();
    
    ofSetColor(255);
    buffer.draw(0,0);
    
}

//--------------------------------------------------------------
void ofApp::switchLookat(){
    
    lookatIndex++ ;
    if(lookatIndex >= kNumNodes) {
        lookatIndex = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch(key) {
            
		case 't':
            switchLookat();
			
		case 'p':
			parentIndex++ ;
			if(parentIndex >= kNumControllers) {
				parentIndex = 0;
            }
            cam.setParent(testControllers[parentIndex]);

			break;
			
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
