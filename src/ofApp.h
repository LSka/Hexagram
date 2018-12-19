#pragma once

#include "ofMain.h"
#include "brick.hpp"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    bool bFill;
    bool rest;
    bool explode;
    bool bHelpText;
    bool bMousePressed;
    
    ofImage texture1;
    ofImage texture2;
    int mode;
    int planesNumber;
    
    float width;
    float height;
    
    float planeWidth;
    float planeHeight;
    
    float r;
    float velocity;
    
    ofVideoPlayer bgMovie;
    
    Brick bricks[3][6];
    ofVec3f startPositions[3][6];
    
    ofPlanePrimitive bgPlane;

    ofBoxPrimitive box;
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;
    
	ofCamera cam;
};
