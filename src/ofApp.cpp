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
	ofEnableLighting();
	
    
    buffer.allocate(1440, 900, GL_RGBA);
    buffer.begin();
        ofClear(255, 255, 255, 0);
    buffer.end();
    
    
    // lights
    light[0].setPosition(0, 50, 0);
    light[1].setPosition(1000, 50, 1200);
    
	for(int i = 0; i < kNumLights; i++) {
		light[i].enable();
        light[i].setSpotlight();
        light[i].lookAt(testNodes[lookatIndex]);
        
        light[i].setAmbientColor(ofColor(r, g, b, 255));
        light[i].setDiffuseColor(ofColor(r+13, g+24, b+12));
        light[i].setSpecularColor(ofColor(255, 255, 255));
	}
	
    
    // cameras
    cam.resetTransform();
    cam.setFov(60);
    cam.clearParent();

    lookatIndex = 1;
    parentIndex = 0;
    camToView = 0;
    
    cam.setPosition(300, 50, -800);
    cam.setParent(testControllers[parentIndex]);

	
    
    // nodes
    testNodes[0].setPosition(0, 0, 0);
    testNodes[1].setPosition(1000, 0, 1200);

	for(int i=0; i<2; i++) {
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
    
    
	float freqMult = 1;
	float amp = 30;
	float scale = 1;
	

    for(int i=0; i<2; i++) {
		testControllers[i].setPosition(ofVec3f(sin(ofGetElapsedTimef() * freqMult) * amp, cos(ofGetElapsedTimef() * freqMult) * amp, sin(ofGetElapsedTimef() * freqMult * 0.7) * amp));
		
		testControllers[i].setOrientation(ofVec3f(sin(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, cos(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, sin(ofGetElapsedTimef() * freqMult * 0.2 * 0.7) * amp * 5));
		
		freqMult *= 3;
		amp *= 0.8;
		scale *= 0.8;
	}
    
    
    if(lookatIndex >= 0){
        cam.lookAt(testNodes[lookatIndex]);
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
    
    
    
    cam.begin();
    
        /* debug
        ofSetColor(255, 0, 0);
        testNodes[0].setScale(5);
        testNodes[0].draw();
        
        ofSetColor(0, 0, 255);
        testNodes[1].setScale(5);
        testNodes[1].draw();
        */
    
    
        // objects
        v[0].draw(0, 0, 0);
        v[1].draw(1000, 0, 1200);
    
        
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
void ofApp::keyPressed(int key){
	
	switch(key) {
			
		case 'v':
			camToView = 1 - camToView;
			break;
			
		case 't':
			lookatIndex++ ;
			if(lookatIndex >= kNumNodes) {
				lookatIndex = 0;
			}
			break;
			
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
