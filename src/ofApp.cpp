#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(20);

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    texture1.load("Anigre_pxr128.tif");
    texture1.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );

    bFill       = true;
    bHelpText   = true;

    width     = ofGetWidth() * .12;
    height    = ofGetHeight() * .12;
    
    planeWidth = width*1.4;
    planeHeight = height*0.9;
    
    planesNumber = 6;
    
    planes.resize(planesNumber);
    planesLeft.resize(planesNumber);
    planesRight.resize(planesNumber);
    
    
    
    for (int i = 0; i < planesNumber; i++){
        planes[i].set( planeWidth, planeHeight );
        planesLeft[i].set( planeWidth, planeHeight );
        planesRight[i].set( planeWidth, planeHeight );
    }
    
    bgPlane.set(ofGetWidth(),ofGetHeight());

    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));


    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 90 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    pointLight.setPosition((ofGetWidth()*.5), ofGetHeight()/2, 500);


 /*   bgMovie.load("background.mp4");
    bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.play();
  */
}

//--------------------------------------------------------------
void ofApp::update() {

   // bgMovie.update();
	//ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    
    pointLight.setPosition((sin(ofGetElapsedTimef()*0.5))*ofGetWidth(), ofGetHeight()/1.5, 500);

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float d = screenHeight / planesNumber;
    float startPosition = -screenHeight*0.51;

    float spinX = (0.25);

	cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();

    
    bgPlane.setPosition(0,0,-10);
  //  bgMovie.getTexture().bind();
    bgPlane.draw();
  //  bgMovie.getTexture().unbind();
    
    material.begin();
    ofFill();
    
    texture1.getTexture().bind();
    for (int j = 0; j < planesNumber; j++){
        
        planes[j].setPosition(      0, (startPosition) + d * j + height, 0);
        planes[j].rotateDeg(spinX, 1.0, 0.0, 0.0);

        planesLeft[j].setPosition(      -planeWidth, (startPosition) + d * j + height, 0);
        planesLeft[j].rotateDeg(spinX, 1.0, 0.0, 0.0);
        
        planesRight[j].setPosition(      planeWidth, (startPosition) + d * j + height, 0);
        planesRight[j].rotateDeg(spinX, 1.0, 0.0, 0.0);


        

            planes[j].draw();
            planesRight[j].draw();
            planesLeft[j].draw();
        
        

    }
    material.end();
    texture1.getTexture().unbind();

    
	ofDisableLighting();
	ofDisableDepthTest();
	ofFill();

	cam.end();
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);


    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch(key) {
        case 'h':
            bHelpText=!bHelpText;
            break;
	}


    //


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
