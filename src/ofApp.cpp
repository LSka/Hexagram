#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(20);

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    //texture1.load("wood.tif");
    //texture1.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );

    bFill       = true;
    bHelpText   = true;
    rest        = true;

    width     = ofGetWidth() * .12;
    height    = ofGetHeight() * .12;
    
    planeWidth = width*1.4;
    planeHeight = height*0.9;
    
    planesNumber = 6;
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float d = screenHeight / planesNumber;
    float startPosition = -screenHeight*0.54;
    
    
    
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < planesNumber; j++){
        

        bricks[i][j].brickWidth = planeWidth;
        bricks[i][j].brickHeight = planeHeight;
        
            switch(i){
                case 0 : startPositions[0][j] = ofVec3f(-planeWidth, (startPosition) + d * j + height, 0);
                    break;
                case 1 : startPositions[1][j] = ofVec3f(0, (startPosition) + d * j + height, 0);
                    break;
                case 2 : startPositions[2][j] = ofVec3f(planeWidth, (startPosition) + d * j + height, 0);
                    break;
            }
            bricks[i][j].position = startPositions[i][j];
            bricks[i][j].setup();
        }
    }
    
    bgPlane.set(screenWidth+430,screenHeight+240);

    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));


    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 60 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(63, 63, 63, 63));
    material.setDiffuseColor(ofColor(0,0,0));
    pointLight.setPosition((ofGetWidth()*.5), ofGetHeight()/2, 500);


 /*   bgMovie.load("background.mp4");
    bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.play();
  */
    
    cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
    cam.rotateDeg(90,0,0,0);
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    force = 10;
    

    
    
    float spinX = 0.1;
    r += spinX;
    
    
   // bgMovie.update();
	//ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    
    
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < planesNumber; j++){
            
            if (rest){
                bricks[i][j].position = startPositions[i][j];
                force = 0;
                bricks[i][j].rotationX = r;
            }
            else if (explode){
                float distance = bricks[i][j].position.distance(startPositions[i][j]);
                bricks[i][j].rotationX = distance;
                if (!allOut()){
                    bricks[i][j].acc = force;
                }
                else if (allOut()){
                    explode = FALSE;
                    bricks[i][j].velocity = ofVec3f(0,0,0);
                    if (i == 1) {
                        float ra = ofRandom(1);
                        if (ra >= 0.5) bricks[i][j].visible = TRUE;
                        else bricks[i][j].visible = FALSE;
                    }
                }
            }
            
            else if (!explode){
            }
            
            else{

            }
            
            bricks[i][j].update();

        }
    }
    
    pointLight.setPosition((sin(ofGetElapsedTimef()*0.5))*ofGetWidth(), ofGetHeight()/1.5, 500);

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float d = screenHeight / planesNumber;


	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();

    
    bgPlane.setPosition(0,0,-planeWidth);
  //  bgMovie.getTexture().bind();
    bgPlane.draw();
  //  bgMovie.getTexture().unbind();
    
    material.begin();
    ofFill();
    
    //texture1.getTexture().bind();
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < planesNumber; j++){
            
            bricks[i][j].draw();
            
        }
    }

    material.end();
    //texture1.getTexture().unbind();

    
	ofDisableLighting();
	ofDisableDepthTest();
	ofFill();

	cam.end();
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);


    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        if (allOut()) ss << "ALL OUT"<< endl << endl;
        else ss << "IN" << endl << endl;
        ss << "REST: " << rest << endl << endl;
        ss << "EXPLODE: " << explode << endl << endl;
        ss << counter << endl << endl;
        ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch(key) {
        case 'h':
            bHelpText=!bHelpText;
            break;
	
        case 'e':
            rest = !rest;
            explode = !explode;
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

bool ofApp::allOut(){
    bool out = FALSE;
    int c = 0;
    
    for (int i = 0; i < 3; i++){
        for (int j = 0; j< planesNumber; j++){
            float x = bricks[i][j].position.x;
            float y = bricks[i][j].position.y;
            if (x > 800 || x < -800 || y > 500 || y < -500){
                c++;
            }
        }
    }
    if (c == 18){
        out = TRUE;
    }
    else out = FALSE;
    return out;
}