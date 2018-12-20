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
    acc = 0;
    direction.x = ofRandom(-1,1);
    direction.y = ofRandom(-1,1);
    brickPlane.setWidth(brickWidth);
    brickPlane.setHeight(brickHeight);
}

void Brick::update(){
    velocity += acc;
    velocity *= direction;
    position += velocity;
    brickPlane.setPosition(position);
    brickPlane.setOrientation(glm::vec3(rotationX,rotationY,0));
    
}

void Brick::draw(){
    if (visible){
        brickPlane.draw();
    }
}
