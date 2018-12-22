#pragma once

#include "ofMain.h"
#include "brick.hpp"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);

	
    bool bFill;
    bool bHelpText;
    bool bMousePressed;
    bool allOut();
    
    enum State {REST, EXPLODE, COMPOSE};
    enum State state;
    
    ofImage texture1;
    ofImage texture2;
    
    int columnsNumber;
    int rowsNumber;
    int counter;
    int nonVisible;
    
    float width;
    float height;
    float planeWidth;
    float planeHeight;
    float r;
    float velocity;
    float force;
    float l;
    float oldTime;
    
    ofVideoPlayer bgMovie;
    
    Brick bricks[3][6];
    ofVec3f startPositions[3][6];
    
    ofPlanePrimitive bgPlane;
    ofPlanePrimitive testPlane;
    

    ofBoxPrimitive box;
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    
    ofMaterial brickMaterial;
    ofMaterial bgMaterial;
    
	ofCamera cam;
    
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
};
