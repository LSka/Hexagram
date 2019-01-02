#pragma once

#include "ofMain.h"
#include "ofxOsc.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    void onMessageReceived(ofxOscMessage &msg);

    enum State {REST = 0, EXPLODE = 1, COMPOSE = 2};
    enum State state;
    
    ofImage texture1;
    ofImage texture2;
    
    float width;
    float height;
    float lightZ;
    float oldTime;

    ofVideoPlayer bgMovie;

    ofPlanePrimitive bgPlane;

    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    
    ofMaterial bgMaterial;
    
    ofCamera cam;
    
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
		
};
