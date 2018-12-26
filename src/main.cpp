#include "ofApp.h"

//--------------------------------------------------------------
int main(){
	//ofSetupOpenGL(1440, 900, OF_FULLSCREEN);
    
    ofGLESWindowSettings settings;
    settings.setGLESVersion(2);
    settings.setSize(1440, 900);
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp()); // start the app
}
