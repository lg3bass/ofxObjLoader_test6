#include "vboMeshObj.h"
#include "ofApp.h"

//--------------------------------------------------------------
vboMeshObj::vboMeshObj() {


    //setup default params
    params.isPlaying = false;
    params.solo = false;
    params.oscControlled = true;
    params.randomized = false;
    params.mirrored = true;
    params.tweenType = 11;
    
    params.g_copies = 1;
    params.g_scale = 10.0;
    //params.g_rot = 0.0;
    params.g_rotate = ofVec3f(0.0,0.0,0.0);
    params.g_trans = ofVec3f(0.0,0.0,0.0);
    
    params.l_copies = 4;
    params.l_scale = 1.0;
    //params.l_rot = -90.0;
    params.l_rotate = ofVec3f(0.0,0.0,-90.0);
    params.l_trans = ofVec3f(0.0,0.0,0.0);
    
    params.mirror_distance = 10.0;
    params.durration_in_ms = 1000.0;
    params.currentSegment = 0;
    
    frame = 0;

    
}

//--------------------------------------------------------------
void vboMeshObj::setup(const objFileLoader::extObjFile &_input, ofxJSONElement _trackData){
    jsonTrackData = _trackData;
    
    index = jsonTrackData["index"].asInt();
    
    //print out the json data.
    ofLogNotice("json_track")<< index << ": " << jsonTrackData.getRawString();

    trackData = _input;
    
    //load all the vboMeshes
    setupVboMesh(trackData);
    
    setMatCap(jsonTrackData["matCap-img"].asString());
    setShader(jsonTrackData["matCap-shader"].asString());
    
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
    
    
    frame = 0;//duplicate in constructor
    
    //setup the ofxUI GUI
    setupGui(index);
    

    //setup more params
    params.cuePoints = parseJSON("objSeq-cues");
    params.durrationPoints = parseJSON("objSeq-durations");
    
    
    //output all my params to check
    reportParams(index);
    
    

}

//--------------------------------------------------------------
vector<int> vboMeshObj::parseJSON(string _param){
    
    vector<int> returnedVector;
    
    for (int i=0; i<jsonTrackData[_param].size(); i++){
        int convertedNumber = jsonTrackData[_param][i].asInt();
        //ofLogNotice() << convertedNumber;
        returnedVector.push_back(convertedNumber);
    }
    
    return returnedVector;
}

//--------------------------------------------------------------
void vboMeshObj::reportParams(int _i){
    cout << "reported params-" << ofToString(_i) << endl;
    cout << "g_copies-" << params.g_copies << endl;
    cout << "g_scale-" << params.g_scale << endl;
    cout << "g_rot-" << params.g_rot << endl;
    cout << "g_rotate-" << params.g_rotate << endl;
    cout << "g_trans-" << params.g_trans << endl;
    
    cout << "l_copies-" << params.l_copies << endl;
    cout << "l_scale-" << params.l_scale << endl;
    cout << "l_rot-" << params.l_rot << endl;
    cout << "l_rotate-" << params.l_rotate << endl;
    cout << "l_trans:-" << params.l_trans << endl;
}


//--------------------------------------------------------------
vector<ofVboMesh> vboMeshObj::passObjTwoVboMesh(vector<ofFile> _files){
    //This function does the acutual loading to video card.
    
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
        
        glRotatef(params.g_rotate.x,1,0,0);
        glRotatef(params.g_rotate.y,0,1,0);
        glRotatef(params.g_rotate.z,0,0,1);
        
        for(int i=0;i<params.l_copies;i++){
            
            glPushMatrix();
            
                glRotatef(i*params.l_rotate.x,1,0,0);
                glRotatef(i*params.l_rotate.y,0,1,0);
                glRotatef(i*params.l_rotate.z,0,0,1);
            
                glTranslatef(params.l_trans.x, params.l_trans.y, params.l_trans.z);
                
                glScalef(params.l_scale,params.l_scale,params.l_scale);
                
                shader.begin();
                    shader.setUniformTexture("tMatCap", matCap, 1);
                    vboMesh1[frame].draw();
                shader.end();
            
            glPopMatrix();
            
        }
        
        if(params.mirrored){
            for(int i=params.l_copies;i>0;i--){
                glPushMatrix();
                    glRotatef(i*params.l_rotate.x,1,0,0);
                    glRotatef(i*params.l_rotate.y,0,1,0);
                    glRotatef(i*params.l_rotate.z,0,0,1);
                    
                    
                    glTranslatef(params.l_trans.x, params.l_trans.y, params.l_trans.z+params.mirror_distance);
                    
                    glScalef(params.l_scale,params.l_scale,-params.l_scale);
                    
                    shader.begin();
                    shader.setUniformTexture("tMatCap", matCap, 1);
                    vboMesh1[frame].draw();
                    shader.end();
                
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    
}

//--------------------------------------------------------------
void vboMeshObj::update(){
    
    
    if(params.isPlaying){
        if(params.oscControlled){
            switch (params.tweenType){
                case 1:
                    frame = tweenback.update();
                    if(tweenback.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 2:
                    frame = tweenbounce.update();
                    if(tweenbounce.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 3:
                    frame = tweencirc.update();
                    if(tweencirc.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 4:
                    frame = tweencubic.update();
                    if(tweencubic.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 5:
                    frame = tweenelastic.update();
                    if(tweenelastic.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 6:
                    frame = tweenexpo.update();
                    if(tweenexpo.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 7:
                    frame = tweenquad.update();
                    if(tweenquad.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 8:
                    frame = tweenquart.update();
                    if(tweenquart.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 9:
                    frame = tweenquint.update();
                    if(tweenquint.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 10:
                    frame = tweensine.update();
                    if(tweensine.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                case 11:
                    frame = tweenlinear.update();
                    if(tweenlinear.isCompleted()){
                        params.isPlaying = false;
                    }
                    break;
                }
                //cout << "frame: " << frame << endl;
            

            
        } else {
            
            //TODO-DO TWEEN CODE HERE TO ON KEY PRESS.
            //NON OSC CONTROLL
//            if(frame < numFiles-1){
//                //timeline++;
//                frame = round(timeline*frame_increment);
//                cout << "timeline|frame:" << timeline << "|" << frame << endl;
//
//            } else {
//                frame = 0;
//                timeline = 0;
//                params.isPlaying = false;
//            }
            
        }// oscControlled
    }// is Playing
   
    
    
    if(params.randomized){
        //randomized positions.
        params.l_trans = ofVec3f(positiontweenbounce_x.update(),positiontweenbounce_y.update(),params.l_trans.z);

        if(positiontweenbounce_x.isCompleted()){
            params.randomized = false;
        }

    }//end params.randomized
    
    
    //sets the button a color when a osc message is recieved.
    setIndicator();
}

//--------------------------------------------------------------
void vboMeshObj::OSCLaunch(int _destinationFrame, int _durration, int _segmentLength, int _tweenType){
    
    
    
    params.tweenType = _tweenType;
    
    //ofxTween -- Figure out where you are going.
    unsigned delay = 0;
    unsigned duration = _durration;
    unsigned start = _destinationFrame - _segmentLength;
    unsigned end = _destinationFrame;
    
    //TWEEN
    switch(params.tweenType){
        case 1:
            tweenback.setParameters(1,easingback, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 2:
            tweenbounce.setParameters(2,easingbounce, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 3:
            tweencirc.setParameters(3,easingcirc, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 4:
            tweencubic.setParameters(4,easingcubic, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 5:
            tweenelastic.setParameters(5,easingelastic, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 6:
            tweenexpo.setParameters(6,easingexpo, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 7:
            tweenquad.setParameters(7,easingquad, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 8:
            tweenquart.setParameters(8,easingquart, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 9:
            tweenquint.setParameters(9,easingquint, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 10:
            tweensine.setParameters(10,easingsine, ofxTween::easeOut,start,end,duration,delay);
            break;
        case 11:
            tweenlinear.setParameters(11,easinglinear, ofxTween::easeOut,start,end,duration,delay);
            break;
        default:
            tweenlinear.setParameters(11,easinglinear, ofxTween::easeOut,start,end,duration,delay);
            break;
    }
    

    cout << "============================" << endl;
    cout << "OSCMessage>frame:" << frame <<
    ", durration:" << _durration <<
    ", segmentEnd:" << _destinationFrame <<
    ", params.tweenType:" << params.tweenType <<
    endl;

    //start the animation
    params.isPlaying = true;
    
    
    //moved to ofApp;
    //params.randomized = true;
    //randLocalPosition();
    
//    float randStart = ofRandom(-15, 0);
//    float randEnd = ofRandom(0, 15);
//    unsigned durr = 500;
//    unsigned del = 500;
//    
//    cout << randStart << ":" << randEnd << endl;
//    
//    positiontweenbounce_x.setParameters(12,easingsine, ofxTween::easeOut,randStart,randEnd,durr,del);
//    
//    randStart = ofRandom(-15, 0);
//    randEnd = ofRandom(0, 15);
//    
//    cout << randStart << ":" << randEnd << endl;
//    
//    positiontweenbounce_y.setParameters(13,easingsine, ofxTween::easeOut,randStart,randEnd,durr,del);
    
    
}

//--------------------------------------------------------------
void vboMeshObj::KeyboardLaunch(int _key, int _durration, int _segmentLength, int _tweenType){
    
    //play the segment
    OSCLaunch(params.cuePoints[params.currentSegment], params.durrationPoints[params.currentSegment], _segmentLength, _tweenType);
    
    if(params.currentSegment < params.cuePoints.size()-1){
        params.currentSegment++;
    } else {
        params.currentSegment = 0;
    }
    
}



//--------------------------------------------------------------
void vboMeshObj::setupGui(int _index){
    
    string index = ofToString(_index);
    
    // TAKE 2
    gui = new ofxUICanvas();
    
    gui->setName("TRACK" + index);
    gui->addLabel("TRACK" + index);
    
    gui->addToggle("SOLO", &params.solo);
    
    gui->addToggle("OSC", &params.oscControlled);
    gui->addToggle("MIRROR", &params.mirrored);
    
    //=====================
    gui->addLabel("GLOBAL");
    gui->addIntSlider("(G)COPIES " + index, 1, 12, &params.g_copies);
    
    gui->addSlider("(G)SCALE", 1.0, 50.0, &params.g_scale);
    
    gui->addSlider("(G)ROTATE-X",-180.0,180.0, &params.g_rotate.x);
    setGuiSnapUnits("(G)ROTATE-X",10.0);
    gui->addSlider("(G)ROTATE-Y",-180.0,180.0, &params.g_rotate.y);
    setGuiSnapUnits("(G)ROTATE-Y",10.0);
    gui->addSlider("(G)ROTATE-Z",-180.0,180.0, &params.g_rotate.z);
    setGuiSnapUnits("(G)ROTATE-Z",10.0);
    gui->addSlider("(G)TRANS-X", -50.0, 50.0, &params.g_trans.x);
    setGuiSnapUnits("(G)TRANS-X",5.0);
    gui->addSlider("(G)TRANS-Y", -50.0, 50.0, &params.g_trans.y);
    setGuiSnapUnits("(G)TRANS-Y",5.0);
    gui->addSlider("(G)TRANS-Z", -50.0, 50.0, &params.g_trans.z);
    setGuiSnapUnits("(G)TRANS-Z",5.0);
    
    //====================
    gui->addLabel("LOCAL");
    gui->addIntSlider("(L)COPIES " + index, 1, 12, &params.l_copies);

    
    gui->addSlider("(L)SCALE", 0.1, 1.0, &params.l_scale);
    gui->addSlider("(L)ROTATE-X",-180.0,180.0,&params.l_rotate.x);
    setGuiSnapUnits("(L)ROTATE-X",5.0);
    gui->addSlider("(L)ROTATE-Y",-180.0,180.0,&params.l_rotate.y);
    setGuiSnapUnits("(L)ROTATE-Y",5.0);
    gui->addSlider("(L)ROTATE-Z",-180.0,180.0,&params.l_rotate.z);
    setGuiSnapUnits("(L)ROTATE-Z",5.0);
    
    gui->addSlider("(L)TRANS-X", -100.0, 100.0, &params.l_trans.x);
    setGuiSnapUnits("(L)TRANS-X",5.0);
    gui->addSlider("(L)TRANS-Y", -100.0, 100.0, &params.l_trans.y);
    setGuiSnapUnits("(L)TRANS-Y",5.0);
    gui->addSlider("(L)TRANS-Z", -100.0, 100.0, &params.l_trans.z);
    setGuiSnapUnits("(L)TRANS-Z",5.0);

    gui->addSlider("MIRROR DISTANCE",-40.0,40.0, &params.mirror_distance);
    gui->addSlider("durration in ms",100.0,2000.0, &params.durration_in_ms);
    setGuiSnapUnits("durration in ms",100.0);
    

    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&vboMeshObj::guiEvent);
    
    
    
    //add this tracks gui to the tabbar
    ((ofApp*)ofGetAppPtr())->guiTabBar->addCanvas(gui);
    ((ofApp*)ofGetAppPtr())->guis.push_back(gui);
    
    
    //set increment and sticky(move to function)
    
//    ofxUISlider *sliderGX = (ofxUISlider *)gui->getWidget("(G)ROTATE-X");
//    sliderGX->setStickyValue(10.0);
//    sliderGX->setIncrement(10.0);
//
//    ofxUISlider *sliderGY = (ofxUISlider *)gui->getWidget("(G)ROTATE-Y");
//    sliderGY->setStickyValue(10.0);
//    sliderGY->setIncrement(10.0);
//    
//    ofxUISlider *sliderGZ = (ofxUISlider *)gui->getWidget("(G)ROTATE-Z");
//    sliderGZ->setStickyValue(10.0);
//    sliderGZ->setIncrement(10.0);
}

//--------------------------------------------------------------
void vboMeshObj::setGuiSnapUnits(string _name, float _unit){
    
    ofxUISlider *slider = (ofxUISlider *)gui->getWidget(_name);
    slider->setStickyValue(_unit);
    slider->setIncrement(_unit);
    
}
//--------------------------------------------------------------
void vboMeshObj::setIndicator(){
    
    if(params.isPlaying){
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::blue);
    } else {
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::black);

    }
}

//--------------------------------------------------------------
void vboMeshObj::randLocalPosition(float _start, float _end, int _durration, int _delay){
    
    //store the last position
    lastPosition = params.l_trans;
    
    float randX = ofRandom(_start, _end);
    float randY = ofRandom(_start, _end);
    //unsigned durration = 1000;
    //unsigned delay = 500;
    
    
    positiontweenbounce_x.setParameters(12,easingback, ofxTween::easeOut,lastPosition.x,randX,_durration,_delay);
    positiontweenbounce_y.setParameters(13,easingback, ofxTween::easeOut,lastPosition.y,randY,_durration,_delay);
    
    //start the animation
    params.randomized = true;
    
}

//--------------------------------------------------------------
void vboMeshObj::clear(){
    //set the frame to 0
    frame = 0;
    params.isPlaying = false;
    
}


//--------------------------------------------------------------
void vboMeshObj::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    
    //int sliderVal = e.widget->
    //cout << "vboMeshObj::" << canvasParent << "-" << kind << "-" << name << "-" << "END" << endl;
    
//    if (name == "(G)ROTATE-Z") {
//        ofxUISlider *slider = (ofxUISlider *) e.widget;
//        //cout << "Global Rotate Z" << endl;
//        slider->setIncrement(10.0);//increment is for key commands
//        slider->setStickyValue(10.0);
//        cout << slider->getIncrement() << endl;
//    }
    

    
}

//--------------------------------------------------------------
void vboMeshObj::keyPressed(int key)
{
    if(!params.oscControlled){
        //cout << "vboMeshObj::keyPressed" << ofToString(index) << "-KEY:" << ofToString(key) << endl;
        if(!params.isPlaying){
            ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::red);
            params.isPlaying = true;
            
            
            
        }
    }
}

//--------------------------------------------------------------
void vboMeshObj::keyReleased(int key)
{
    if(!params.oscControlled){
        //cout << ofToString(index) << " released from withing vboMeshObj" << endl;
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::black);

    }

    

}

//--------------------------------------------------------------
void vboMeshObj::exit()
{

    
    
}
