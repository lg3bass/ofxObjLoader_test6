#include "vboMeshObj.h"
#include "ofApp.h"

//--------------------------------------------------------------
vboMeshObj::vboMeshObj() {
    frame = 0;
    lastFrame = 0;

    //setup default params
    params.isPlaying = false;
    params.solo = false;
    params.oscControlled = true;
    
    params.g_copies = 1;
    params.g_scale = 10.0;
    params.g_rot = 0.0;
    params.g_trans = ofVec3f(0.0,0.0,0.0);
    
    params.l_copies = 4;
    params.l_scale = 1.0;
    params.l_rot = -90.0;
    params.l_trans = ofVec3f(0.0,0.0,0.0);
    
    
    
}

//--------------------------------------------------------------
void vboMeshObj::setup(const objFileLoader::extObjFile &_input, string _img, string _shader, int _index){
  
    index = _index;
    
    trackData = _input;
    
    //load all the vboMeshes
    setupVboMesh(trackData);
    
    setMatCap(_img);
    setShader(_shader);
    
    /*
    parameters.setName("testName");
    parameters.add(gui_gInstances.set("G instances",1,1,10));
    parameters.add(gui_gScale.set("G scale",10.0,1.0,50.0));
    parameters.add(gui_gRotAngle.set("G angle",0.0,-180.0,180.0));
    parameters.add(gui_gTrans.set("G trans",
                                 ofVec3f(0.0,0.0,0.0),
                                 ofVec3f(-100.0,-100.0,-100.0),
                                 ofVec3f(100.0,100.0,100.0)));
    parameters.add(gui_lInstances.set("L instances",4,1,10));
    parameters.add(gui_lRotAngle.set("L angle",-90.0,-180.0,180.0));
    parameters.add(gui_lTrans.set("L trans",
                                 ofVec3f(0.0,0.0,0.0),
                                 ofVec3f(-100.0,-100.0,-100.0),
                                 ofVec3f(100.0,100.0,100.0)));
    
    */
    
    
    //setup the ofxUI GUI
    setupGui(_index);
    
    reportParams(_index);

}

void vboMeshObj::reportParams(int _i){
    cout << "reported params-" << ofToString(_i) << endl;
    cout << "g_copies-" << params.g_copies << endl;
    cout << "g_scale-" << params.g_scale << endl;
    cout << "g_rot-" << params.g_rot << endl;
    cout << "g_trans-" << params.g_trans << endl;
    
    cout << "l_copies-" << params.l_copies << endl;
    cout << "l_scale-" << params.l_scale << endl;
    cout << "l_rot-" << params.l_rot << endl;
    cout << "l_trans:-" << params.l_trans << endl;
}


//--------------------------------------------------------------
vector<ofVboMesh> vboMeshObj::passObjTwoVboMesh(vector<ofFile> _files){
    
    ofMesh original;
    
    vector<ofVboMesh> tempVboMeshSequence;
    
    for(vector<ofFile>::iterator it = _files.begin(); it != _files.end(); ++it) {
        
        ofxObjLoader::load((*it).getAbsolutePath(), original, false, false);
        ofMesh temp = original;
        tempVboMeshSequence.push_back(temp);
        //load all the obj files to the vboMeshBuffer. This is the reason why it takes so long to load.
        cout << (*it).getFileName() << endl;
        
    }
    
    return tempVboMeshSequence;
    
}


//--------------------------------------------------------------
void vboMeshObj::setupVboMesh(const objFileLoader::extObjFile &_input){
    
    frame = 0;
    numFiles = _input.numFiles;
    
    vboMesh1.clear();
    vboMesh1 = passObjTwoVboMesh(_input.objs);

    
}

//--------------------------------------------------------------
void vboMeshObj::setMatCap(string _img){
    matCap.loadImage("matCap/"+_img);
}

//--------------------------------------------------------------
void vboMeshObj::setShader(string _shader){
    shader.load("shaders/"+_shader);
}

//--------------------------------------------------------------
void vboMeshObj::draw(){
    
    //accumulate transform stacks.
    for(int j=0;j<params.g_copies;j++){
        glPushMatrix();
        //global scale
        glScalef(params.g_scale, params.g_scale, params.g_scale);//scale of this layer
        //global trans
        glTranslatef(params.g_trans.x,params.g_trans.y,j*params.g_trans.z);
        //global rot
        glRotatef(params.g_rot,0,0,1);//z axis.
        for(int i=0;i<params.l_copies;i++){
            glPushMatrix();
            glRotatef(i*params.l_rot,0,0,1);//z axis
            glTranslatef(params.l_trans.x, params.l_trans.y, params.l_trans.z);
            shader.begin();
                shader.setUniformTexture("tMatCap", matCap, 1);
                vboMesh1[frame].draw();
            shader.end();
            glPopMatrix();
        }
        
        glPopMatrix();
    }
    
}

//--------------------------------------------------------------
void vboMeshObj::update(){
    
    
    //cout << "track 8 frame: " << frame << endl;

    
    
    if(params.isPlaying){
        if(params.oscControlled){
            if(frame < incomingFrame){
                
                frame++;
                cout << "track 8 frame: *" << frame << endl;
            } else {
                params.isPlaying = false;
            }
        } else {
            //key pressed functions
            if(frame < numFiles-1){
                frame++;
            } else {
                frame = 0;
                params.isPlaying = false;
            }
        }
        
        

        
    } else {
        
        //key pressed functions
//        frame = 0;
//        params.isPlaying = false;
    }
    

}

//--------------------------------------------------------------
void vboMeshObj::processOSCFrame(int _frame){
    
    
    //stop advancing
    
    //store the last frame.
    lastFrame = frame;
    //set the current frame.
    incomingFrame = _frame;
    
    //first check if incoming frame is greater than current frame
    if(incomingFrame > lastFrame){
        params.isPlaying = false;
        
        //if the incoming frame is within the range x+9
        if(incomingFrame > (lastFrame+1) && incomingFrame < (lastFrame+20)){
            //cout << "---- frame rate has jumped" << endl;
            params.isPlaying = true;
        } else {
            //else: just jump cut forward
            frame = incomingFrame;
            cout << "track 8 frame: " << frame << endl;
        }
        
    } else {
    // else: jump cut backwards
        frame = incomingFrame;
        cout << "track 8 frame: " << frame << endl;
        
    }

    
}


//--------------------------------------------------------------
void vboMeshObj::advanceFrame(){
    frame++;
}

//--------------------------------------------------------------
void vboMeshObj::play() {
    
    counter = 0;
    params.isPlaying = true;
    
}

//--------------------------------------------------------------
void vboMeshObj::setupGui(int _index){
    
    string index = ofToString(_index);
    
    // TAKE 2
    ofxUICanvas* gui = new ofxUICanvas();
    gui->setName("TRACK" + index);
    gui->addLabel("TRACK" + index);
    
    gui->addToggle("SOLO", &params.solo);
    gui->addToggle("OSC", &params.oscControlled);
    
    gui->addLabel("GLOBAL");
    gui->addIntSlider("(G)COPIES " + index, 1, 12, &params.g_copies);
    
    gui->addSlider("(G)SCALE", 1.0, 50.0, &params.g_scale);
    gui->addSlider("(G)ROTATE", -180.0, 180.0, &params.g_rot);
    gui->addSlider("(G)TRANS-X", -100.0, 100.0, &params.g_trans.x);
    gui->addSlider("(G)TRANS-Y", -100.0, 100.0, &params.g_trans.y);
    gui->addSlider("(G)TRANS-Z", -100.0, 100.0, &params.g_trans.z);
    
// Minimal Slider not passing a reliable value
//    gui->addMinimalSlider("(G)SCALE", 1.0, 50.0, &params.g_scale,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(G)ROTATE", -180.0, 180.0, &params.g_rot,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(G)TRANS-X", -100.0, 100.0, &params.g_trans.x,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(G)TRANS-Y", -100.0, 100.0, &params.g_trans.y,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(G)TRANS-Z", -100.0, 100.0, &params.g_trans.z,OFX_UI_FONT_SMALL);

    
    gui->addLabel("LOCAL");
    gui->addIntSlider("(L)COPIES " + index, 1, 12, &params.l_copies);

    
    gui->addSlider("(L)SCALE", 1.0, 50.0, &params.l_scale);
    gui->addSlider("(L)ROTATE", -180.0, 180.0, &params.l_rot);
    gui->addSlider("(L)TRANS-X", -100.0, 100.0, &params.l_trans.x);
    gui->addSlider("(L)TRANS-Y", -100.0, 100.0, &params.l_trans.y);
    gui->addSlider("(L)TRANS-Z", -100.0, 100.0, &params.l_trans.z);

    // Minimal Slider not passing a reliable value
//    gui->addMinimalSlider("(L)SCALE", 1.0, 50.0, &params.l_scale,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(L)ROTATE", -180.0, 180.0, &params.l_rot,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(L)TRANS-X", -100.0, 100.0, &params.l_trans.x,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(L)TRANS-Y", -100.0, 100.0, &params.l_trans.y,OFX_UI_FONT_SMALL);
//    gui->addMinimalSlider("(L)TRANS-Z", -100.0, 100.0, &params.l_trans.z,OFX_UI_FONT_SMALL);
    
    //gui->add2DPad("PAD " + index, ofVec2f(-1, 1), ofVec2f(-1, 1), ofVec2f(0.0, 0.0));
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&vboMeshObj::guiEvent);
    
    //add this tracks gui to the tabbar
    ((ofApp*)ofGetAppPtr())->guiTabBar->addCanvas(gui);
    ((ofApp*)ofGetAppPtr())->guis.push_back(gui);
    
}

//--------------------------------------------------------------
void vboMeshObj::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    
    //int sliderVal = e.widget->
    cout << "vboMeshObj::" << canvasParent << "-" << kind << "-" << name << "-" << "END" << endl;
}

//--------------------------------------------------------------
void vboMeshObj::keyPressed(int key)
{
    
    if(!params.oscControlled){
        cout << "vboMeshObj::keyPressed" << ofToString(index) << "-KEY:" << ofToString(key) << endl;
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::red);
        params.isPlaying = true;
    }

    

}

//--------------------------------------------------------------
void vboMeshObj::keyReleased(int key)
{
    if(!params.oscControlled){
        cout << ofToString(index) << " released from withing vboMeshObj" << endl;
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::black);
        params.isPlaying = false;
    }

    

}

//--------------------------------------------------------------
void vboMeshObj::exit()
{

    
    
}
