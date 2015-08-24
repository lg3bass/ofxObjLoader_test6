#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

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
    track7.setup(appFileLoader.externalObjFiles[STARBURST],"JG_Red.png","sem",7);
    */
     
     
    track8.setup(appFileLoader.externalObjFiles[WIERDARMS3],"JG_Gold.png","sem",8);
    track9.setup(appFileLoader.externalObjFiles[WIERDARMS2],"JG_Drink01.png","sem",9);
    track10.setup(appFileLoader.externalObjFiles[WIERDARMS1],"normals.jpg","sem",10);
    
    /*
     track11.setup(appFileLoader.externalObjFiles[LIGHTNING10],"thuglee-03.jpg","sem");
     track12.setup(appFileLoader.externalObjFiles[LIGHTNING9],"thuglee-backlight-01.jpg","sem");
     track13.setup(appFileLoader.externalObjFiles[LIGHTNING8],"normals.jpg","sem");
     track14.setup(appFileLoader.externalObjFiles[LIGHTNING7],"mshade3.jpg","sem");
     track15.setup(appFileLoader.externalObjFiles[LIGHTNING6],"mshade4.jpg","sem");
     track16.setup(appFileLoader.externalObjFiles[EXTRUDETRI],"material4.png","sem");
     track17.setup(appFileLoader.externalObjFiles[EXTRUDESTAR],"material3.jpg","sem");
     track18.setup(appFileLoader.externalObjFiles[EXTRUDEBALL],"mashrim2.jpg","sem");
     track19.setup(appFileLoader.externalObjFiles[DIAMONDLAYERS],"mashrim.png","sem");
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
    track7.update();
    */
     
     
    track8.update();
    track9.update();
    track10.update();
    
    /*
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
        if(m.getAddress() == "/track8"){
            //
            track8.processOSCFrame(m.getArgAsInt32(0));
			//cout << "track 8 frame: " << ofToString(m.getArgAsInt32(0)) << endl;
		}
        if(m.getAddress() == "/track9"){
            //
            track9.processOSCFrame(m.getArgAsInt32(0));
			//cout << "track 8 frame: " << ofToString(m.getArgAsInt32(0)) << endl;
		}
        if(m.getAddress() == "/track10"){
            //
            track10.processOSCFrame(m.getArgAsInt32(0));
			//cout << "track 8 frame: " << ofToString(m.getArgAsInt32(0)) << endl;
		}
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("fps: "+ofToString(ofGetFrameRate())+" - "+ofToString(ofGetWidth())+","+ofToString(ofGetHeight())+"easyCam:"+ofToString(modkey));
    
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
        track7.draw();
    */
    
    
        track8.draw();
        track9.draw();
        track10.draw();
    
    
    cam.end();
        
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
    track7.exit();
    */
     
    
    track8.exit();
    track9.exit();
    track10.exit();
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
            case '8':
            {
                track8.keyPressed(key);
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
        */
         
         
        track8.frame++;
        track9.frame++;
        track10.frame++;
        
        /*
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

    if(key=='p'){
        //ofxUICanvas* test = guiTabBar->getActiveCanvas();
        //test->setColorFill(ofColor::blue);
        
        //guis[0]->getWidget("TRACK2")->setColorFill(ofColor::blue);
        
        
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

