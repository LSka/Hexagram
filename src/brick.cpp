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
    brickPlane.set(brickWidth,brickHeight,10);
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

}

