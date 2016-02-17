#pragma once

#include "ofMain.h"
#include "ofxObjLoader.h"
#include "objFileLoader.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "bwUtil.h"
#include "ofxUI.h"
#include "ofxTween.h"
#include "ofxJSON.h"

class vboMeshObj {
    
    public:
    objFileLoader::extObjFile trackData;
    ofxJSONElement jsonTrackData;
    
    vector<ofVboMesh> vboMesh1;
    int index;//keep track of what instance you are
    
    //intances
    struct instance {
        bool isPlaying;
        bool isTweening;
        bool playToEnd;
        bool playNoteOff;
        int midiState; // 1=noteOn, 0=noteOff
        int currentSegment;
        int noteID;
        int note;
        int vel;
        int delta;
        int frame;
        int startFrame;
        int midFrame;
        int endFrame;
        int direction; //forward=1, reverse=2;
        int duration;
        int clockedDurration;
        
        
    };
    vector<instance> instances;
    
    //matCaps
    ofImage matCap;
    ofShader shader;
    vector<string> matcaps;
    
    
    //drawing params
    struct guiParams {
        bool playAll;
        bool isSelected;
        bool isLoaded;
        bool isPlaying;
        int instancePlayingId;
        int lastInstancePlayed;
        bool still;
        bool oscControlled;
        bool randomized;
        bool mirrored;
        int tweenType;
        bool playNoteOff;
        string type;
        int numOfSeg;
                
        float mirror_distance;
        //int currentSegment;
        vector<int> cuePoints;
        vector<int> durrationPoints;
        vector<int> midpointCues;
        vector<int> segmentLengths;
        int stillFrame;
        int totalFrames;
        
        bool spinX;
        bool spinY;
        bool spinZ;
        ofVec3f spin;
        ofVec3f spinRange;
        int ltransMod;
        
        //mod params
        //global
        int g_copies;
        bool gScale;
        float g_scale;
        float gScaleMod;
        float gScaleModVal;
        bool gTransX;
        bool gTransY;
        bool gTransZ;
        ofVec3f g_trans;
        ofVec3f gTransMod;
        ofVec3f gTransModVal;
        bool gRotX;
        bool gRotY;
        bool gRotZ;
        ofVec3f g_rotate;
        ofVec3f gRotateMod;
        ofVec3f gRotateModVal;
        
        //local
        int l_copies;
        bool lScale;
        float l_scale;
        float lScaleMod;
        float lScaleModVal;
        bool lTransX;
        bool lTransY;
        bool lTransZ;
        ofVec3f l_trans;
        ofVec3f lTransMod;
        ofVec3f lTransModVal;
        bool lRotX;
        bool lRotY;
        bool lRotZ;
        ofVec3f l_rotate;
        ofVec3f lRotateMod;
        ofVec3f lRotateModVal;
        
        //object
        bool oRotX;
        bool oRotY;
        bool oRotZ;
        ofVec3f o_rotate;
        ofVec3f oRotateMod;
        ofVec3f oRotateModVal;
        
    };    
    guiParams params;
    
    
    ofParameterGroup parameters;
    ofParameter<int> gui_buffers;
    
    
    ofParameter<string> gui_instanceList;
    ofParameter<string> gui_isTweeningList;
    ofParameter<string> gui_isPlayingList;
    ofParameter<string> gui_currentSegment;

    
    //OLD - ofParameter ---- REMOVE
    /*
    ofParameterGroup parameters;
    ofParameter<int> gui_gInstances;
    ofParameter<float> gui_gScale;
    ofParameter<float> gui_gRotAngle;
    ofParameter<ofVec3f> gui_gTrans;
    ofParameter<int> gui_lInstances;
    ofParameter<float> gui_lRotAngle;
    ofParameter<ofVec3f> gui_lTrans;
    */

    //ofxTween
//    ofxTween tweenback;
//    ofxTween tweenbounce;
//    ofxTween tweencirc;
//    ofxTween tweencubic;
//    ofxTween tweenelastic;
//    ofxTween tweenexpo;
//    ofxTween tweenquad;
//    ofxTween tweenquart;
//    ofxTween tweenquint;
//    ofxTween tweensine;
//    ofxTween tweenlinear,tweenlinear2,tweenlinear3,tweenlinear4,tweenlinear5,tweenlinear6,tweenlinear7,tweenlinear8;
    
    vector <ofxTween> backTweens;
    vector <ofxTween> bounceTweens;
    vector <ofxTween> circTweens;
    vector <ofxTween> cubicTweens;
    vector <ofxTween> elasticTweens;
    vector <ofxTween> expoTweens;
    vector <ofxTween> quadTweens;
    vector <ofxTween> quartTweens;
    vector <ofxTween> quintTweens;
    vector <ofxTween> sineTweens;
    vector <ofxTween> linearTweens;
    
    ofxTween multitween;
    
    ofxEasingBack       easingback;
    ofxEasingBounce     easingbounce;
    ofxEasingCirc       easingcirc;
    ofxEasingCubic      easingcubic;
    ofxEasingElastic    easingelastic;
    ofxEasingExpo       easingexpo;
    ofxEasingQuad       easingquad;
    ofxEasingQuart      easingquart;
    ofxEasingQuint      easingquint;
    ofxEasingSine       easingsine;
    ofxEasingLinear 	easinglinear;
    
    //tween randLocalPosition
    ofxTween positiontweenbounce_x;
    ofxTween positiontweenbounce_y;
    
    //last position
    ofVec3f lastPosition = ofVec3f(0.0,0.0,0.0);
    
    //constructor
    vboMeshObj();
    
    ofxUICanvas* gui;
    
    void setup(const objFileLoader::extObjFile &_input);
    vector<int> parseJSON(string _param);
    void reportParams(int _index);
    vector<ofVboMesh> passObjTwoVboMesh(vector<ofFile> _files);
    void loadVboMesh(const objFileLoader::extObjFile &_input);
    void setMatCap(int _imgIndex);
    void setShader(string _shader);
    void draw();
    void update();
    void KeyboardLaunch(int _tweenType, int _instanceId);
    void setupGui(int _index);
    void setGuiSnapUnits(string _name,float _unit);
    void setIndicator();
    void randLocalPosition(float _start, float _end, int _durration, int _delay);
    void resetBufferInstance(int _buffer, string _mode);
    void clear();
    void setLocalCopies(int _buffers);
    void bassControl(float &_amp, int _noteLength);
    
    void guiEvent(ofxUIEventArgs &e);
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    //PLAY
    void advanceInstance();
    void advanceSegment(int _buffer);
    void noteOn(int _buffer, int _noteId, int _note, int _velocity, int _delta);
    void play(int _buffer, int _noteID, int _duration, int _tweenType);
    void noteOff(int _noteId, int _durration);
    void tweenPlayInstance(int _buffer, int _tweenType, int _start, int _end, int _duration, int _delay);
    
    void exit();
    
    private:
};