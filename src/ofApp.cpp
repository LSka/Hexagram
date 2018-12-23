#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(0);

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    texture1.load("paper.jpg"); //Background texture ===To be subsituted by background videos!===
    texture1.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );

    bFill       = true;
    bHelpText   = false;
    state       = REST; //set initial state t rest
    width     = ofGetWidth() * .12; //initialize a static measure unit
    height    = ofGetHeight() * .12;
    
    
    //these are the bricks dimensions
    planeWidth = width*1.4;
    planeHeight = height*0.9;
    
    columnsNumber = 3;
    rowsNumber = 6;
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float d = screenHeight / rowsNumber;
    float startPosition = -screenHeight*0.54; //where the first row will appear
    
    float zPos = -width; //Z position of the hexagram
    
    
  //Set the initial positions and direction relative to the position of the bricks
  // To do so, we store an array with the initial positions which every brick will refer when turning back after the explosion
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j < rowsNumber; j++){
        

        bricks[i][j].brickWidth = planeWidth;
        bricks[i][j].brickHeight = planeHeight;
        
            switch(i){
                    //fist column
                case 0 : {
                    startPositions[0][j] = ofVec3f(-planeWidth, (startPosition) + d * j + height, zPos);
                    bricks[i][j].direction.x = ofRandom(-.9,-.7);
                    bricks[i][j].direction.y = ofMap(j,0,5,-1.,1.);
                    break;
                }
                    //second column
                case 1 : {
                    startPositions[1][j] = ofVec3f(0, (startPosition) + d * j + height, zPos);
                    if (ofInRange(j, 0, columnsNumber/2)) bricks[i][j].direction.y = ofRandom(.3,.7);
                    else bricks[i][j].direction.y = ofRandom(-.3,-.7);
                    break;
                }
                    
                    //third column
                case 2 : {
                    startPositions[2][j] = ofVec3f(planeWidth, (startPosition) + d * j + height, zPos);
                    bricks[i][j].direction.x = ofRandom(.3,.5);;
                    bricks[i][j].direction.y = ofMap(j,0,5,-1.,1.);
                    break;
                }
            }
            bricks[i][j].position = startPositions[i][j];
            
            //create and intialize the bricks
            bricks[i][j].setup();
        }
    }

//initialize the background plane
    bgPlane.set(screenWidth*1.45,screenHeight*1.45);
    bgPlane.setPosition(0,0,-(width*2));


    
//initialize lighting
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(1, 1, 1) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor(.95, .95, .65) );
    pointLight2.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight.setPosition((ofGetWidth()*.5), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5), 0, 0);

    
//set the materials
    // shininess is a value between 0 - 128, 128 being the most shiny //
    brickMaterial.setShininess( 120 );
    // the light highlight of the material //
    brickMaterial.setSpecularColor(ofColor(80, 80, 80, 80));
    brickMaterial.setDiffuseColor(ofColor(0,0,0));
    
    bgMaterial.setShininess( 63 );
    // the light highlight of the material //
    bgMaterial.setSpecularColor(ofColor(63, 63, 63, 63));
    bgMaterial.setDiffuseColor(ofColor(1,1,1));

 /*   bgMovie.load("background.mp4");
    bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.play();
  */
    
//set the camera position
    cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
    cam.rotateDeg(90,0,0,0);
    
//initialize OSC
    receiver.setup(5000);
    sender.setup("localhost",6000);
    
    ofxOscMessage m;
    m.setAddress("/hexagram/init");
    m.addStringArg("ready");
    sender.sendMessage(m);
    
}

//--------------------------------------------------------------
void ofApp::update() {

    
//send a heartbeat message every second
    float time = ofGetElapsedTimef();
    float elapsedTime = time - oldTime;
    //ofLog(OF_LOG_NOTICE,ofToString(elapsedTime));
    
    if ( elapsedTime >= 1){
        ofxOscMessage heartbeat;
        heartbeat.setAddress("/hexagram/heartbeat");
        sender.sendMessage(heartbeat);
        oldTime = time;
    }
   
   // bgMovie.update();
	//ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));

    
//update the bricks' state based on the set state
switch (state) {
    case REST: {
        receiver.start(); //receive messages only if we are in REST state
        if (receiver.hasWaitingMessages()){
            ofxOscMessage receivedMessage;
            receiver.getNextMessage(receivedMessage);
            string addr = receivedMessage.getAddress();
            
            if (addr.compare("/force") == 0){ //if the OSC address corresponds to /force
                force = receivedMessage.getArgAsFloat(0); //set the force to the received parameter
                state = EXPLODE; //set up the explosion
                //broadcast the new state
                ofxOscMessage mess;
                mess.setAddress("/hexagram/state");
                mess.addIntArg(state);
                sender.sendMessage(mess);
                receiver.stop(); //stop the OSC receiver to avoid double triggering
                //ofLog(OF_LOG_NOTICE, ofToString(receivedForce));
                
                
            }
        }
                float spinX = 0.1;
                //r += spinX;
        
//set the bricks' position to their initial state (probably not necessary)
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){
                bricks[i][j].acc = 0;
                bricks[i][j].position = startPositions[i][j];
                
               // bricks[i][j].rotationX = r;
                    }
                }
        break;
    }
        
    case EXPLODE: {
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){
                        bricks[i][j].acc = force; //apply force to the bricks
                        //rotation is related to the distance from the points of origin
                        float distance = bricks[i][j].position.distance(startPositions[i][j]);
                        bricks[i][j].rotationX = distance;
                        bricks[i][j].rotationZ = -distance;
                    }
                }
            

                if (allOut()){ //check if all bricks are out of the camera range
//we want to randomly choose which of the central bricks will be visible and to define
//each combination with a number. To do so, we create a 6-bit bitset to store the visibility states as binary digits
                    std::bitset<6> hexa;
                    nonVisible = 0;
                    for (int i = 0; i < columnsNumber; i++){
                        for (int j = 0; j < rowsNumber; j++){
                            
                            //Stop the bricks
                            bricks[i][j].acc = 0;
                            bricks[i][j].velocity = ofVec3f(0,0,0);
                            bricks[i][j].rotationX = 0;
                            bricks[i][j].rotationY = 0;
                            
//the interpolator represent the normalized distance between the bricks' current position and their respective origin points.
//we'll use a simple interpolation to take them back to the origin
                            bricks[i][j].interpolator = 0;
                            
                            if (i == 1) { //only for the bricks in the middle column, choose if they're visible
                                float ra = ofRandom(0,1);
                                if (ra >= 0.5){
                                    bricks[i][j].visible = TRUE;
                                    hexa.set(j); //set the correspondent bit to TRUE
                                }
                                else{
                                    bricks[i][j].visible = FALSE;
                                    nonVisible++;
                                    hexa.set(j,0); //set the correspondent bit to FALSE
                                }
                                
                            }
                        }
                    }
                    
                    int hexaId = int(hexa.to_ulong()); //convert the bit set to an integer
                    //cout<< hexaId << '\n';
                    
//Send the hexagram number to the haiku visualizers
                    ofxOscMessage hexagramId;
                    hexagramId.setAddress("/haiku/hexagram");
                    hexagramId.addIntArg(hexaId);
                    sender.sendMessage(hexagramId);

//set running state to COMPOSE and report it
                    state = COMPOSE;
                    ofxOscMessage mess;
                    mess.setAddress("/hexagram/state");
                    mess.addIntArg(state);
                    sender.sendMessage(mess);
                }
        break;
    }
            
    case COMPOSE: {
                //force = 10;
                float distances = 0;
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){

//move back the bricks by interpolating their current position with their original position
                        bricks[i][j].interpolator += 0.00002 * force;
                        bricks[i][j].position = bricks[i][j].position.interpolate(startPositions[i][j],bricks[i][j].interpolator);
                        
                        float distance = bricks[i][j].position.distance(startPositions[i][j]);
                        bricks[i][j].rotationX = -distance;
                        bricks[i][j].rotationZ = distance;
                        
                        distances += distance;
                    }
                }
               // ofLog(OF_LOG_NOTICE,ofToString(interpols));
//when all bricks are back in position, remodulate their explosion speed and direction
                if (distances <= 0 ){
                    for (int i = 0; i < columnsNumber; i++){
                        for (int j = 0; j < rowsNumber; j++){
                            bricks[i][j].setDirection();
                           // ofLog(OF_LOG_NOTICE,ofToString(bricks[i][j].direction));
                        }
                    }
                    //r = 0;
//get back to REST state and report
                    state = REST;
                    ofxOscMessage mess;
                    mess.setAddress("/hexagram/state");
                    mess.addIntArg(state);
                    sender.sendMessage(mess);
                    
                }
        break;
    }
}
        
//Update the bricks' state
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j < rowsNumber; j++){
            bricks[i][j].update();
        }
    }
    
//move lights
    //pointLight.setPosition((sin(ofGetElapsedTimef()*0.5))*ofGetWidth(), ofGetHeight()/1.5, 500);
    pointLight2.setPosition(cos(ofGetElapsedTimef())*ofGetWidth(), 0, 100);

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();
    float d = screenHeight / rowsNumber;


	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();

  //  bgMovie.getTexture().bind();
    ofSetColor(255,255,255);
    ofFill();
    texture1.bind();
    bgPlane.draw();
    texture1.unbind();
  //  bgMovie.getTexture().unbind();
    //bgMaterial.end();
    
    brickMaterial.begin();
    ofSetColor(0,0,0);
    ofFill();
    
    //texture1.getTexture().bind();
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j < rowsNumber; j++){
            
            bricks[i][j].draw();
            
        }
    }

    brickMaterial.end();
    //texture1.getTexture().unbind();

    
	ofDisableLighting();
	ofDisableDepthTest();
	ofFill();

	cam.end();
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

//visualize the help and diagnostics
    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        if (allOut()) ss << "ALL OUT"<< endl << endl;
        else ss << "IN" << endl << endl;
        ss << "STATE: " << state << endl << endl;
        ss << "NON VISIBLE: " << nonVisible << endl << endl;
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
            force = ofRandom(4,9);
            state = EXPLODE;
            ofxOscMessage mess;
            mess.setAddress("/hexagram/state");
            mess.addIntArg(state);
            sender.sendMessage(mess);
            break;
    }

    //


}

bool ofApp::allOut(){
    bool out = FALSE;
    int c = 0;
    
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j< rowsNumber; j++){
            float x = bricks[i][j].position.x;
            float y = bricks[i][j].position.y;
            if (!ofInRange(x,-1100,1100) || !ofInRange(y,-700,700)){
                c++;
            }
        }
    }
    if (c == columnsNumber*rowsNumber){
        out = TRUE;
    }
    else out = FALSE;
    return out;
}
