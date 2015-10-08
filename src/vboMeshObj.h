#pragma once

#include "ofMain.h"
#include "ofxObjLoader.h"
#include "objFileLoader.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "bwUtil.h"
#include "ofxUI.h"
#include "ofxTween.h"

class vboMeshObj {
    
    public:
    objFileLoader::extObjFile trackData;

    vector<ofVboMesh> vboMesh1;
    ofImage matCap;
    ofShader shader;
    
    int index;//keep track of what instance you are
    
    int frame;//current frame
    int numFiles;//total number of obj files.
    
    int counter;
    
    //drawing params
    struct guiParams {
        bool isPlaying;
        bool solo;
        bool oscControlled;
        bool mirrored;
        int tweenType;
                
        int g_copies;
        float g_scale;
        float g_rot;
        ofVec3f g_rotate;
        ofVec3f g_trans;
        
        
        int l_copies;
        float l_scale;
        float l_rot;
        ofVec3f l_rotate;
        ofVec3f l_trans;
        
        float mirror_distance;
        float durration_in_ms;
        
    };
    
    
    guiParams params;
    
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
    ofxTween tweenback;
    ofxTween tweenbounce;
    ofxTween tweencirc;
    ofxTween tweencubic;
    ofxTween tweenelastic;
    ofxTween tweenexpo;
    ofxTween tweenquad;
    ofxTween tweenquart;
    ofxTween tweenquint;
    ofxTween tweensine;
    ofxTween tweenlinear;
    
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
    
    //constructor
    vboMeshObj();
    
    ofxUICanvas* gui;
    
    void setup(const objFileLoader::extObjFile &_input, string _img, string _shader,int _index);
    void reportParams(int _index);
    vector<ofVboMesh> passObjTwoVboMesh(vector<ofFile> _files);
    void setupVboMesh(const objFileLoader::extObjFile &_input);
    void setMatCap(string _img);
    void setShader(string _shader);
    void draw();
    void update();
    void OSCLaunch(int _destinationFrame, int _durration, int _segmentLength, int _tweenType);
    void setupGui(int _index);
    void setGuiSnapUnits(string _name,float _unit);
    void setIndicator();
    
    void guiEvent(ofxUIEventArgs &e);
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void exit();
    
    private:
};