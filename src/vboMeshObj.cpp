#include "vboMeshObj.h"
#include "ofApp.h"

//--------------------------------------------------------------
vboMeshObj::vboMeshObj() {

    //setup default params
    params.isSelected = false;
    params.isLoaded = false;
    params.isPlaying = false;
    params.instancePlayingId = -1;// -1 = all
    params.lastInstancePlayed = -1;
    params.still = false;
    params.oscControlled = true;
    params.randomized = false;
    params.mirrored = true;
    params.tweenType = 11;
    
    params.mirror_distance = 10.0;
    //params.currentSegment = 0;//moved to instances
    params.stillFrame = 0;
    params.totalFrames = 0;
    
    params.spin = ofVec3f(0.0,0.0,0.0);
    params.spinRange = ofVec3f(0.0,0.0,0.0);
    params.spinX = false;
    params.spinY = false;
    params.spinZ = false;
    params.ltransMod = 0;

    //global
    params.g_copies = 1;
    params.gScale = false;
    params.g_scale = 10.0;
    params.gScaleMod = 0.0;
    params.gScaleModVal = 0.0;
    params.gTransX = false;
    params.gTransY = false;
    params.gTransZ = false;
    params.g_trans = ofVec3f(0.0,0.0,0.0);
    params.gTransMod = ofVec3f(0.0,0.0,0.0);
    params.gTransModVal = ofVec3f(0.0,0.0,0.0);
    params.gRotX = false;
    params.gRotY = false;
    params.gRotY = false;
    params.g_rotate = ofVec3f(0.0,0.0,0.0);
    params.gRotateMod = ofVec3f(0.0,0.0,0.0);
    params.gRotateModVal = ofVec3f(0.0,0.0,0.0);
    
    //local
    params.l_copies = 4;
    params.lScale = false;
    params.l_scale = 1.0;
    params.lScaleMod = 0.0;
    params.lScaleModVal = 0.0;
    params.lTransX = false;
    params.lTransY = false;
    params.lTransZ = false;
    params.l_trans = ofVec3f(0.0,0.0,0.0);
    params.lTransMod = ofVec3f(0.0,0.0,0.0);
    params.lTransModVal = ofVec3f(0.0,0.0,0.0);
    params.lRotX = false;
    params.lRotY = false;
    params.lRotY = false;
    params.l_rotate = ofVec3f(0.0,0.0,-90.0);
    params.lRotateMod = ofVec3f(0.0,0.0,0.0);
    params.lRotateModVal = ofVec3f(0.0,0.0,0.0);
    
    //object
    params.oRotX = false;
    params.oRotY = false;
    params.oRotZ = false;
    params.o_rotate = ofVec3f(0.0,0.0,0.0);
    params.oRotateMod = ofVec3f(0.0,0.0,0.0);
    params.oRotateModVal = ofVec3f(0.0,0.0,0.0);
    
}

//--------------------------------------------------------------
void vboMeshObj::setup(const objFileLoader::extObjFile &_input){
    
    instances.reserve(50);//TIP: http://arturocastro.net/blog/2011/10/28/stl::vector/
    for(int t=0; t<50;t++){
        instance copy;
        instances.push_back(copy);
        instances[t].playAll = false;
        instances[t].isPlaying = false;
        instances[t].currentSegment = -1;//or 0
        instances[t].noteID = 0;
        instances[t].note = 0;
        instances[t].vel = 0;
        instances[t].delta = 0;
        instances[t].frame = 0;
        instances[t].startFrame = 0;
        instances[t].midFrame = 0;
        instances[t].endFrame = 0;
        instances[t].direction = 1;
        instances[t].duration = 0;
        instances[t].clockedDurration = 700;
    }
    
    linearTweens.reserve(12);
    for(int i=0; i<12;i++){
        ofxTween tween;
        linearTweens.push_back(tween);
        
    }
    
    //pull in the objFileLoader data
    trackData = _input;
    
    //separate out the JSON data.
    jsonTrackData = _input.jsonData;
    
    index = jsonTrackData["index"].asInt();
    
    
    parameters.setName("TRACK "+ofToString(index));
    parameters.add(gui_test.set("TEST",1,1,10));
    
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
    
    //frame = 0;//duplicate in constructor
    
    setShader(jsonTrackData["matCap-shader"].asString());
    
    //setup more params
    params.cuePoints = parseJSON("objSeq-cues");
    params.durrationPoints = parseJSON("objSeq-durations");
    params.midpointCues = parseJSON("objSeq-midpoint-cues");
    params.segmentLengths = parseJSON("objSeq-segmentLengths");
    
    params.stillFrame = jsonTrackData["objSeq-still"].asInt();
    params.totalFrames = jsonTrackData["objSeq-files"].asInt();
    
    //get all the matcaps from ofApp
    matcaps = ((ofApp*)ofGetAppPtr())->appFileLoader.externalMatCapFiles;
    
    setMatCap(0);
    
    //setup the ofxUI GUI
    setupGui(index);
    
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
    
    ofLogVerbose("ofxUI") << "track " << _i <<
    "g_copies(" << params.g_copies << ")," <<
    "g_scale(" << params.g_scale  << ")," <<
    "g_rotate(" << params.g_rotate << ")," <<
    "g_trans(" << params.g_trans << ")," <<
    "l_copies(" << params.l_copies << ")," <<
    "l_scale(" << params.l_scale << ")," <<
    "l_rotate(" << params.l_rotate << ")," <<
    "l_trans(" << params.l_trans << ")," <<
    "totalFrames(" << params.totalFrames << ")";
    
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
        ofLogVerbose("objloader") << (*it).getFileName();
        
        //here is where I can set a progress bar
        
    }
    
    return tempVboMeshSequence;
    
}


//--------------------------------------------------------------
void vboMeshObj::loadVboMesh(const objFileLoader::extObjFile &_input){
    
    vboMesh1.clear();
    vboMesh1 = passObjTwoVboMesh(_input.objs);
    
    //TODO -- add timer event, add progress bar
    ofLogNotice("objloader") << "track " << index << " loaded!";
}


//--------------------------------------------------------------
void vboMeshObj::setMatCap(int _imgIndex){
    string matCapFile = "matCap/"+ofToString(matcaps[_imgIndex]);
    matCap.loadImage(matCapFile);
}

//--------------------------------------------------------------
void vboMeshObj::setShader(string _shader){
    shader.load("shaders/"+_shader);
}

//--------------------------------------------------------------
void vboMeshObj::draw(){
    
    if(params.isLoaded){
        
        //accumulate transform stacks.
        for(int j=1;j<params.g_copies+1;j++){
            glPushMatrix();
            //global scale
            glScalef(params.g_scale+params.gScaleModVal, params.g_scale+params.gScaleModVal, params.g_scale+params.gScaleModVal);//scale of this layer
            //global trans
            glTranslatef(j*params.g_trans.x+params.gTransModVal.x,j*params.g_trans.y+params.gTransModVal.y,j*params.g_trans.z+params.gTransModVal.z);
            //global rot
            
            if(params.spinX){
                glRotatef(sin((ofGetFrameNum()*0.2)*params.spin.x)*params.spinRange.x,1,0,0);
            } else {
                glRotatef(params.g_rotate.x,1,0,0);
            }
            if(params.spinY){
                glRotatef(sin((ofGetFrameNum()*0.2)*params.spin.y)*params.spinRange.y,0,1,0);
            } else {
                glRotatef(params.g_rotate.y,0,1,0);
            }
            if(params.spinZ){
                glRotatef(sin((ofGetFrameNum()*0.2)*params.spin.z)*params.spinRange.z,0,0,1);
            } else {
                glRotatef(params.g_rotate.z,0,0,1);
            }

            //glRotatef(params.g_rotate.z,0,0,1);
            
            for(int i=0;i<params.l_copies;i++){
                
                glPushMatrix();
                glRotatef(i*params.l_rotate.x+params.lRotateModVal.x,1,0,0);
                glRotatef(i*params.l_rotate.y+params.lRotateModVal.y,0,1,0);
                glRotatef(i*params.l_rotate.z+params.lRotateModVal.z,0,0,1);

                glTranslatef(params.l_trans.x+params.lTransModVal.x, params.l_trans.y+params.lTransModVal.y, params.l_trans.z+params.lTransModVal.z);
            
                glScalef(params.l_scale+params.lScaleModVal,params.l_scale+params.lScaleModVal,params.l_scale+params.lScaleModVal);
                
                shader.begin();
                shader.setUniformTexture("tMatCap", matCap, 1);
                
                    glPushMatrix();
                        glRotatef(params.o_rotate.x+params.oRotateModVal.x,1,0,0);
                        glRotatef(params.o_rotate.y+params.oRotateModVal.y,0,1,0);
                        glRotatef(params.o_rotate.z+params.oRotateModVal.z,0,0,1);
                        if(params.still){
                            vboMesh1[params.stillFrame].draw();
                        } else {
                            //Which timeline do I draw?
                            //timeline 1
                            vboMesh1[instances[i].frame].draw();
                            
                            //timeline 2
                            //vboMesh1[instance[1].frame].draw();
                            
                            //timeline 3
                            //vboMesh1[instance[2].frame].draw();
                            
                            //.....
                        }
                    glPopMatrix();
                
                shader.end();
                
                glPopMatrix();
                
            }
            
            if(params.mirrored){
                for(int i=params.l_copies;i>0;i--){
                    glPushMatrix();
                    glRotatef(i*params.l_rotate.x+params.lRotateModVal.x,1,0,0);
                    glRotatef(i*params.l_rotate.y+params.lRotateModVal.y,0,1,0);
                    glRotatef(i*params.l_rotate.z+params.lRotateModVal.z,0,0,1);
                    
                    glTranslatef(params.l_trans.x+params.lTransModVal.x, params.l_trans.y+params.lTransModVal.y, params.l_trans.z+params.lTransModVal.z+params.mirror_distance);
                    
                    glScalef(params.l_scale+params.lScaleModVal,params.l_scale+params.lScaleModVal,-(params.l_scale+params.lScaleModVal));
                    
                    shader.begin();
                    shader.setUniformTexture("tMatCap", matCap, 1);
                    
                        glPushMatrix();
                        glRotatef(params.o_rotate.x+params.oRotateModVal.x,1,0,0);
                        glRotatef(params.o_rotate.y+params.oRotateModVal.y,0,1,0);
                        glRotatef(params.o_rotate.z+params.oRotateModVal.z,0,0,1);
                        if(params.still){
                            vboMesh1[params.stillFrame].draw();
                        } else {
                            vboMesh1[instances[i].frame].draw();
                        }
                        glPopMatrix();
                    
                    shader.end();
                    
                    glPopMatrix();
                }
            }
            glPopMatrix();
        }
        
        
        
    }
    
}

//--------------------------------------------------------------
void vboMeshObj::update(){
    //OLD L1 encapsulation code.
    //    if(params.isPlaying){
    //
    //    }// is Playing
    
    for(int i=0; i<params.l_copies;i++){
        if(instances[i].isPlaying){
            if(params.oscControlled){
                switch (params.tweenType){
                    case 11:
                        //update the frame on the instance
                        instances[i].frame = linearTweens[i].update();
                        
                        if(linearTweens[i].isCompleted()){
                            instances[i].isPlaying = false;
                            
                            //if no noteOff message is expected.
                            if(!instances[i].playNoteOff){
                                //resets everthing. Move to noteOff???
                                instances[i].noteID = 0;
                                instances[i].note = 0;
                                instances[i].vel = 0;
                                instances[i].delta = 0;
                                instances[i].direction = 1;
                            }
                            //instances[i].clockedDurration = 0;
                            
                            //params.isPlaying = false;//not needed
                        }
                        break;
                }
                
            } else {
                //non osc controlled block
                
            }// oscControlled
        }//instances[i].isPlaying
    }//params.l_copies
    
    
    
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

////--------------------------------------------------------------
//void vboMeshObj::OSCLaunch(int _destinationFrame, int _durration, int _tweenType, int _instanceId){
//    
//    params.instancePlayingId = _instanceId;//keep track but not needed. Instances are logged in intances[].currentPlaying
//    params.tweenType = _tweenType;
//    
//    //ofxTween -- Figure out where you are going.
//    unsigned delay = 0;
//    unsigned duration = _durration;
//    unsigned start = _destinationFrame - params.segmentLengths[instances[_instanceId].currentSegment];
//    unsigned end = _destinationFrame;
//    
//    //TWEEN
//    switch(params.tweenType){
//        case 1:
//            tweenback.setParameters(1,easingback, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 2:
//            tweenbounce.setParameters(2,easingbounce, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 3:
//            tweencirc.setParameters(3,easingcirc, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 4:
//            tweencubic.setParameters(4,easingcubic, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 5:
//            tweenelastic.setParameters(5,easingelastic, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 6:
//            tweenexpo.setParameters(6,easingexpo, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 7:
//            tweenquad.setParameters(7,easingquad, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 8:
//            tweenquart.setParameters(8,easingquart, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 9:
//            tweenquint.setParameters(9,easingquint, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 10:
//            tweensine.setParameters(10,easingsine, ofxTween::easeOut,start,end,duration,delay);
//            break;
//        case 11:
//            tweenlinear.setParameters(11,easinglinear, ofxTween::easeOut,start,end,duration,delay);//not needed
//
//            //TURN ON THE ANIMATION
//            for(int i=0;i<params.l_copies;i++){
//                
//                if(instances[i].playAll){
//                    //setup the
//                    linearTweens[i].setParameters(11+i,easinglinear, ofxTween::easeOut,start,end,duration,delay);
//                    instances[i].isPlaying = true;//play all instances at once.
//                } else {
//                    if(instances[i].noteID > 0){
//                        linearTweens[i].setParameters(11+i,easinglinear, ofxTween::easeOut,start,end,duration,delay);
//                        instances[i].isPlaying = true; //only play the ones that have a noteID set.
//                    }
//                }
//                
//            }
//            break;
//        default:
//            tweenlinear.setParameters(11,easinglinear, ofxTween::easeOut,start,end,duration,delay);
//
//            if(params.instancePlayingId >= 0){
//                linearTweens[params.instancePlayingId].setParameters(11,easinglinear, ofxTween::easeOut,start,end,duration,delay);
//            } else {
//                for(int i=0;i<12;i++){
//                    linearTweens[i].setParameters(11+i,easinglinear, ofxTween::easeOut,start,end,duration,delay);
//                }
//            }
//
//            break;
//    }
//    
//
//    cout << "============================" << endl;
//    cout << "OSCMessage>frame:" << frame <<
//    ", _segmentLength:" << jsonTrackData["objSeq-segLen"] <<
//    ", start:" << start <<
//    ", end:" << end <<
//    ", durration:" << _durration <<
//    ", delay:" << delay <<
//    ", segmentEnd:" << _destinationFrame <<
//    ", params.tweenType:" << params.tweenType <<
//    ", _instanceId:" << _instanceId <<
//    endl;
//    
//    params.isPlaying = true;// NOT USED ANYMORE.  MOVE TO STRUCT INSTANCE
//    
//    //TURN ON THE ANIMATION
//    for(int i=0;i<params.l_copies;i++){
//        
//        if(instances[i].playAll){
//            instances[i].isPlaying = true;//play all instances at once.
//        } else {
//            if(instances[i].noteID > 0){
//                instances[i].isPlaying = true; //only play the ones that have a noteID set.
//            }
//        }
//    
//    }
//
//    
//}

//--------------------------------------------------------------
void vboMeshObj::KeyboardLaunch(int _tweenType, int _instanceId){
    
    //OLD
    //OSCLaunch(params.cuePoints[instances[_instanceId].currentSegment], params.durrationPoints[instances[_instanceId].currentSegment], _tweenType, _instanceId);
    
    play(_instanceId, params.durrationPoints[instances[_instanceId].currentSegment], _tweenType);
    
    //increments the buffer instance to play clockwise.
    advanceSegment(_instanceId);
    
    
}

//--------------------------------------------------------------
void vboMeshObj::setupGui(int _index){
    
    string index = ofToString(_index);
    
    // TAKE 2
    gui = new ofxUICanvas();
    
    gui->setWidth(150);
    
    //set the background color and text(syphon)
    gui->setColorFill(ofColor::darkGrey);
    gui->setColorBack(ofColor(128,100));
    
    gui->setWidgetSpacing(2);
    
    gui->setName("TRACK" + index);
    
    gui->addLabel(jsonTrackData["objSeq-basefilename"].asString());
   
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSpacer(100, 5);
    gui->addDropDownList("MATCAP", matcaps, 250, 0,0);
    
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
   
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addToggle("LOADED", &params.isLoaded);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    gui->addToggle("STILL", &params.still);
    
    gui->addToggle("OSC", &params.oscControlled);
    gui->addToggle("MIRROR", &params.mirrored);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addIntSlider("FRAME", 1, params.totalFrames, &params.stillFrame, 300,8,0,0);
    gui->addSlider("MIRROR",-40.0,40.0, &params.mirror_distance,150,8,0,0);
    
    gui->addSpacer(320, 1);
    
    //copies
    gui->addIntSlider("(G)COPIES " + index, 1, 12, &params.g_copies,150,8,0,0);
    
    //scale
    gui->addSlider("(G)SCALE", 1.0, 50.0, &params.g_scale,150,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-scale-bs", "GUI/toggle.png", &params.gScale, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gScaleMod",0.0, 2.0, &params.gScaleMod,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    //global Rotate
    gui->addSlider("(G)ROTATE-X",-180.0,180.0, &params.g_rotate.x,150,8,0,0);
    setGuiSnapUnits("(G)ROTATE-X",10.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-rot-x", "GUI/toggle.png", &params.gRotX, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gRotXMod",0.0, 2.0, &params.gRotateMod.x,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(G)ROTATE-Y",-180.0,180.0, &params.g_rotate.y,150,8,0,0);
    setGuiSnapUnits("(G)ROTATE-Y",10.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-rot-y", "GUI/toggle.png", &params.gRotY, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gRotYMod",0.0, 2.0, &params.gRotateMod.y,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(G)ROTATE-Z",-180.0,180.0, &params.g_rotate.z,150,8,0,0);
    setGuiSnapUnits("(G)ROTATE-Z",10.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-rot-z", "GUI/toggle.png", &params.gRotZ, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gRotZMod",0.0, 2.0, &params.gRotateMod.z,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    //global translate
    gui->addSlider("(G)TRANS-X", -50.0, 50.0, &params.g_trans.x,150,8,0,0);
    setGuiSnapUnits("(G)TRANS-X",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-trans-x", "GUI/toggle.png", &params.gTransX, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gTransXMod",0.0, 2.0, &params.gTransMod.x,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(G)TRANS-Y", -50.0, 50.0, &params.g_trans.y,150,8,0,0);
    setGuiSnapUnits("(G)TRANS-Y",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-trans-y", "GUI/toggle.png", &params.gTransY, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gTransYMod",0.0, 2.0, &params.gTransMod.y,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    
    gui->addSlider("(G)TRANS-Z", -50.0, 50.0, &params.g_trans.z,150,8,0,0);
    setGuiSnapUnits("(G)TRANS-Z",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("g-trans-z", "GUI/toggle.png", &params.gTransZ, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("gTransZMod",0.0, 2.0, &params.gTransMod.z,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer(320, 1);
    
    
    //LOCAL
    gui->addIntSlider("(L)COPIES " + index, 1, 12, &params.l_copies,150,8,150,0);

    
    gui->addSlider("(L)SCALE", 0.1, 1.0, &params.l_scale,150,8,150,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-scale-bs", "GUI/toggle.png", &params.lScale, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lScaleMod",0.0, 2.0, &params.lScaleMod,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)ROTATE-X",-180.0,180.0,&params.l_rotate.x,150,8,0,0);
    setGuiSnapUnits("(L)ROTATE-X",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-rot-x", "GUI/toggle.png", &params.lRotX, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lRotXMod",0.0, 2.0, &params.lRotateMod.x,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)ROTATE-Y",-180.0,180.0,&params.l_rotate.y,150,8,0,0);
    setGuiSnapUnits("(L)ROTATE-Y",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-rot-y", "GUI/toggle.png", &params.lRotY, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lRotYMod",0.0, 2.0, &params.lRotateMod.y,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)ROTATE-Z",-180.0,180.0,&params.l_rotate.z,150,8,0,0);
    setGuiSnapUnits("(L)ROTATE-Z",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-rot-z", "GUI/toggle.png", &params.lRotZ, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lRotZMod",0.0, 2.0, &params.lRotateMod.z,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    
    gui->addSlider("(L)TRANS-X", -100.0, 100.0, &params.l_trans.x,150,8,0,0);
    setGuiSnapUnits("(L)TRANS-X",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-trans-x", "GUI/toggle.png", &params.lTransX, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lTransXMod",0.0, 2.0, &params.lTransMod.x,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)TRANS-Y", -100.0, 100.0, &params.l_trans.y,150,8,0,0);
    setGuiSnapUnits("(L)TRANS-Y",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-trans-y", "GUI/toggle.png", &params.lTransY, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lTransYMod",0.0, 2.0, &params.lTransMod.y,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)TRANS-Z", -100.0, 100.0, &params.l_trans.z,150,8,0,0);
    setGuiSnapUnits("(L)TRANS-Z",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("l-trans-z", "GUI/toggle.png", &params.lTransZ, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("lTransZMod",0.0, 2.0, &params.lTransMod.z,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer(320, 1);
    //====================
    //object space
    gui->addSlider("(O)ROTATE-X",-180.0,180.0,&params.o_rotate.x,150,8,0,0);
    setGuiSnapUnits("(O)ROTATE-X",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("o-rot-x", "GUI/toggle.png", &params.oRotX, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("oRotXMod",0.0, 2.0, &params.oRotateMod.x,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(O)ROTATE-Y",-180.0,180.0,&params.o_rotate.y,150,8,0,0);
    setGuiSnapUnits("(O)ROTATE-Y",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("o-rot-y", "GUI/toggle.png", &params.oRotY, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("oRotYMod",0.0, 2.0, &params.oRotateMod.y,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(O)ROTATE-Z",-180.0,180.0,&params.o_rotate.z,150,8,0,0);
    setGuiSnapUnits("(O)ROTATE-Z",5.0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMultiImageToggle("o-rot-z", "GUI/toggle.png", &params.oRotZ, 20, 20,0,0,OFX_UI_FONT_SMALL);
    gui->addSlider("oRotZMod",0.0, 2.0, &params.oRotateMod.z,130,8,0,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    //====================
    //spin controls
    gui->addLabelToggle("spinX", &params.spinX,50,12,0,0,false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("Spin X", -1.0, 1.0, &params.spin.x,100,8,0,0);
    gui->addSlider("Spin Range X", 5,45,&params.spinRange.x,100,8,0,0);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addLabelToggle("spinY", &params.spinY,50,12,0,0,false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("Spin Y", -0.1, 0.1, &params.spin.y,100,8,0,0);
    gui->addSlider("Spin Range Y", 5,45,&params.spinRange.y,100,8,0,0);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addLabelToggle("spinZ", &params.spinZ,50,12,0,0,false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("Spin Z", -0.1, 0.1, &params.spin.z,100,8,0,0);
    gui->addSlider("Spin Range Z", 5,45,&params.spinRange.z,100,8,0,0);
    
    //FINALIZE THE GUI
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&vboMeshObj::guiEvent);
    
    //add this tracks gui to the tabbar
    ((ofApp*)ofGetAppPtr())->guiTabBar->addCanvas(gui);
    ((ofApp*)ofGetAppPtr())->guis.push_back(gui);
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

    params.isPlaying = false;
    
    for(int i=0; i<params.l_copies;i++){

        instances[i].playAll = false;
        instances[i].isPlaying = false;
        instances[i].currentSegment = -1;
        instances[i].noteID = 0;
        instances[i].note = 0;
        instances[i].vel = 0;
        instances[i].delta = 0;
        instances[i].frame = 0;
        instances[i].direction = 1;

    }
    
    params.instancePlayingId = 0;
    
}

//--------------------------------------------------------------
void vboMeshObj::bassControl(float &_amp, int _noteLength){
    
    
    //GLOBAL
    params.gScale ? params.gScaleModVal = _amp * params.gScaleMod : params.gScaleModVal = 0.0;
    
    params.gRotX ? params.gRotateModVal.x = _amp * params.gRotateMod.x : params.gRotateModVal.x = 0.0;
    params.gRotY ? params.gRotateModVal.y = _amp * params.gRotateMod.y : params.gRotateModVal.y = 0.0;
    params.gRotZ ? params.gRotateModVal.z = _amp * params.gRotateMod.z : params.gRotateModVal.z = 0.0;

    params.gTransX ? params.gTransModVal.x = _amp * params.gTransMod.x : params.gTransModVal.x = 0.0;
    params.gTransY ? params.gTransModVal.y = _amp * params.gTransMod.y : params.gTransModVal.y = 0.0;
    params.gTransZ ? params.gTransModVal.z = _amp * params.gTransMod.z : params.gTransModVal.z = 0.0;
    
    
    
    //LOCAL
    params.lScale ? params.lScaleModVal = _amp * params.lScaleMod : params.lScaleModVal = 0.0;
    
    params.lRotX ? params.lRotateModVal.x = _amp * params.lRotateMod.x : params.lRotateModVal.x = 0.0;
    params.lRotY ? params.lRotateModVal.y = _amp * params.lRotateMod.y : params.lRotateModVal.y = 0.0;
    params.lRotZ ? params.lRotateModVal.z = _amp * params.lRotateMod.z : params.lRotateModVal.z = 0.0;
    
    params.lTransX ? params.lTransModVal.x = _amp * params.lTransMod.x : params.lTransModVal.x = 0.0;
    params.lTransY ? params.lTransModVal.y = _amp * params.lTransMod.y : params.lTransModVal.y = 0.0;
    params.lTransZ ? params.lTransModVal.z = _amp * params.lTransMod.z : params.lTransModVal.z = 0.0;
    
    
    //OBJECT
    
    params.oRotX ? params.oRotateModVal.x = _amp * params.oRotateMod.x : params.oRotateModVal.x = 1.0;
    params.oRotY ? params.oRotateModVal.y = _amp * params.oRotateMod.y : params.oRotateModVal.y = 1.0;
    params.oRotZ ? params.oRotateModVal.z = _amp * params.oRotateMod.z : params.oRotateModVal.z = 1.0;
    
    
}


//--------------------------------------------------------------
void vboMeshObj::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    
    if (name == "LOADED") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofLogNotice("objloader") << "LOADING TRACK:" << index << " - button pressed:" << toggle->getValue();
        
        if(params.isLoaded){
            //load all the vboMeshes
            loadVboMesh(trackData);
        }
        if(!params.isLoaded){
            vboMesh1.clear();
        }
        
    } else if(name == "STILL"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if(toggle->getValue()){
            params.still = true;
        
        } else {
            params.still = false;
        }
    } else if(name == "MATCAP"){
        ofxUIDropDownList *dd = (ofxUIDropDownList *) e.widget;
        
        //dd->activateToggle("silver.jpg");
        
        
        if(!dd->getSelectedIndeces().empty()){
            ofLogNotice("matcap") << "setting the matcap";
            setMatCap(dd->getSelectedIndeces()[0]);
        }
        
        
        ofLogNotice("matcap") << ofToString(dd->getSelectedIndeces());

        
    }
    

    
}

//--------------------------------------------------------------
void vboMeshObj::keyPressed(int key)
{
    
    //increments the buffer in a clockwise fashion.
    if(params.instancePlayingId < params.l_copies-1){
        params.instancePlayingId++;
    } else {
        params.instancePlayingId = 0;
    }
    
    //revisit how the osc controller button is supposed to work.
    if(!params.oscControlled){
        if(!params.isPlaying){
            ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::red);
            params.isPlaying = true;//turn on the animation
        }
    }
}

//--------------------------------------------------------------
void vboMeshObj::keyReleased(int key)
{
    if(!params.oscControlled){
        ((ofApp*)ofGetAppPtr())->guiTabBar->getWidget("TRACK"+ofToString(index))->setColorBack(ofColor::black);
    }

}

//--------------------------------------------------------------
void vboMeshObj::advanceInstance(){
    if(params.instancePlayingId < params.l_copies-1){
        params.instancePlayingId++;
    } else {
        params.instancePlayingId = 0;
    }
    
}

//--------------------------------------------------------------
void vboMeshObj::advanceSegment(int _buffer){
    //What Segment or Cue we are on.
    
    if(instances[_buffer].currentSegment < params.cuePoints.size()-1){
        instances[_buffer].currentSegment++;
    } else {
        instances[_buffer].currentSegment = 0;
    }
}


//--------------------------------------------------------------
void vboMeshObj::noteOn(int _buffer, int _noteId, int _note, int _velocity, int _delta, bool _playNoteOff){

    
    //increment the cue/segment.
    advanceSegment(_buffer);
    
    
    //noteId comes from Max and is a unique identifier per instance.
    if(instances[_buffer].noteID == 0){
        ofLogNotice("OSC") << "noteOn-buffer:" << _buffer;
        //check if the specified buffer is empty
        //set the instance params.
        instances[_buffer].playNoteOff = _playNoteOff;
        instances[_buffer].midiState = 1;
        instances[_buffer].noteID = _noteId;
        instances[_buffer].note = _note;
        instances[_buffer].vel = _velocity;
        instances[_buffer].delta = _delta;
        
        //set the cuepoint data based on instances[].currentSegment
        instances[_buffer].startFrame = params.cuePoints[instances[_buffer].currentSegment] - params.segmentLengths[instances[_buffer].currentSegment];
        instances[_buffer].midFrame = params.midpointCues[instances[_buffer].currentSegment];
        instances[_buffer].endFrame = params.cuePoints[instances[_buffer].currentSegment];
        
    } else {
        //search for a random buffer to place a note
        int randBuffer = bwUtil::getUniqueRandomInt(1, params.l_copies, _buffer);
        ofLogNotice("OSC") << "noteOn-randBuffer:" << randBuffer;
        instances[randBuffer].noteID = _noteId;
        instances[randBuffer].note = _note;
        instances[randBuffer].vel = _velocity;
        instances[randBuffer].delta = _delta;

        
    }
}

//--------------------------------------------------------------
void vboMeshObj::play(int _buffer, int _duration, int _tweenType){
    params.tweenType = _tweenType;
    
    //ofxTween -- Figure out where you are going.
    unsigned delay = 0;
    unsigned duration = _duration;

    //RIGHT NOW instances[].currentSegment is how we track what cue we are on.
    //FUTURE Which cue is managged by MAX app.  Perhaps have a switch.
    //OLD -  unsigned start = params.cuePoints[instances[_buffer].currentSegment] - params.segmentLengths[instances[_buffer].currentSegment];
    //OLD -  unsigned end = params.cuePoints[instances[_buffer].currentSegment];
    
    //set the markers per instance(start,mid,end)
    unsigned start = 0;
    unsigned end = 0;
    
    if(instances[_buffer].playNoteOff){
        if(instances[_buffer].midiState == 1){
            start = instances[_buffer].startFrame;
            end = instances[_buffer].midFrame;
            
        } else {
            start = instances[_buffer].midFrame;
            end = instances[_buffer].endFrame;
            
        }
    } else {
        start = instances[_buffer].startFrame;
        end = instances[_buffer].endFrame;
    }
    
    //remember the duration
    instances[_buffer].duration = _duration;
    
    ofLogNotice("OSC") << "PLAYING: " << ofToString(_buffer) << ":[" << ofToString(start) << "-" << ofToString(end) << "]";
    
    //setup the Tween
    tweenPlayInstance(params.tweenType, start, end, duration, delay);
    
    //TURN ON THE ANIMATION  //==== I THINK I CAN TAKE THIS OUT.  SAME THING IN tweenPlayInstance()
//    for(int i=0;i<params.l_copies;i++){
//        
//        if(instances[i].playAll){
//            instances[i].isPlaying = true;//play all instances at once.
//        } else {
//            if(instances[i].noteID > 0){
//                instances[i].isPlaying = true; //only play the ones that have a noteID set.
//            }
//        }
//    }
}

//--------------------------------------------------------------
void vboMeshObj::noteOff(int _noteId, int _durration){
    ofLogNotice("OSC") << "vboMeshObj::noteOff -- REPORT:";
    
    for(int t=0; t<50;t++){
        //search through all the buffers and find the right id
        if(instances[t].noteID == _noteId){
            
            instances[t].isPlaying = false;
            instances[t].clockedDurration = _durration;
            
            
            instances[t].midiState = 0;//midiState = buffer is in noteOn or Off.
            if(instances[t].playNoteOff){
                
                //play only the note off.
                play(t,instances[t].duration+200,params.tweenType);
                
                //reset my instance data
                instances[t].noteID = 0;
                instances[t].note = 0;
                instances[t].vel = 0;
                instances[t].delta = 0;
                instances[t].direction = 1;
                
            }
            
            
            cout << "[" <<
            "instance:" << t <<
            ",isPlaying: " << instances[t].isPlaying <<
            ",noteID: " << instances[t].noteID <<
            ",note: " << instances[t].note <<
            ",vel: " << instances[t].vel <<
            ",delta: " << instances[t].delta <<
            ",frame: " << instances[t].frame <<
            ",direction: " << instances[t].direction <<
            ",clockedDurration: " << instances[t].clockedDurration << "]" <<
            endl;
            
            
            
        } else {
            cout << instances[t].noteID << ",";
        }
    }
    cout << endl;
    
}


//--------------------------------------------------------------
void vboMeshObj::tweenPlayInstance(int _tweenType, int _start, int _end, int _duration, int _delay){
    //RIGHT NOW ONLY WORKS ON LINEAR TWEEN MODE.
    switch(_tweenType){
        case 11:
            
            //TURN ON THE ANIMATION
            for(int i=0;i<params.l_copies;i++){
                
                if(instances[i].playAll){
                    //PLAY ALL INTANCES
                    linearTweens[i].setParameters(11+i,easinglinear, ofxTween::easeOut,_start,_end,_duration,_delay);
                    instances[i].isPlaying = true;//play all instances at once.
                } else {
                    if(instances[i].noteID > 0){
                        if(instances[i].isPlaying == false){
                            //PLAY JUST THE SELECTED INSTANCE
                            linearTweens[i].setParameters(11+i,easinglinear, ofxTween::easeOut,_start,_end,_duration,_delay);
                            instances[i].isPlaying = true; //only play the ones that have a noteID set.
                        }
                    }
                }
                
            }
            break;
        default:
            break;
    }
    
}



//--------------------------------------------------------------
void vboMeshObj::exit()
{

    
    
}
