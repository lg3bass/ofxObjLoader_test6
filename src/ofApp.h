#pragma once
#include "ofAppGlutWindow.h"
#include "ofMain.h"
//#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "objFileLoader.h"
#include "vboMeshObj.h"
#include "ofxUI.h"

#define HOW_MANY 2


class ofApp : public ofBaseApp{

	public:
    
        enum TEST_SEQENCES{
            BANDTUBE = 1,
            BASSSTAR = 2,
            BLOCKSTAR = 3,
            BOXSTAR = 4,
            UNBLOCKERBASS = 5,
            TRACERS = 6,
            STARBURST = 7,
            WIERDARMS3 = 8,
            WIERDARMS2 = 9,
            WIERDARMS1 = 10,
            LIGHTNING10 = 11,
            LIGHTNING9 = 12,
            LIGHTNING8 = 13,
            LIGHTNING7 = 14,
            LIGHTNING6 = 15,
            EXTRUDETRI = 16,
            EXTRUDESTAR = 17,
            EXTRUDEBALL = 18,
            DIAMONDLAYERS = 19
        };
    
        ofEasyCam cam;
    
        //matcap Shader
        ofImage matCap;
        ofImage matCap2;
        ofImage matCap3;
        ofShader shader;
        
        //modifier keys
        bool modkey;
        bool ctrlKey;


        //1. get paths to the obj files.
        objFileLoader appFileLoader;
        
        //2. load obj files to separate vbomeshes. FOR NOW :)
    
        vboMeshObj track1;
        vboMeshObj track2;
        vboMeshObj track3;
        vboMeshObj track4;
        vboMeshObj track5;
        vboMeshObj track6;
        vboMeshObj track7;
        vboMeshObj track8;
        vboMeshObj track9;
        vboMeshObj track10;
        vboMeshObj track11;
        vboMeshObj track12;
        vboMeshObj track13;
        vboMeshObj track14;
        vboMeshObj track15;
        vboMeshObj track16;
        vboMeshObj track17;
        vboMeshObj track18;
        vboMeshObj track19;
    
        //vboMeshObj track20;
    
        //MASTER LIGHT
        ofLight light0;
    
        //GUI
        ofxPanel gui;
        bool minimized;
        ofParameterGroup parameters;
        ofXml settings;
    
        //ofxUI
        ofxUITabBar *guiTabBar;
        vector<ofxUICanvas *> guis;
    
        void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void drawLights();
        void guiTabEvent(ofxUIEventArgs &e);

    
    
};
