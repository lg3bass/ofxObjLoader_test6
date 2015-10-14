#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    std::string file = "example.json";
    
    // Now parse the JSON
    bool parsingSuccessful = result.open(file);
    
    if (parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << result.getRawString();
    }
    
    //SYPHON
    //much nicer than processing's vs.:)
    ofSetWindowTitle("VMM 7");
    mainOutputSyphonServer.setName("Screen Output");
    
    mClient.setup();
    
    mClient.setApplicationName("Simple Server");
    mClient.setServerName("");
    //END SYPHON
    
    
    // listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    
    
    ofSetVerticalSync(true);
    //ofSetFrameRate(90);
    ofDisableArbTex();
        
    //1. get paths to the obj files.
    appFileLoader.setup();
    
    guiTabBar = new ofxUITabBar();
    ofAddListener(guiTabBar->newGUIEvent,this,&ofApp::guiTabEvent);
    
    
    //2. load all the assets.
    
    /*
    track1.setup(appFileLoader.externalObjFiles[BANDTUBE],"red_bob.jpg","sem",1);
    track2.setup(appFileLoader.externalObjFiles[BASSSTAR],"generator8.jpg","sem",2);
    track3.setup(appFileLoader.externalObjFiles[BLOCKSTAR],"silver.jpg","sem",3);
    track4.setup(appFileLoader.externalObjFiles[BOXSTAR],"ghost.jpg","sem",4);
    track5.setup(appFileLoader.externalObjFiles[UNBLOCKERBASS],"red_clay.jpg","sem",5);
    track6.setup(appFileLoader.externalObjFiles[TRACERS],"gJGSpecial_01.png","sem",6);
    */
    track7.setup(appFileLoader.externalObjFiles[STARBURST],"JG_Red.png","sem",7);
    track7.setCuePoints("30");
    track7.setDurrations("500");
    track8.setup(appFileLoader.externalObjFiles[WIERDARMS3],"JG_Gold.png","sem",8);
    track8.setCuePoints("10,20,30,40,50,60,70,80,90,100,110");
    track8.setDurrations("500,250,100,250,100,250,100,250,100,250,400");
    
    /*
    track9.setup(appFileLoader.externalObjFiles[WIERDARMS2],"JG_Drink01.png","sem",9);
    track10.setup(appFileLoader.externalObjFiles[WIERDARMS1],"normals.jpg","sem",10);
    track11.setup(appFileLoader.externalObjFiles[LIGHTNING10],"thuglee-03.jpg","sem",11);
    track12.setup(appFileLoader.externalObjFiles[LIGHTNING9],"thuglee-backlight-01.jpg","sem",12);
    track13.setup(appFileLoader.externalObjFiles[LIGHTNING8],"normals.jpg","sem",13);
    track14.setup(appFileLoader.externalObjFiles[LIGHTNING7],"mshade3.jpg","sem",14);
    track15.setup(appFileLoader.externalObjFiles[LIGHTNING6],"mshade4.jpg","sem",15);
    track16.setup(appFileLoader.externalObjFiles[EXTRUDETRI],"material4.png","sem",16);
    track17.setup(appFileLoader.externalObjFiles[EXTRUDESTAR],"material3.jpg","sem",17);
    track18.setup(appFileLoader.externalObjFiles[EXTRUDEBALL],"mashrim2.jpg","sem",18);
    track19.setup(appFileLoader.externalObjFiles[DIAMONDLAYERS],"mashrim.png","sem",19);
    */

    //save an xml per gui.
    //guiTabBar->loadSettings("settings/", "tab-");
    
    
    //Setup GUI
//    parameters.setName("settings");
//    parameters.add(track8.parameters);
//    parameters.add(track9.parameters);
//    parameters.add(track10.parameters);
//    gui.setup(parameters);
//    gui.loadFromFile("settings.xml");
//    
//    gui.setPosition(1000,50);
//
//    minimized = true;
//    
//    gui.minimizeAll();
    
    
     
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    if(modkey){
        cam.disableMouseInput();
    } else if (!modkey){
        cam.enableMouseInput();
    }
    
    /*
    track1.update();
    track2.update();
    track3.update();
    track4.update();
    track5.update();
    track6.update();
    */
    track7.update();
    track8.update();
    
    /*
    track9.update();
    track10.update();
    track11.update();
    track12.update();
    track13.update();
    track14.update();
    track15.update();
    track16.update();
    track17.update();
    track18.update();
    track19.update();
    */
    
    while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        
        cout << m.getAddress() << endl << "track:" << m.getArgAsInt32(0) <<
        ", note:" << m.getArgAsInt32(1) <<
        ", durration:" << m.getArgAsInt32(2) <<
        ", velocity:" << m.getArgAsInt32(3) <<
        ", isMuted:" << m.getArgAsInt32(4) <<
        ", targetSegment:" << m.getArgAsInt32(5) <<
        ", totalSegments:" << m.getArgAsInt32(6) <<
        ", easing:" << m.getArgAsInt32(7) <<
        endl;
        
        
        if(m.getAddress() == "/play"){
            
            if(m.getArgAsInt32(0) == 7){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track7.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 8){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track8.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 10,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 9){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track9.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 10,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 10){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track10.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 10,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 11){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track11.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 12){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track12.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 13){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track13.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 14){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track14.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 15){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track15.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 16){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track16.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 17){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track17.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 18){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track18.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 19){
                //OSCLaunch(<destination frame>, <durration>, <segment length>
                track19.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("fps: "+ofToString(ofGetFrameRate())+" - "+ofToString(ofGetWidth())+","+ofToString(ofGetHeight())+"easyCam:"+ofToString(modkey));

    
//    //SYPHON
//    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    //=================================================
    
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    ofEnableDepthTest();
    
    cam.begin();
    
    /*
        track1.draw();
        track2.draw();
        track3.draw();
        track4.draw();
        track5.draw();
        track6.draw();
     */
        track7.draw();
        track8.draw();
    
    /*
        track9.draw();
        track10.draw();
        track11.draw();
        track12.draw();
        track13.draw();
        track14.draw();
        track15.draw();
        track16.draw();
        track17.draw();
        track18.draw();
        track19.draw();
     */
    
    
    cam.end();

    
//    //SYPHON
//    mClient.draw(50, 50);
//    mainOutputSyphonServer.publishScreen();
//    //=======================================
    

    
    drawLights();// <-- NOT SURE I NEED THIS WITH MATCAP
    
    //Need this.
    ofDisableDepthTest();
    
    //draw all GUI's after this
    //----------------------------------------
    
    //gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::exit()
{
    //will load an xml per gui
    //guiTabBar->saveSettings("settings/", "tab-");
    
    for(vector<ofxUICanvas *>::iterator it = guis.begin(); it != guis.end(); ++it)
    {
        ofxUICanvas *g = *it;
        delete g;
    }
    delete guiTabBar;

    
    /*
    track1.exit();
    track2.exit();
    track3.exit();
    track4.exit();
    track5.exit();
    track6.exit();
    */
     
    track7.exit();
    track8.exit();
    
    /*
    track9.exit();
    track10.exit();
    track11.exit();
    track12.exit();
    track13.exit();
    track14.exit();
    track15.exit();
    track16.exit();
    track17.exit();
    track18.exit();
    track19.exit();
    */
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key & OF_KEY_MODIFIER){
        if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
            //cout << "F KEY:" << key << endl;
        }else{
            
            switch(key){
                case OF_KEY_LEFT_SUPER:
                    cout << "Left Apple Pressed" << endl;
                    modkey = true;
                    break;
                case OF_KEY_RIGHT_SUPER:
                    cout << "Right Apple Pressed" << endl;
                    modkey = true;
                    break;
                case OF_KEY_CONTROL:
                    cout << "Control Pressed" << endl;
                    ctrlKey = true;
                    break;
            }
        }
    } else {
        switch (key)
        {
            case '7':
            {
                track7.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track7.KeyboardLaunch(7, 500, 30, 11);
                
                if(modkey){
                    track7.randLocalPosition();
                }
                
            }
                break;
            case '8':
            {
                track8.keyPressed(key);
                track8.KeyboardLaunch(8, 500, 10, 11);
                if(modkey){
                    track8.randLocalPosition();
                }
            }
                break;
            case '9':
            {
                track9.keyPressed(key);
            }
                break;
            case '0':
            {
                track10.keyPressed(key);
            }
                break;
            default:
                break;
        }
        
    }
    
    
    
    
    
    
    switch (key)
    {
        case 'g':
        {
            guiTabBar->toggleVisible();
        }
            break;
        default:
            break;
    }
    
    if(key == ' '){
        //example: go to the center of the boxes.
        //cam.setPosition(myObjSeq.vboMeshSequence1[14].getCentroid());
        
        /*
        track1.frame++;
        track2.frame++;
        track3.frame++;
        track4.frame++;
        track5.frame++;
        track6.frame++;
        track7.frame++;
        track8.frame++;
        track9.frame++;
        track10.frame++;
        track11.frame++;
        track12.frame++;
        track13.frame++;
        track14.frame++;
        track15.frame++;
        track16.frame++;
        track17.frame++;
        track18.frame++;
        track19.frame++;
        */
    
        
    }
    
    

    
    
    
    
    if(key=='s'){
//        settings.serialize(parameters);
//        settings.save("settings.xml");
        
        guiTabBar->saveSettings("settings/", "tab-");
        
    }
    if(key=='l'){
//        settings.load("settings.xml");
//        settings.deserialize(parameters);
        //save an xml per gui.
        guiTabBar->loadSettings("settings/", "tab-");
    }
    if(key=='o'){
//        cout << track8.parameters;
//        cout << track9.parameters;
//        cout << track10.parameters;
    }

    
    if(key=='m'){
        if(minimized){
            
            gui.maximizeAll();
            minimized = false;
        } else {
            gui.minimizeAll();
            minimized = true;
        }
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key & OF_KEY_MODIFIER){
        if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
            //cout << "F KEY:" << key << endl;
        }else{
            
            switch(key){
                case OF_KEY_LEFT_SUPER:
                    cout << "Left Apple Released" << endl;
                    modkey = false;
                    break;
                case OF_KEY_RIGHT_SUPER:
                    cout << "Right Apple Released" << endl;
                    modkey = false;
                    break;
                case OF_KEY_CONTROL:
                    cout << "Control Released" << endl;
                    ctrlKey = false;
                    break;
            }
        }
    } else {
        switch (key)
        {
            case '7':
            {
                track7.keyReleased(key);
            }
                break;
            case '8':
            {
                track8.keyReleased(key);
            }
                break;
            case '9':
            {
                track9.keyReleased(key);
            }
                break;
            case '0':
            {
                track10.keyReleased(key);
            }
                break;
            default:
                break;
        }
        
        
        
    }
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    matCap.loadImage(dragInfo.files[0]);
    matCap.update();
}

//--------------------------------------------------------------
void ofApp::drawLights(){
    //NOTE: Make sure this after the cam.
    //      Otherwise rotating the cam will
    //      move the lights too.
    
    
    //ofLight light0;
    ofColor ambientColor(200.0f);
    ofColor diffuseColor(130.0f);
    ofColor specularColor(25.0f);
    light0.setPosition(ofGetWidth(), ofGetHeight()/2, 500);
    light0.setAmbientColor(ambientColor);
    light0.setDiffuseColor(diffuseColor);
    light0.setSpecularColor(specularColor);
    light0.setPointLight();
    light0.enable();
    
}

//--------------------------------------------------------------
void ofApp::guiTabEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    
    //if you want to set the highlighted item.
    //e.widget->setColorFill(ofColor::yellow);
    
    cout << "ofApp::Selected Track:" << name << endl;
}

