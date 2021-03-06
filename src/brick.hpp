//
//  brick.hpp
//  Hexagram
//
//  Created by Luca on 18/12/2018.
//

#ifndef brick_hpp
#define brick_hpp

#include "ofMain.h"

class Brick {
    
public:
    void setup();
    void update();
    void draw();
    void setDirection();
    
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f direction;
    
    float acc;
    float brickWidth;
    float brickHeight;
    float rotationX;
    float rotationY;
    float rotationZ;
    float interpolator;
    
    bool visible;
    
    ofBoxPrimitive brickPlane;
    
    
    Brick();
    
private:

};

#endif /* brick_hpp */
