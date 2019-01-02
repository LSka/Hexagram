#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    ofSetVerticalSync(true);
    ofBackground(0);
    
    state       = REST; //set initial state t rest
    width     = ofGetWidth() * .12; //initialize a static measure unit
    height    = ofGetHeight() * .12;
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    
    bgPlane.set(screenWidth*1.45,screenHeight*1.45);
    bgPlane.setPosition(0,0,-(width*2));
    bgPlane.rotateDeg(90,0,0,1);
    
    ofDisableArbTex();
    
    //initialize lighting
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(1, 1, 1) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    pointLight.setAttenuation(1.2,.0,0);
    pointLight.setPosition((ofGetWidth()*.5), ofGetHeight()/3, 300);
    
    pointLight2.setDiffuseColor( ofFloatColor(.95, .95, .65) );
    pointLight2.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    pointLight2.setPosition(-(ofGetWidth()*.5), ofGetHeight()/3, 300);
    
    pointLight3.setDiffuseColor( ofFloatColor(.95, .95, .65) );
    pointLight3.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    lightZ = 800; //pointLight3 Z position
    pointLight3.setPosition(0,0, lightZ);
    
    
    
    //set the materials
    // shininess is a value between 0 - 128, 128 being the most shiny //

    
    bgMaterial.setShininess( 15 );
    // the light highlight of the material //
    bgMaterial.setSpecularColor(ofFloatColor(1,1,1));
    bgMaterial.setDiffuseColor(ofColor(80,80,80));
    bgMaterial.setAmbientColor(ofFloatColor(1,1,1));
    
    bgMovie.load("bgMovies/bgMovieTest_Left.mp4");
    bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.play();
    
    
    //set the camera position
    cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
    cam.rotateDeg(90,0,0,1);
    
    //initialize OSC
    receiver.setup(5000);
    sender.setup("192.168.1.255",6000);
    
    ofxOscMessage m;
    m.setAddress("/haikuit/init");
    m.addStringArg("ready");
    sender.sendMessage(m);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //send a heartbeat message every second
    float time = ofGetElapsedTimef();
    float elapsedTime = time - oldTime;
    //ofLog(OF_LOG_NOTICE,ofToString(elapsedTime));
    
    if ( elapsedTime >= 1){
        ofxOscMessage heartbeat;
        heartbeat.setAddress("/haiku/heartbeat");
        sender.sendMessage(heartbeat);
        oldTime = time;
    }
    
    bgMovie.update();
    
    switch (state) {
        case REST:
            break;
        case EXPLODE:
            break;
        case COMPOSE:
            break;
    }
    
    
    
    //move lights
    float pulse1 = (sin(time*0.5)*0.6);
    float pulse2 = (cos(time*0.4)*0.6);
    float pulse3 = (sin(time*0.1)*0.3);
    float noise = ofNoise(time*0.3);
    pointLight.setAttenuation(1.8+pulse1,0,0);
    pointLight2.setAttenuation(1.7+pulse2,0,0);
    pointLight3.setAttenuation(0.8+noise,0,0);
    pointLight3.setPosition(ofMap(noise,0.,1.,-width,width),pulse3*ofGetHeight(),lightZ);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    
    ofEnableDepthTest();
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    
    bgMaterial.begin();
    bgMovie.bind();
    ofSetColor(255,255,255);
    bgPlane.draw();
    bgMovie.unbind();
    bgMaterial.end();

    
    ofDisableLighting();
    ofDisableDepthTest();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
            
        case 'e':{
            state = EXPLODE;
            ofxOscMessage mess;
            mess.setAddress("/hexagram/state");
            mess.addIntArg(state);
            sender.sendMessage(mess);
            
            state = EXPLODE;
            bgMovie.load("dissTest.mov");
            bgMovie.setLoopState(OF_LOOP_NONE);
            bgMovie.play();
            
            break;
        }
        case 'c':{
            state = COMPOSE;
            ofxOscMessage mess;
            mess.setAddress("/hexagram/state");
            mess.addIntArg(state);
            sender.sendMessage(mess);
            break;
        }
        case 'l' :
            lightZ+= 50;
            break;
    }

}

//--------------------------------------------------------------

void ofApp::onMessageReceived(ofxOscMessage &msg)
{
    int message;
    string addr = msg.getAddress();
    if (addr.compare("/haiku/state") == 0){
      message = msg.getArgAsInt(0);
    }
    switch (message){
        case 0:
            state = REST;
            break;
        case 1:
            ofLog()<<"exploding"<<endl;
            break;
        case 2:
            state = COMPOSE;
            break;
        default:
            break;
    }
    
}
