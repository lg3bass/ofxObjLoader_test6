#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    std::string file = "vmm_tracks.json";
    
    // Now parse the JSON
    bool parsingSuccessful = result.open(file);

// MOVED TO TRACK
//    if (parsingSuccessful) {
//        
//        ofxJSONElement json_track8 = result["track"+ofToString(8)];
//        ofLogNotice("json_track8:") << json_track8.getRawString();
//        
//        getTrackJSON(WIERDARMS3, "objSeq-cues");
//        getTrackJSON(WIERDARMS3, "objSeq-durations");
//    }
    
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
    
    if (parsingSuccessful) {
    //2. load all the assets.
        
        
        track1.setup(appFileLoader.externalObjFiles[BANDTUBE],result["track"+ofToString(1)]);
        track2.setup(appFileLoader.externalObjFiles[BASSSTAR],result["track"+ofToString(2)]);
        track3.setup(appFileLoader.externalObjFiles[BLOCKSTAR],result["track"+ofToString(3)]);
        track4.setup(appFileLoader.externalObjFiles[BOXSTAR],result["track"+ofToString(4)]);
        track5.setup(appFileLoader.externalObjFiles[UNBLOCKERBASS],result["track"+ofToString(5)]);
        track6.setup(appFileLoader.externalObjFiles[TRACERS],result["track"+ofToString(6)]);
        
        track7.setup(appFileLoader.externalObjFiles[STARBURST],result["track"+ofToString(7)]);
        track8.setup(appFileLoader.externalObjFiles[WIERDARMS3],result["track"+ofToString(8)]);
        track9.setup(appFileLoader.externalObjFiles[WIERDARMS2],result["track"+ofToString(9)]);
        track10.setup(appFileLoader.externalObjFiles[WIERDARMS1],result["track"+ofToString(10)]);
        track11.setup(appFileLoader.externalObjFiles[LIGHTNING10],result["track"+ofToString(11)]);

        track12.setup(appFileLoader.externalObjFiles[LIGHTNING9],result["track"+ofToString(12)]);
        track13.setup(appFileLoader.externalObjFiles[LIGHTNING8],result["track"+ofToString(13)]);
        track14.setup(appFileLoader.externalObjFiles[LIGHTNING7],result["track"+ofToString(14)]);
        track15.setup(appFileLoader.externalObjFiles[LIGHTNING6],result["track"+ofToString(15)]);
        track16.setup(appFileLoader.externalObjFiles[EXTRUDETRI],result["track"+ofToString(16)]);
        track17.setup(appFileLoader.externalObjFiles[EXTRUDESTAR],result["track"+ofToString(17)]);
        track18.setup(appFileLoader.externalObjFiles[EXTRUDEBALL],result["track"+ofToString(18)]);
        track19.setup(appFileLoader.externalObjFiles[DIAMONDLAYERS],result["track"+ofToString(19)]);

        
    }
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
    
    
    track1.update();
    track2.update();
    track3.update();
    track4.update();
    track5.update();
    track6.update();
    
    track7.update();
    track8.update();
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
    
    
    while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        /*
        cout << m.getAddress() << endl << "track:" << m.getArgAsInt32(0) <<
        ", note:" << m.getArgAsInt32(1) <<
        ", durration:" << m.getArgAsInt32(2) <<
        ", velocity:" << m.getArgAsInt32(3) <<
        ", isMuted:" << m.getArgAsInt32(4) <<
        ", targetSegment:" << m.getArgAsInt32(5) <<
        ", totalSegments:" << m.getArgAsInt32(6) <<
        ", easing:" << m.getArgAsInt32(7) <<
        endl;
        */
        
        if(m.getAddress() == "/play"){
            if(m.getArgAsInt32(0) == 1){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track1.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 2){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track2.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 20,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 3){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track3.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 4){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track4.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 5){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track5.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 20,m.getArgAsInt32(7));
            }
            if(m.getArgAsInt32(0) == 6){
                //OSCLaunch(<destination frame>, <durration>, <segment length>, <easingProfile>
                track6.OSCLaunch(m.getArgAsInt32(5), m.getArgAsInt32(2), 30,m.getArgAsInt32(7));
            }
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
        } else if (m.getAddress() == "/randomTrans"){
            
            switch (m.getArgAsInt32(0)) {
                case 1:
                    track1.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 2:
                    track2.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 3:
                    track3.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 4:
                    track4.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 5:
                    track5.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 6:
                    track6.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 7:
                    track7.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 8:
                    track8.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 9:
                    track9.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 10:
                    track10.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 11:
                    track11.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 12:
                    track12.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 13:
                    track13.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 14:
                    track14.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 15:
                    track15.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 16:
                    track16.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 17:
                    track17.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 18:
                    track18.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                case 19:
                    track19.randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
                    break;
                default:
                    break;
            }
            
        } else if (m.getAddress() == "/clear"){
            switch (m.getArgAsInt32(0)) {
                case 1:
                    track1.clear();
                    break;
                case 2:
                    track2.clear();
                    break;
                case 3:
                    track3.clear();
                    break;
                case 4:
                    track4.clear();
                    break;
                case 5:
                    track5.clear();
                    break;
                case 6:
                    track6.clear();
                    break;
                case 7:
                    track7.clear();
                    break;
                case 8:
                    track8.clear();
                    break;
                case 9:
                    track9.clear();
                    break;
                case 10:
                    track10.clear();
                    break;
                case 11:
                    track11.clear();
                    break;
                case 12:
                    track12.clear();
                    break;
                case 13:
                    track13.clear();
                    break;
                case 14:
                    track14.clear();
                    break;
                case 15:
                    track15.clear();
                    break;
                case 16:
                    track16.clear();
                    break;
                case 17:
                    track17.clear();
                    break;
                case 18:
                    track18.clear();
                    break;
                case 19:
                    track19.clear();
                    break;
                default:
                    break;
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
    
    
    track1.draw();
    track2.draw();
    track3.draw();
    track4.draw();
    track5.draw();
    track6.draw();
    track7.draw();
    track8.draw();
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
    
    
    cam.end();

    
//    //SYPHON
//    mClient.draw(50, 50);
//    mainOutputSyphonServer.publishScreen();
//    //=======================================
    

    
    //drawLights();// <-- NOT SURE I NEED THIS WITH MATCAP, disabled 10/22/15
    
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

    
    
    track1.exit();
    track2.exit();
    track3.exit();
    track4.exit();
    track5.exit();
    track6.exit();
    track7.exit();
    track8.exit();
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
            case '1':
            {
                track11.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track11.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track11.randLocalPosition(-15,15,600,400);
                }
                if(ctrlKey){
                    track11.clear();
                }
                
            }
                break;
            case '2':
            {
                track12.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track12.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track12.randLocalPosition(-15,15,600,400);
                }
                if(ctrlKey){
                    track12.clear();
                }
                
            }
                break;
            case '3':
            {
                track13.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track13.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track13.randLocalPosition(-15,15,600,400);
                }
                
            }
                break;
            case '4':
            {
                track14.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track14.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track14.randLocalPosition(-15,15,600,400);
                }
                
            }
                break;
            case '5':
            {
                track15.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track15.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track15.randLocalPosition(-15,15,600,400);
                }
                
            }
                break;
            case '6':
            {
                track16.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track16.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track16.randLocalPosition(-15,15,600,400);
                }
                
            }
                break;
            case '7':
            {
                track7.keyPressed(key);
                //OSCLaunch(<key-unused>,<durration-unused>,<sequenceLength>,<easing>
                track7.KeyboardLaunch(7, 500, 30, 11);
                if(modkey){
                    //randLocalPosition(<lowRand>,<highRand>,<durration>,<delay>);
                    track7.randLocalPosition(-15,15,600,400);
                }
                
            }
                break;
            case '8':
            {
                track8.keyPressed(key);
                track8.KeyboardLaunch(8, 500, 10, 11);
                if(modkey){
                    track8.randLocalPosition(-15,15,600,400);
                }
            }
                break;
            case '9':
            {
                track9.keyPressed(key);
                track9.KeyboardLaunch(9, 500, 10, 11);
                if(modkey){
                    track9.randLocalPosition(-15,15,600,400);
                }
            }
                break;
            case '0':
            {
                track10.keyPressed(key);
                track10.KeyboardLaunch(10, 500, 10, 11);
                if(modkey){
                    track10.randLocalPosition(-15,15,600,400);
                }
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

//THIS FUNCTION IS NOT NEEDED ANYMORE
//--------------------------------------------------------------
void ofApp::getTrackJSON(int _track, string _param){
    
            cout << "[track" << _track << "]" << "[" << _param << "]:";
            
            for(int j=0;j<result["track"+ofToString(_track)][_param].size();j++){
                
                //durrations list
                cout << result["track"+ofToString(_track)][_param][j].asInt();
                
                if(j<result["track"+ofToString(_track)][_param].size()-1){
                    cout << ",";
                }
                
            }
            cout << endl;
    
}
