#include "vboMeshObj.h"
#include "ofApp.h"

//--------------------------------------------------------------
vboMeshObj::vboMeshObj() {

    
//    //setup default params
//    params.playAll = false;
//    params.playAllFinalize = false;
//    params.isSelected = false;
//    params.isLoaded = false;
//    params.isPlaying = false;
//    params.instancePlayingId = -1;// -1 = all
//    params.lastInstancePlayed = -1;
//    params.still = false;
//    params.oscControlled = true;
//    params.randomized = false;
//    params.mirrored = false;
//    params.mirrorX = false;
//    params.mirrorY = false;
//    params.mirrorZ = false;
//    params.tweenType = 11;
//    params.playNoteOff = true;  //this is the noteOn/Off switch. On = play only on NoteOn.  Off = play noteOn THEN noteOff.
//    params.type = "";
//    params.numOfSeg = 1;
//    
//    
//    
//    params.mirror_distance = 10.0;
//    //params.currentSegment = 0;//moved to instances
//    params.stillFrame = 0;
//    params.totalFrames = 0;
//    params.testSpeed = 500;
//    
//    params.spin = ofVec3f(0.0,0.0,0.0);
//    params.spinRange = ofVec3f(0.0,0.0,0.0);
//    params.spinX = false;
//    params.spinY = false;
//    params.spinZ = false;
//    params.ltransMod = 0;
//
//    //global
//    params.g_copies = 1;
//    params.gScale = false;
//    params.g_scale = 10.0;
//    params.gScaleMod = 0.0;
//    params.gScaleModVal = 0.0;
//    params.gTransX = false;
//    params.gTransY = false;
//    params.gTransZ = false;
//    params.g_trans = ofVec3f(0.0,0.0,0.0);
//    params.gTransMod = ofVec3f(0.0,0.0,0.0);
//    params.gTransModVal = ofVec3f(0.0,0.0,0.0);
//    params.gRotX = false;
//    params.gRotY = false;
//    params.gRotY = false;
//    params.g_rotate = ofVec3f(0.0,0.0,0.0);
//    params.gRotateMod = ofVec3f(0.0,0.0,0.0);
//    params.gRotateModVal = ofVec3f(0.0,0.0,0.0);
//    
//    //local
//    params.l_copies = 12;
//    params.l_slices = 2;
//    params.lScale = false;
//    params.l_scale = 1.0;
//    params.lScaleMod = 0.0;
//    params.lScaleModVal = 0.0;
//    params.lTransX = false;
//    params.lTransY = false;
//    params.lTransZ = false;
//    params.l_trans = ofVec3f(0.0,0.0,0.0);
//    params.lTransMod = ofVec3f(0.0,0.0,0.0);
//    params.lTransModVal = ofVec3f(0.0,0.0,0.0);
//    params.lRotX = false;
//    params.lRotY = false;
//    params.lRotY = false;
//    params.l_rotate = ofVec3f(0.0,0.0,30.0);
//    params.lRotateMod = ofVec3f(0.0,0.0,0.0);
//    params.lRotateModVal = ofVec3f(0.0,0.0,0.0);
//    
//    //object
//    params.oRotX = false;
//    params.oRotY = false;
//    params.oRotZ = false;
//    params.o_rotate = ofVec3f(0.0,0.0,0.0);
//    params.oRotateMod = ofVec3f(0.0,0.0,0.0);
//    params.oRotateModVal = ofVec3f(0.0,0.0,0.0);
    
}


//--------------------------------------------------------------
void vboMeshObj::setup(const objFileLoader::extObjFile &_input){
    
    //setup the default params.
    trackParameters.setDefault(params);
    
    
    instances.reserve(50);//TIP: http://arturocastro.net/blog/2011/10/28/stl::vector/
    for(int t=0; t<50;t++){
        instance copy;
        instances.push_back(copy);
        instances[t].cued2play = false;
        instances[t].isPlaying = false;
        instances[t].isTweening = false;
        instances[t].playToEnd = false;//unused
        instances[t].playNoteOff = false;//unused
        instances[t].midiState = 0; // currently playing 1(noteOn), 0(noteOff)
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
    
    setShader(jsonTrackData["matCap-shader"].asString());
    
    
    //DEBUGGING PARAMS
    parameters.setName("TRACK "+ofToString(index));
    parameters.add(gui_buffers.set("buffers",params.l_copies));
    parameters.add(gui_cued2PlayList.set("cue","0,0,0,0,0,0,0,0,0,0,0,0"));
    parameters.add(gui_instanceList.set("noteID","0,0,0,0,0,0,0,0,0,0,0,0"));
    parameters.add(gui_isTweeningList.set("isTweening","0,0,0,0,0,0,0,0,0,0,0,0"));
    parameters.add(gui_isPlayingList.set("isPlaying","0,0,0,0,0,0,0,0,0,0,0,0"));
    parameters.add(gui_currentSegment.set("currentSegment","0,0,0,0,0,0,0,0,0,0,0,0"));
    parameters.add(gui_instancePlayingId.set("instancePlayingId",params.instancePlayingId));

    params.cuePoints = parseJSON("objSeq-cues");
    params.durrationPoints = parseJSON("objSeq-durations");
    params.midpointCues = parseJSON("objSeq-midpoint-cues");
    params.segmentLengths = parseJSON("objSeq-segmentLengths");
    
    params.stillFrame = jsonTrackData["objSeq-still"].asInt();
    params.totalFrames = jsonTrackData["objSeq-files"].asInt();
    
    params.type = jsonTrackData["objSeq-type"].asString();
    params.numOfSeg = jsonTrackData["objSeq-numOfSeg"].asInt();
    
    if(jsonTrackData["objSeq-noteEvents"].asString() == "noteOff"){
        params.playNoteOff = true;
    } else {
        params.playNoteOff = false;
    }
    
    //get all the matcaps from ofApp
    matcaps = ((ofApp*)ofGetAppPtr())->appFileLoader.externalMatCapFiles;
    
    setMatCap(0);
    
    //setup the ofxUI GUI
    setupGui(index);
    
    //output all my params to check
    //trackParameters.reportParams(params, index);
    
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
                for(int i=params.l_copies-1;i>-1;i--){
                    glPushMatrix();
                    glRotatef(i*params.l_rotate.x+params.lRotateModVal.x,1,0,0);
                    glRotatef(i*params.l_rotate.y+params.lRotateModVal.y,0,1,0);
                    glRotatef(i*params.l_rotate.z+params.lRotateModVal.z,0,0,1);
                    
                    glTranslatef(params.l_trans.x+params.lTransModVal.x, params.l_trans.y+params.lTransModVal.y, params.l_trans.z+params.lTransModVal.z+params.mirror_distance);
                    
                    
                    glScalef(params.mirrorX ? params.l_scale+params.lScaleModVal : -(params.l_scale+params.lScaleModVal),
                             params.mirrorY ? params.l_scale+params.lScaleModVal : -(params.l_scale+params.lScaleModVal),
                             params.mirrorZ ? params.l_scale+params.lScaleModVal : -(params.l_scale+params.lScaleModVal)
                             );
                    
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
    
    //GEOMETRY
    //==================================
    for(int i=0; i<params.l_copies;i++){
        //if(instances[i].isPlaying){
            if(instances[i].isTweening){
                if(params.oscControlled){
                    switch (params.tweenType){
                        case 11:
                            //update the frame on the instance
                            instances[i].frame = linearTweens[i].update();
                            
                            if(linearTweens[i].isCompleted()){
                            
                                instances[i].isTweening = false;
                                if(params.playNoteOff){
                                    //If playing noteOn AND noteOff continue.
                                    ofLogNotice("OSC") << i << " - PauseAt(" << instances[i].frame << ")";
                                    
                                    //only play if it's the noteOff side of the message.
                                    if(instances[i].midiState == 0){//<--set in noteOff

                                        //at what level do you want to reset?
                                        if(params.type == "one-shot"){
                                            resetBufferInstance(i,"one-shot");
                                        } else if (params.type == "sequence"){
                                            if(instances[i].currentSegment == params.numOfSeg-1){
                                                resetBufferInstance(i,"one-shot");
                                            } else {
                                                resetBufferInstance(i,"sequence");
                                            }
                                        } else {
                                            //nothing in here for now
                                        }
                                    }//end if .midiState
                                
                                } else {
                                    //ONLY noteOn was triggered.
                                     //at what level do you want to reset?
                                    if(params.type == "one-shot"){
                                        resetBufferInstance(i,"one-shot");
                                    } else if (params.type == "sequence"){
                                        if(instances[i].currentSegment == params.numOfSeg-1){
                                            resetBufferInstance(i,"one-shot");
                                        } else {
                                            resetBufferInstance(i,"sequence");
                                        }
                                    } else {
                                        //nothing in here for now
                                    }
                                    
                                }//if .playNoteOff
                                
                                
                                
                                
                            }//if linearTweens[i].isCompleted()
                            break;
                    }
                    
                } else {
                    //non osc controlled block
                    
                }// oscControlled
            }//instances[i].isTweening
        //}//instances[i].isPlaying
    }//params.l_copies
    
    //ANIMATION TWEENS
    //====================================================
    
    //RUNNING OBJECT ROTATION TWEENS??
    if(params.randObjRotBoolX){
        params.o_rotate = ofVec3f(posRandomObjRotX.update(),params.o_rotate.y,params.o_rotate.z);
        posRandomObjRotX.isCompleted() ? params.randObjRotBoolX = false : params.randObjRotBoolX = true;
    }
    if(params.randObjRotBoolY){
        params.o_rotate = ofVec3f(params.o_rotate.x,posRandomObjRotY.update(),params.o_rotate.z);
        posRandomObjRotY.isCompleted() ? params.randObjRotBoolY = false : params.randObjRotBoolY = true;
    }
    if(params.randObjRotBoolZ){
        params.o_rotate = ofVec3f(params.o_rotate.x,params.o_rotate.y,posRandomObjRotZ.update());
        posRandomObjRotZ.isCompleted() ? params.randObjRotBoolZ = false : params.randObjRotBoolZ = true;
    }
    
    //RUNNING LOCAL ROTATION TWEENS??
    if(params.randLocalRotBoolX){
        params.l_rotate = ofVec3f(posRandomLocalRotX.update(),params.l_rotate.y,params.l_rotate.z);
        posRandomLocalRotX.isCompleted() ? params.randLocalRotBoolX = false : params.randLocalRotBoolX = true;
    }
    if(params.randLocalRotBoolY){
        params.l_rotate = ofVec3f(params.l_rotate.x,posRandomLocalRotY.update(),params.l_rotate.z);
        posRandomLocalRotY.isCompleted() ? params.randLocalRotBoolY = false : params.randLocalRotBoolY = true;
    }
    if(params.randLocalRotBoolZ){
        params.l_rotate = ofVec3f(params.l_rotate.x,params.l_rotate.y,posRandomLocalRotZ.update());
        posRandomLocalRotZ.isCompleted() ? params.randLocalRotBoolZ = false : params.randLocalRotBoolZ = true;
    }
    
    //RUNNING LOCAL TRANSLATE TWEENS??
    if(params.randLocalPosBoolX){
        params.l_trans = ofVec3f(posRandomLocalX.update(),params.l_trans.y,params.l_trans.z);
        posRandomLocalX.isCompleted() ? params.randLocalPosBoolX = false : params.randLocalPosBoolX = true;
    }
    if(params.randLocalPosBoolY){
        params.l_trans = ofVec3f(params.l_trans.x,posRandomLocalY.update(),params.l_trans.z);
        posRandomLocalY.isCompleted() ? params.randLocalPosBoolY = false : params.randLocalPosBoolY = true;
    }
    if(params.randLocalPosBoolZ){
        params.l_trans = ofVec3f(params.l_trans.x,params.l_trans.y,posRandomLocalZ.update());
        posRandomLocalZ.isCompleted() ? params.randLocalPosBoolZ = false : params.randLocalPosBoolZ = true;
    }
    
    
    
    
    
    if(params.randomized){
        //randomized positions.
        //params.l_trans = ofVec3f(positiontweenbounce_x.update(),positiontweenbounce_y.update(),params.l_trans.z);

        if(positiontweenbounce_x.isCompleted()){
            //params.randomized = false;
        }
        
        params.o_rotate = ofVec3f(posRandomObjRotX.update(),params.o_rotate.y,params.o_rotate.z);
        
        if(posRandomObjRotX.isCompleted()){
            params.randomized = false;
        }
        
    }//end params.randomized
    
    
    
    
    
    //FLOATING DEBUG MENU ON RIGHT
    //=======================================================================
    //validate what's playing in ofxGUI
    gui_buffers.set("buffers",params.l_copies);
    gui_instancePlayingId.set("instancePlayingId",params.instancePlayingId);
    
    string cued2playList = "";
    string instanceList = "";
    string isTweeningList = "";
    string isPlayingList = "";
    string currentSegmentList = "";

    
    for(int i=0;i<params.l_copies;i++){
        cued2playList += ofToString(instances[i].cued2play);
        instanceList += ofToString(instances[i].noteID);
        isTweeningList += ofToString(instances[i].isTweening);
        isPlayingList += ofToString(instances[i].isPlaying);
        currentSegmentList += ofToString(instances[i].currentSegment);

        if(i < params.l_copies-1){
            cued2playList += ",";
            instanceList += ",";
            isTweeningList += ",";
            isPlayingList += ",";
            currentSegmentList += ",";
        }
    }
   
    gui_cued2PlayList.set(cued2playList);
    gui_instanceList.set(instanceList);
    gui_isTweeningList.set(isTweeningList);
    gui_isPlayingList.set(isPlayingList);
    gui_currentSegment.set(currentSegmentList);
    //=======================================================================
    //END - FLOATING DEBUG MENU ON RIGHT.


    //sets the button a color when a osc message is recieved.
    setIndicator();


}

//--------------------------------------------------------------
void vboMeshObj::KeyboardLaunch(int _string, int _note, int _tweenType, int _instanceId, bool _noteOff){
    
    int VMMnoteID = ofToInt(ofToString(_string) + ofToString(_note));
    
    advanceInstance();
    
    noteOn(params.instancePlayingId, VMMnoteID, _note, 127, 500);
    play(params.instancePlayingId, VMMnoteID, params.testSpeed, 11);
    
    if(_noteOff){
        cout << "play /noteOff" << endl;
    }
    
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
   
    //test button
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    //gui->addLabelToggle("spinZ", &params.spinZ,50,12,0,0,false);
    gui->addLabelButton("TEST", false,50,20,0,0);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addLabelButton("CLEAR", false,50,20,0,0);
    gui->addLabelButton("RANDOM", false,50,20,0,0);
    gui->addIntSlider("SPEED", 1, 2000, &params.testSpeed, 150,8,0,0);
    
    //loaded.
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addToggle("LOADED", &params.isLoaded);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    gui->addToggle("STILL", &params.still);
    
    gui->addToggle("OSC", &params.oscControlled);//not sure if this is useful anymore.
    gui->addToggle("MIRROR", &params.mirrored);
    
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addLabelToggle("mirror x", &params.mirrorX,75,12,0,0,false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addLabelToggle("mirror y", &params.mirrorY,75,12,0,0,false);
    gui->addLabelToggle("mirror z", &params.mirrorZ,75,12,0,0,false);
    

    
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addToggle("playNoteOff", &params.playNoteOff);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addToggle("playAll", &params.playAll);
    gui->addToggle("finalize", &params.playAllFinalize);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addIntSlider("FRAME", 1, params.totalFrames, &params.stillFrame, 300,8,0,0);
    gui->addSlider("MIRROR",-100.0,100.0, &params.mirror_distance,150,8,0,0);
    
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
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addIntSlider("slices " + index, 1, 4, &params.l_slices,150,8,150,0);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSlider("(L)SCALE", 0.1, 10.0, &params.l_scale,150,8,150,0);
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
void vboMeshObj::resetBufferInstance(int _buffer, string _mode){
    
        instances[_buffer].cued2play = false;
        instances[_buffer].isTweening = false;
        instances[_buffer].playToEnd = false;//unused
        instances[_buffer].playNoteOff = false;//unused
        instances[_buffer].midiState = 0; // currently playing 1(noteOn), 0(noteOff)
        instances[_buffer].noteID = 0;
        instances[_buffer].note = 0;
        instances[_buffer].vel = 0;
        instances[_buffer].delta = 0;
        
        instances[_buffer].startFrame = 0;
        instances[_buffer].midFrame = 0;
        instances[_buffer].endFrame = 0;
        instances[_buffer].direction = 1;
        instances[_buffer].duration = 0;
        instances[_buffer].clockedDurration = 0;
        
    if (_mode == "one-shot"){
        
        instances[_buffer].isPlaying = false;
        instances[_buffer].currentSegment = -1;//or 0
        instances[_buffer].frame = 0;
        
        if(params.playAllFinalize){
            params.playAllFinalize = false;
            params.playAll = false;
        }
    
        ofLogVerbose("OSC") << "resetBufferInstance > one-shot";
    }
    
    ofLogVerbose("OSC") << "resetBufferInstance - " << _buffer << " has stopped and reset!";
}

//--------------------------------------------------------------
void vboMeshObj::clear(){
    //set the frame to 0

    params.isPlaying = false;
    params.instancePlayingId = -1;
    
    //reset all the buffers.
    for(int t=0; t<params.l_copies;t++){
        resetBufferInstance(t,"one-shot");
    }
    
}


//--------------------------------------------------------------
void vboMeshObj::setMirrorDistance(float _mirDist){
    params.mirror_distance = _mirDist;
}


//--------------------------------------------------------------
void vboMeshObj::setLocalCopies(int _buffers){
    //clear();
    params.l_copies = _buffers;
    setSliceAngle(_buffers);
}


//--------------------------------------------------------------
void vboMeshObj::setLocalSlices(int _slices){
    params.l_slices = _slices;
}


//--------------------------------------------------------------
void vboMeshObj::setSliceAngle(int _copies){
    float sliceAngle = 360.0/_copies;
    params.l_rotate = ofVec3f(0.0,0.0,sliceAngle);
}


//--------------------------------------------------------------
void vboMeshObj::setLocalScale(float _scale){
    params.l_scale = _scale;
}


//bool randLocalRotBoolX;
//ofxTween posRandomLocalRotX;
//ofVec3f lastLocRot
//--------------------------------------------------------------
void vboMeshObj::tweenLocRotX(float _rotX, float _duration){
    lastLocRot = params.l_rotate;    //store the last position
    
    posRandomLocalRotX.setParameters(easingback, ofxTween::easeOut,lastLocRot.x,_rotX, _duration,0);
    
    params.randLocalRotBoolX = true; //start the animation
}

void vboMeshObj::setLocRotX(float _rotX){
    params.l_rotate.x = _rotX;
}

void vboMeshObj::tweenLocRotY(float _rotY, float _duration){
    lastLocRot = params.l_rotate;    //store the last position
    
    posRandomLocalRotY.setParameters(easingback, ofxTween::easeOut,lastLocRot.y,_rotY, _duration,0);
    
    params.randLocalRotBoolY = true; //start the animation
}

void vboMeshObj::setLocRotY(float _rotY){
    params.l_rotate.y = _rotY;
}

void vboMeshObj::tweenLocRotZ(float _rotZ, float _duration){
    lastLocRot = params.l_rotate;    //store the last position
    
    posRandomLocalRotZ.setParameters(easingback, ofxTween::easeOut,lastLocRot.z,_rotZ, _duration,0);
    
    params.randLocalRotBoolZ = true; //start the animation
}

void vboMeshObj::setLocRotZ(float _rotZ){
    params.l_rotate.z = _rotZ;
}


//--------------------------------------------------------------
void vboMeshObj::tweenLocTransX(float _rotX, float _duration){
    lastLocTrans = params.l_trans;    //store the last position
    
    posRandomLocalX.setParameters(easingback, ofxTween::easeOut,lastLocTrans.x,_rotX, _duration,0);
    
    params.randLocalPosBoolX = true; //start the animation
}

void vboMeshObj::setLocTransX(float _rotX){
    params.l_trans.x = _rotX;
}

void vboMeshObj::tweenLocTransY(float _rotY, float _duration){
    lastLocTrans = params.l_trans;    //store the last position
    
    posRandomLocalY.setParameters(easingback, ofxTween::easeOut,lastLocTrans.y,_rotY, _duration,0);
    
    params.randLocalPosBoolY = true; //start the animation
}

void vboMeshObj::setLocTransY(float _rotY){
    params.l_trans.y = _rotY;
}

void vboMeshObj::tweenLocTransZ(float _rotZ, float _duration){
    lastLocTrans = params.l_trans;    //store the last position
    
    posRandomLocalZ.setParameters(easingback, ofxTween::easeOut,lastLocTrans.z,_rotZ, _duration,0);
    
    params.randLocalPosBoolZ = true; //start the animation
}

void vboMeshObj::setLocTransZ(float _rotZ){
    params.l_trans.z = _rotZ;
}




//--------------------------------------------------------------
void vboMeshObj::tweenObjRotX(float _rotX, float _duration){
    lastObjRot = params.o_rotate;    //store the last position
    
    posRandomObjRotX.setParameters(easingback, ofxTween::easeOut,lastObjRot.x,_rotX, _duration,0);
    
    params.randObjRotBoolX = true; //start the animation
}

void vboMeshObj::setObjRotX(float _rotX){
    params.o_rotate.x = _rotX;
}

void vboMeshObj::tweenObjRotY(float _rotY, float _duration){
    lastObjRot = params.o_rotate;    //store the last position
    
    posRandomObjRotY.setParameters(easingback, ofxTween::easeOut,lastObjRot.y,_rotY, _duration,0);
    
    params.randObjRotBoolY = true; //start the animation
}

void vboMeshObj::setObjRotY(float _rotY){
    params.o_rotate.y = _rotY;
}

void vboMeshObj::tweenObjRotZ(float _rotZ, float _duration){
    lastObjRot = params.o_rotate;    //store the last position
    
    posRandomObjRotZ.setParameters(easingback, ofxTween::easeOut,lastObjRot.z,_rotZ, _duration,0);
    
    params.randObjRotBoolZ = true; //start the animation
}

void vboMeshObj::setObjRotZ(float _rotZ){
    params.o_rotate.z = _rotZ;
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

        
    } else if(name == "TEST"){
        ofxUIButton *testbut = (ofxUIButton *) e.widget;
        if(testbut->getValue()){
            
            //launch a test
            KeyboardLaunch(1, 60, 11, params.instancePlayingId, true);

        }
        
    } else if(name == "CLEAR"){
        ofxUIButton *clearbut = (ofxUIButton *) e.widget;
        if(clearbut->getValue()){
            clear();
            
        }
    } else if(name == "RANDOM"){
        ofxUIButton *randbut = (ofxUIButton *) e.widget;
        if(randbut->getValue()){
            randLocalPosition(-15,15,600,400);
        }
    } else if(name == "(L)COPIES " + ofToString(index)){
        ofxUIIntSlider *lcopies = (ofxUIIntSlider *) e.widget;
        //set the z angle to correlate to the number of slices
        
        setSliceAngle(lcopies->getValue());
        
        
        
    }
    

    
}

//--------------------------------------------------------------
void vboMeshObj::keyPressed(int key)
{
    
    //increments the buffer in a clockwise fashion.
    //advanceInstance();
    
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
void vboMeshObj::noteOn(int _buffer, int _noteId, int _note, int _velocity, int _delta){
    
    //ofLogNotice("OSC") << "instances[buffer].cued2play: " << ofToString(_buffer) << "-START";
    
    if(params.playAllFinalize){
        params.playAll = true;
    }
    
    if(params.playAll){
        //flag all the buffers to play
        for(int g=0;g<params.l_copies;g++){
            instances[g].cued2play = true;
            ofLogNotice("OSC") << "instances[" << ofToString(g) << "].cued2play" << "-START(playAll)";
        }
        
    } else {
        //flag ONLY the passed buffer to play
        instances[_buffer].cued2play = true;
        ofLogNotice("OSC") << "instances[" << ofToString(_buffer) << "].cued2play" << "-START";
        
        //calculate the double, triple, quad buffers.
        for(int d=1;d<params.l_slices;d++){
            
            int raw_calc_buf = ((params.l_copies/params.l_slices)*d)+_buffer;
            
            int mod_calc_buf;
            raw_calc_buf > params.l_copies ? mod_calc_buf = raw_calc_buf-params.l_copies : mod_calc_buf = raw_calc_buf;
            
            if(mod_calc_buf == params.l_copies){
                mod_calc_buf=0;
            }
            instances[mod_calc_buf].cued2play = true;
            ofLogNotice("OSC") << "instances[" << ofToString(mod_calc_buf) << "].cued2play(" << ofToString(raw_calc_buf) << ")";
            
        }
    }
    
    //loop through all the buffers and play.
    for(int buffer=0;buffer<params.l_copies;buffer++){
        
        if(instances[buffer].cued2play){//only if the buffer is marked.
            if(instances[buffer].isTweening){
                
                
            } else {
                
                if(instances[buffer].noteID == 0){
                    //loop through all the buffers and setup note ids.
                    //increment the cue/segment.
                    advanceSegment(buffer);
                    
                    //noteId comes from Max and is a unique identifier per instance.
                    //set the instance params.
                    instances[buffer].midiState = 1;
                    instances[buffer].noteID = _noteId;
                    instances[buffer].note = _note;
                    instances[buffer].vel = _velocity;
                    instances[buffer].delta = _delta;
                    
                    //set the target frames in the obj sequence data -- referencing instances[].currentSegment(which segment is playing)
                    instances[buffer].startFrame = params.cuePoints[instances[buffer].currentSegment] - params.segmentLengths[instances[buffer].currentSegment];
                    instances[buffer].midFrame = params.midpointCues[instances[buffer].currentSegment];
                    
                    //either play to the next segment or play to the end (finalize).
                    if(params.playAllFinalize){
                        if(params.type == "sequence"){
                            instances[buffer].endFrame = params.totalFrames;
                            instances[buffer].currentSegment = params.numOfSeg-1;
                        } else {
                            instances[buffer].endFrame = params.cuePoints[instances[buffer].currentSegment];
                        }
                    } else {
                        instances[buffer].endFrame = params.cuePoints[instances[buffer].currentSegment];
                    }
                        


                    
                }

            }//if !isTweening
        }//if cued2play
    }//for
}


//--------------------------------------------------------------
void vboMeshObj::play(int _buffer, int _noteId, int _duration, int _tweenType){
    
    //set the tween type
    params.tweenType = _tweenType;
    
    //set the markers per instance(start,mid,end)
    unsigned startFrame = 0;
    unsigned endFrame = 0;
    unsigned delay = 0;//not used
    
    for(int buffer=0;buffer<params.l_copies;buffer++){
       
        if(instances[buffer].cued2play){//only if the buffer is marked.
            if(!instances[buffer].isTweening){
                if(instances[buffer].noteID == _noteId){
                    //setup the tween by figuring out the start and end points.
                    if(params.playNoteOff){
                        //Note On > Note Off
                        if(instances[buffer].midiState == 1){
                            //START > MID the PAUSE
                            startFrame = instances[buffer].startFrame;
                            endFrame = instances[buffer].midFrame;
                            
                        } else {
                            //MID > END then STOP
                            startFrame = instances[buffer].midFrame;
                            endFrame = instances[buffer].endFrame;
                            
                        }
                    } else {
                        //Note On
                        //play start-end and stop
                        startFrame = instances[buffer].startFrame;
                        endFrame = instances[buffer].endFrame;
                        
                    }
                    
                    //remember the duration
                    instances[buffer].duration = _duration;
                    
                    ofLogNotice("OSC") << buffer << " - Play(" << startFrame << "-" << endFrame << ") - " << instances[buffer].duration;
                    
                    //setup the Tween
                    tweenPlayInstance(buffer, params.tweenType, startFrame, endFrame, instances[buffer].duration, delay);
                
                    instances[buffer].isPlaying = true;
                }//noteID > 0
            }//!.isPlaying
        }//.cued2play
    }//end for
}


//--------------------------------------------------------------
void vboMeshObj::noteOff(int _noteId, int _durration){
    
    for(int buffer=0;buffer<params.l_copies;buffer++){
        if(instances[buffer].cued2play){//only if the buffer is marked.
            instances[buffer].clockedDurration = _durration;
            
            instances[buffer].midiState = 0;//midiState = buffer is currently playing noteOn or Off.
            if(params.playNoteOff){//play 2nd half of animation
                
                //play only the note off.
                //play(buffer,_noteId,500,params.tweenType);
                
                if(instances[buffer].isTweening){
                    //tweenPlayInstance(buffer, params.tweenType, instances[buffer].frame, instances[buffer].startFrame, instances[buffer].duration, 0);
                    ofLogNotice("OSC") << buffer << " - Frame(" << instances[buffer].frame << ")";
                    resetBufferInstance(buffer, "sequence");
                    instances[buffer].frame = instances[buffer].startFrame;
                } else {
                    if(instances[buffer].noteID == _noteId){
                        tweenPlayInstance(buffer, params.tweenType, instances[buffer].midFrame, instances[buffer].endFrame, instances[buffer].duration, 0);
                        ofLogNotice("OSC") << buffer << " - End(" << instances[buffer].noteID << ")";
                    }
                }
                
            }//if .playNoteOff
        }// .cued2Play
    }//for
}//noteOff()



//--------------------------------------------------------------
void vboMeshObj::tweenPlayInstance(int _buffer, int _tweenType, int _start, int _end, int _duration, int _delay){
    //RIGHT NOW ONLY WORKS ON LINEAR TWEEN MODE.
    switch(_tweenType){
        case 11:
            //TURN ON THE ANIMATION
            if(instances[_buffer].isTweening == false){
                
                //move this if noteID out to play().
                if(instances[_buffer].noteID > 0){
                    
                    //PLAY JUST THE SELECTED INSTANCE
                    linearTweens[_buffer].setParameters(11+_buffer,easinglinear, ofxTween::easeOut,_start,_end,_duration,_delay);
                    instances[_buffer].isTweening = true;
                
                
                } else {
                    ofLogNotice("OSC") << "noteID not set!";
                }
                
            } else {
                
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
