#include "vbo.h"

const int vbo::WIDTH;
const int vbo::HEIGHT;
const int vbo::NUM_PARTICLES;
int vbo::sum;
int x,y;

//--------------------------------------------------------------
void vbo::setID(int identify){
    
    ID = identify;
    sum++;  //  sum is ２ when the program starts
    
}

//--------------------------------------------------------------
void vbo::reset(){

//    image.clear();
    sum++;
    x = y = 0;
    char str[20];
    if (sum < 5) {
        sprintf(str, "test_%02d.jpg", sum);
        image.loadImage(str);
    }else{
        sum = 2;
        sprintf(str, "test_%02d.jpg", ID);
        image.loadImage(str);
    }
    
//    myVbo.clearVertices();
//    myVbo.clearColors();
    
    
    int i,j;
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    
    for (i=0; i<WIDTH; i++) {
        for (j=0; j<HEIGHT; j++) {
                myVerts[j * WIDTH + i].set(i - WIDTH/2, j - HEIGHT/2, 10000);
                myColor[j * WIDTH + i].set(0.0, 0.0, 0.0);
        }
    }
    
    myVbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    myVbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    
    
    pixels = image.getPixels();
    for (i=0; i<WIDTH; i++) {
        for (j=0; j<HEIGHT; j++) {
            
            float r = (float)pixels[j * WIDTH*3 + i * 3] / 256.0;
            float invertR = r+(2*(0.5-r));
            
            float g = (float)pixels[j * WIDTH*3 + i * 3+1] / 256.0;
            float invertG = g+(2*(0.5-g));
            
            float b = (float)pixels[j * WIDTH*3 + i * 3+2] / 256.0;
            float invertB = b+(2*(0.5-b));
            
            float brightness = (invertR+invertG+invertB) / 3.0f;
            if (pixels[j * WIDTH*3 + i * 3] < 170 || pixels[j * WIDTH*3 + i * 3+2] > 100) {
                myVerts[j * WIDTH + i] = ofVec3f(i - WIDTH/2, j - HEIGHT/2, brightness * 256.0);
                myColor[j * WIDTH + i] = ofFloatColor(invertR,invertG,invertB, 1.0);
            }
        }
    }
    
    myVbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    myVbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    
}

//--------------------------------------------------------------
void vbo::setup(){

    x = y = 0;
    char str[20];
    sprintf(str, "test_%02d.jpg", ID);
    image.loadImage(str);
    
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    pixels = image.getPixels();
    int i,j;
    
    for (i=0; i<WIDTH; i++) {
        for (j=0; j<HEIGHT; j++) {
            
            float r = (float)pixels[j * WIDTH*3 + i * 3] / 256.0;
            float invertR = r+(2*(0.5-r));
            
            float g = (float)pixels[j * WIDTH*3 + i * 3+1] / 256.0;
            float invertG = g+(2*(0.5-g));
            
            float b = (float)pixels[j * WIDTH*3 + i * 3+2] / 256.0;
            float invertB = b+(2*(0.5-b));
            
            float brightness = (invertR+invertG+invertB) / 3.0f;
            if (pixels[j * WIDTH*3 + i * 3] != 255 &&
                pixels[j * WIDTH*3 + i * 3+1] != 255 &&
                pixels[j * WIDTH*3 + i * 3+2] != 255) {
                myVerts[j * WIDTH + i] = ofVec3f(i - WIDTH/2, j - HEIGHT/2, brightness * 256.0);
                myColor[j * WIDTH + i] = ofFloatColor(invertR,invertG,invertB, 1.0);
                
                vec[j*WIDTH+i] = myVerts[j * WIDTH + i] - myVerts[HEIGHT/2 * WIDTH + WIDTH/2];
                //　速度ベクトル　＝　位置ベクトルB（末端）　ー　位置ベクトルA（中心）
                vecLength[j*WIDTH+i] = vec[j*WIDTH+i].length();
            }
            

        }
    }
    
    myVbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    myVbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);

}

//--------------------------------------------------------------
void vbo::update(){
    
    if (timer % 80 == 0) {
        if (y < HEIGHT) {
            if(x < WIDTH) {
                xRange = pixels[y*WIDTH*3+x*3];
                yRange = pixels[y*WIDTH*3+x*3+1];
                zRange = pixels[y*WIDTH*3+x*3+2];
                
                xAxis = xRange/100;
                yAxis = yRange/100;
                zAxis = zRange/100;
                x++;
            }else{
                x = 0;
                y++;
            }
        }else{
            y = 0;
        }
    }
    /*
    if (timer < 3000) {
        timer++;
    }else{
        timer = 0;
        reset();
    }


    if (!overdose) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            

            
//            myVerts[j * WIDTH + i].x += (vec[j*WIDTH+i].x*vec[j*WIDTH+i].length()*0.0001);
            myVerts[j * WIDTH + i].y += (vec[j*WIDTH+i].y * vecLength[j*WIDTH+i] * 0.0001);
            myVerts[j * WIDTH + i].z += (vec[j*WIDTH+i].z * vecLength[j*WIDTH+i] * 0.0001);
            //            myVerts[j * WIDTH + i].x += (vec[j*WIDTH+1].x*0.0001);
//            myVerts[j * WIDTH + i].z += (vec[j*WIDTH+1].z*0.01);

//            if(vec[j * WIDTH + i].length() < 100) {
//                vec[j * WIDTH + i].normalize();
//
//                myVerts[j * WIDTH + i] -= vec[j * WIDTH + i];
//                myVerts[j * WIDTH + i].z -= vec.z/100;
//            }
            if(vec[j * WIDTH + i].length() > 200) {
//                overdose = true;
//                cout << "overdose" << "\n";
            }
            
//            if(myVerts[j * WIDTH + i].z <= -100){
//                myVerts[j * WIDTH + i].z *= 10;
//            }

        }
    }
    }else{
        
        for (int i=0; i<WIDTH; i++) {
            for (int j=0; j<HEIGHT; j++) {
                
                float r = (float)pixels[j * WIDTH*3 + i * 3] / 256.0;
                float invertR = r+(2*(0.5-r));
                
                float g = (float)pixels[j * WIDTH*3 + i * 3+1] / 256.0;
                float invertG = g+(2*(0.5-g));
                
                float b = (float)pixels[j * WIDTH*3 + i * 3+2] / 256.0;
                float invertB = b+(2*(0.5-b));
                
                float brightness = (invertR+invertG+invertB) / 3.0f;
                if (pixels[j * WIDTH*3 + i * 3] < 170 || pixels[j * WIDTH*3 + i * 3+2] > 100) {
                    myVerts[j * WIDTH + i] = ofVec3f(i - WIDTH/2, j - HEIGHT/2, brightness * 256.0);
                    myColor[j * WIDTH + i] = ofFloatColor(invertR,invertG,invertB, 1.0);
                }
            }
            overdose = false;
        }
        
        myVbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
        myVbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);
         
    }
 */
    myVbo.updateVertexData(myVerts, NUM_PARTICLES);

}

//--------------------------------------------------------------
void vbo::draw(float x, float y, float z){
    
    glTranslatef(x, y, z);
    myVbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    
}
