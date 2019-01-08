#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
	ofSetVerticalSync(true);
	ofBackground(0);
    
    ofLog() << "loading HexagramSettings.xml" <<endl;
    
    //we load our settings file
    //if it doesn't exist we can still make one
    //by hitting the 's' key
    if( settings.loadFile("HexagramSettings.xml") ){
       ofLog() << "HexagramSettings.xml loaded!" << endl;
    }
    else{
        ofLog()<< "unable to load HexagramSettings.xml check data/ folder" << endl;
    }
    
    

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    string bgTex = settings.getValue("BACKGROUND:TEXTURE", "paper.jpg");
    texture1.load(bgTex); //Background texture ===To be subsituted by background videos!===
    texture1.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );

    
    string path = settings.getValue("BRICKS:TEXTUREFOLDER","wood/images");
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    
    for (int t = 0; t < dir.size(); t++){
        ofImage tex;
        tex.load(dir.getPath(t));
        textures.push_back(tex);
    }
    
    

    bFill       = true;
    bHelpText   = false;
    state       = REST; //set initial state t rest
    width     = ofGetWidth() * .12; //initialize a static measure unit
    height    = ofGetHeight() * .12;
    
    //external ranges
    horizRange = 1300;
    vertRange = 900;
    
    
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
                    bricks[i][j].direction.y = ofMap(j,0,5,-1.,1.)*ofRandom(.9,1.);
                    break;
                }
                    //second column
                case 1 : {
                    startPositions[1][j] = ofVec3f(0, (startPosition) + d * j + height, zPos);
                    if (ofInRange(j, 0, columnsNumber/2)) bricks[i][j].direction.y = ofRandom(-.6,-.5);
                    else bricks[i][j].direction.y = ofRandom(.5,.6);
                    break;
                }
                    
                    //third column
                case 2 : {
                    startPositions[2][j] = ofVec3f(planeWidth, (startPosition) + d * j + height, zPos);
                    bricks[i][j].direction.x = ofRandom(.3,.5);;
                    bricks[i][j].direction.y = ofMap(j,0,5,-1.,1.)*ofRandom(.9,1.);
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
    bgPlane.rotateDeg(90,0,0,1);


    
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
    brickMaterial.setShininess( 80 );
    // the light highlight of the material //
    brickMaterial.setSpecularColor(ofFloatColor(.9,.9,.9));
    brickMaterial.setDiffuseColor(ofColor(1,1,1));
    brickMaterial.setAmbientColor(ofColor(0.8,0.8,0.8));
    
    bgMaterial.setShininess( 15 );
    // the light highlight of the material //
    bgMaterial.setSpecularColor(ofFloatColor(1,1,1));
    bgMaterial.setDiffuseColor(ofColor(80,80,80));
    bgMaterial.setAmbientColor(ofFloatColor(1,1,1));

   /* bgMovie.load("bgMovies/bgMovieTest_Center.mp4");
    bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.play();
  */
    
//set the camera position
    cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
    cam.rotateDeg(90,0,0,1);
    
//initialize OSC
    int oscInPort = settings.getValue("OSC:RECEIVER:PORT",5000);
    receiver.setup(oscInPort);
    string oscOutHost = settings.getValue("OSC:SENDER:HOST","192.168.1.255");
    int oscOutPort = settings.getValue("OSC:SENDER:PORT",6000);
    sender.setup(oscOutHost,oscOutPort);
    
    ofxOscMessage mess;
    mess.setAddress("/hexagram/init");
    mess.addStringArg("ready");
    sender.sendMessage(mess);
    
    heartbeat.setAddress("/hexagram/heartbeat");

}

//--------------------------------------------------------------
void ofApp::update() {
    
  Brick* b;   //Create a pointer that points to a corresponding brick in the array

    
//send a heartbeat message every second
    float time = ofGetElapsedTimef();
    float elapsedTime = time - oldTime;
    //ofLog(OF_LOG_NOTICE,ofToString(elapsedTime));
    
    if ( elapsedTime >= 1){
        sender.sendMessage(heartbeat);
        oldTime = time;
    }
   

    
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
                //float spinX = 0.1;
                //r += spinX;
 /*
//set the bricks' position to their initial state (probably not necessary)
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){
                        Brick* b; //Create a pointer that points to the corresponding brick in the array
                        b = &bricks[i][j];
                b->acc = 0;
                b->position = startPositions[i][j];
                
               // bricks[i][j].rotationX = r;
                    }
                }*/
        break;
    }
        
    case EXPLODE: {
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){
                        b = &bricks[i][j];
                        b->acc = force; //apply force to the bricks
                        //rotation is related to the distance from the points of origin
                        float distance = bricks[i][j].position.distance(startPositions[i][j]);
                        b->rotationX = distance;
                        b->rotationZ = -distance*0.8;
                    }
                }
            

                if (allOut()){ //check if all bricks are out of the camera range
//we want to randomly choose which of the central bricks will be visible and to define
//each combination with a number. To do so, we create a 6-bit bitset to store the visibility states as binary digits
                    std::bitset<6> hexa;
                    for (int i = 0; i < columnsNumber; i++){
                        for (int j = 0; j < rowsNumber; j++){
                            b = &bricks[i][j];
                            
                            //Stop the bricks
                            b->acc = 0;
                            b->velocity = ofVec3f(0,0,0);
                            b->rotationX = 0;
                            b->rotationY = 0;
                            
//the interpolator represent the normalized distance between the bricks' current position and their respective origin points.
//we'll use a simple interpolation to take them back to the origin
                            b->interpolator = 0;
                            
                            if (i == 1) { //only for the bricks in the middle column, choose if they're visible
                                float ra = ofRandom(0,1);
                                if (ra >= 0.5){
                                    b->visible = TRUE;
                                    hexa.set(j); //set the correspondent bit to TRUE
                                }
                                else{
                                    b->visible = FALSE;
                                    hexa.set(j,0); //set the correspondent bit to FALSE
                                }
                                
                            }
                        }
                    }
                    
                    int hexaId = int(hexa.to_ulong()); //convert the bit set to an integer
                    //cout<< hexaId << '\n';
                    
//Send the hexagram number to the haiku visualizers
                  /*  ofxOscMessage hexagramId;
                    hexagramId.setAddress("/haiku/hexagram");
                    hexagramId.addIntArg(hexaId);
                    sender.sendMessage(hexagramId); */

//set running state to COMPOSE and report it
                    state = COMPOSE;
                    ofxOscMessage mess;
                    mess.setAddress("/hexagram/state");
                    mess.addIntArg(state);
                    mess.addIntArg(hexaId);
                    sender.sendMessage(mess);
                }
        break;
    }
            
    case COMPOSE: {
                //force = 10;
                float distances = 0;
                for (int i = 0; i < columnsNumber; i++){
                    for (int j = 0; j < rowsNumber; j++){
                        b = &bricks[i][j];

//move back the bricks by interpolating their current position with their original position
                        b->interpolator += 0.00003; //the recomposing speed
                        b->position = b->position.interpolate(startPositions[i][j],b->interpolator);
                        
                        float distance = b->position.distance(startPositions[i][j]);
                        b->rotationX = -distance;
                        b->rotationZ = distance*.8;
                        
                        distances += distance;
                    }
                }
                //ofLog(OF_LOG_NOTICE,ofToString(distances));
//when all bricks are back in position, remodulate their explosion speed and direction
                if (distances <= 0.001 ){
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
    float pulse1 = (sin(time*0.5)*0.6);
    float pulse2 = (cos(time*0.4)*0.6);
    float pulse3 = (sin(time*0.1)*0.3);
    float noise = ofNoise(time*0.3);
    pointLight.setAttenuation(1.8+pulse1,0,0);
    pointLight2.setAttenuation(1.7+pulse2,0,0);
    pointLight3.setAttenuation(0.8+noise,0,0);
    pointLight3.setPosition(ofMap(noise,0.,1.,-width,width),pulse3*ofGetHeight(),lightZ);
    
//VISUALIZE A BOX IN THE LIGHT'S POSITION FOR DEBUGGING
   /* box.set(100,100,100);
    box.setPosition(pointLight3.getPosition());
    */
    
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
    pointLight2.enable();
    pointLight3.enable();

    bgMaterial.begin();
  //bgMovie.getTexture().bind();
    ofSetColor(255,255,255);
    texture1.bind();
    bgPlane.draw();
       texture1.unbind();
  //bgMovie.getTexture().unbind();
    bgMaterial.end();
    

    int index = 0;
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j < rowsNumber; j++){
            brickMaterial.begin();
            textures[index].bind();
            bricks[i][j].draw();
            textures[index].unbind();
            brickMaterial.end();
            index++;
            
        }
    }

   
//VISUALIZE A BOX IN THE LIGHT'S POSITION FOR DEBUGGING
   // box.draw();
    
	ofDisableLighting();
	ofDisableDepthTest();

	cam.end();
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

//visualize the help and diagnostics
    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        if (allOut()) ss << "ALL OUT"<< endl << endl;
        else ss << "IN" << endl << endl;
        ss << "STATE: " << state << endl << endl;
        ss << "Light Z: " << lightZ << endl << endl;
        ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch(key) {
        case 'h':
            bHelpText=!bHelpText;
            break;
	
        case 'e':{
            force = ofRandom(4,9);
            state = EXPLODE;
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

    //


}

bool ofApp::allOut(){
    bool out = FALSE;
    int c = 0;
    float* x;
    float* y;
    
    Brick* b;
    
    for (int i = 0; i < columnsNumber; i++){
        for (int j = 0; j< rowsNumber; j++){
            b = &bricks[i][j];
            x = &b->position.x;
            y = &b->position.y;
            if (!ofInRange(*x,-horizRange,horizRange) || !ofInRange(*y,-vertRange,vertRange)){
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
