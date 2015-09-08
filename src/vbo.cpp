#include "vbo.h"

const int vbo::WIDTH;
const int vbo::HEIGHT;
const int vbo::NUM_PARTICLES;
int vbo::sum = -1;

//--------------------------------------------------------------
void vbo::setID(int identify){
    
    ID = identify;
    
}

//--------------------------------------------------------------
void vbo::resetImg(){
   
    x = y = 0;
    char str[20];

    if(sum <= 0){
        
        sprintf(str, "test_%02d.jpg", ID);
        sum++;  //  sum is 1 when the program starts
        
    }else{
        
        sum++;
        if (sum < kNumImgs) {
            sprintf(str, "test_%02d.jpg", sum);
            
            if(sum == kNumImgs-1){
                sum = -1;
            }
            
        }
        
    }

//    cout << str << "\n";
    image.loadImage(str);
    
}

//--------------------------------------------------------------
void vbo::resetVerts(){
    
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            myVerts[j * WIDTH + i].set(i - WIDTH/2, j - HEIGHT/2, 10000);
            myColor[j * WIDTH + i].set(0.0, 0.0, 0.0);
        }
    }
    
    myVbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    myVbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    
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
            
            
            if (pixels[j * WIDTH*3 + i * 3] < 170 ||
                pixels[j * WIDTH*3 + i * 3+2] > 100) {
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
    
    overdose = true;
    
}

//--------------------------------------------------------------
void vbo::setup(){
    
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    resetImg();
    resetVerts();

}

//--------------------------------------------------------------
void vbo::update(){
    
    if (timer % 80 == 0) {
        
        if (y < HEIGHT) {
            if(x < WIDTH) {
                
                xValue = pixels[y*WIDTH*3+x*3];
                yValue = pixels[y*WIDTH*3+x*3+1];
                zValue = pixels[y*WIDTH*3+x*3+2];
                                
                x++;

            }else{
                x = 0;
                y++;
            }
            
        }else{
            y = 0;
        }

    }
    
    
    if (timer < lastingTime) {
        timer++;
    }else{
        
        timer = 0;
        resetImg();
        resetVerts();
        
    }


    if (!overdose) {
    
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
//                myVerts[j * WIDTH + i].x += (vec[j*WIDTH+i].x * vecLength[j*WIDTH+i] * 0.0001);
                myVerts[j * WIDTH + i].y += (vec[j*WIDTH+i].y * vecLength[j*WIDTH+i]/10000);
                myVerts[j * WIDTH + i].z += (vec[j*WIDTH+i].z * vecLength[j*WIDTH+i]/10000);

            }
        }
        
    }else{
        resetVerts();
    }

    
    myVbo.updateVertexData(myVerts, NUM_PARTICLES);

}

//--------------------------------------------------------------
void vbo::draw(float posX, float posY, float posZ){
    
    glTranslatef(posX, posY, posZ);
    myVbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    
}
