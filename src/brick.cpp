//
//  brick.cpp
//  Hexagram
//
//  Created by Luca on 18/12/2018.
//

#include "brick.hpp"

Brick::Brick(){
}

void Brick::setup(){
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;
    acc = 0;
    setDirection();
    brickPlane.setWidth(brickWidth);
    brickPlane.setHeight(brickHeight);
    visible = TRUE;
}

void Brick::update(){
    velocity += acc;
    velocity *= direction;
    position += velocity;
    brickPlane.setPosition(position);
    brickPlane.setOrientation(glm::vec3(rotationX,rotationY,rotationZ));
    
}

void Brick::draw(){
    if (visible){
        brickPlane.draw();
    }
}

void Brick::setDirection(){
    //direction.x += ofRandom(-0.4,0.);
    //direction.y += ofRandom(-0.7,0.);
}


float Brick::chooseDirection(){
    float dir;
    float d = ofRandom(1);
    if (d > .5) dir = ofRandom(.6,1);
    else dir = ofRandom(-1,.6);
    return dir;
}
