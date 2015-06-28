#include "vbo.h"

const int vbo::WIDTH;
const int vbo::HEIGHT;
const int vbo::NUM_PARTICLES;
int vbo::sum = -1;


//--------------------------------------------------------------
void vbo::setID(int identify){
    
    ID = identify;
    sum++;  //  sum is 1 when the program starts
    
}

//--------------------------------------------------------------
void vbo::reset(){

    overdose = false;
    
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    pixels = image.getPixels();
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            
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
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            
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
    
    if (timer < 100) {
        timer++;
    }else{
        timer = 0;
//        image.clear();
        sum++;
        x = y = 0;
        char str[20];
        if (sum < 4) {
            sprintf(str, "test_%02d.jpg", sum);
            image.loadImage(str);
//            cout << str << "\n";
        }else{
            sum = 0;
            sprintf(str, "test_%02d.jpg", sum);
            image.loadImage(str);
//            cout << str << "\n";
        }
//        myVbo.clearVertices();
//        myVbo.clearColors();
        reset();
    }

    
    if (!overdose) {
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                
                myVerts[j * WIDTH + i].y += (vec[j*WIDTH+i].y * vecLength[j*WIDTH+i] * 0.0001);
                myVerts[j * WIDTH + i].z += (vec[j*WIDTH+i].z * vecLength[j*WIDTH+i] * 0.0001);


            }
        }
    }else{
        
        reset();
         
    }
 
    myVbo.updateVertexData(myVerts, NUM_PARTICLES);

}

//--------------------------------------------------------------
void vbo::draw(float x, float y, float z){
    
    glTranslatef(x, y, z);
    myVbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    
}
