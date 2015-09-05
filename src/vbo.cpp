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
                    myVerts[j * WIDTH + i] = ofVec3f(i - WIDTH/2,
                                                     j - HEIGHT/2,
                                                     brightness * 256.0);
                    myColor[j * WIDTH + i] = ofFloatColor(invertR, invertG, invertB, 1.0);
                    
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
    
    reset();

}

//--------------------------------------------------------------
void vbo::update(){
    
    if (timer % 80 == 0) {
        
        if (y < HEIGHT) {
            if(x < WIDTH) {
                
                xRange = pixels[y*WIDTH*3+x*3];
                yRange = pixels[y*WIDTH*3+x*3+1];
                zRange = pixels[y*WIDTH*3+x*3+2];
                                
                x++;

            }else{
                x = 0;
                y++;
            }
            
        }else{
            y = 0;
        }

    }
    
    
    if (timer < 10000) {
        timer++;
    }else{
        
        timer = 0;
        sum++;
        x = y = 0;
        
        char str[20];
        if (sum < kNumImgs) {
            
            sprintf(str, "test_%02d.jpg", sum);
            image.loadImage(str);

        }else{
            
            sum = 0;
            sprintf(str, "test_%02d.jpg", sum);
            image.loadImage(str);

        }

        reset();
        
    }

    
    if (!overdose) {
        
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
//                myVerts[j * WIDTH + i].x += (vec[j*WIDTH+i].x * vecLength[j*WIDTH+i] * 0.0001);
//                myVerts[j * WIDTH + i].y += (vec[j*WIDTH+i].y * vecLength[j*WIDTH+i] * 0.0001);
//                myVerts[j * WIDTH + i].z += (vec[j*WIDTH+i].z * vecLength[j*WIDTH+i] * 0.0001);

            }
        }
        
    }else{
        reset();
    }
 
    
    myVbo.updateVertexData(myVerts, NUM_PARTICLES);

}

//--------------------------------------------------------------
void vbo::draw(float posX, float posY, float posZ){
    
    glTranslatef(posX, posY, posZ);
    myVbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    
}
