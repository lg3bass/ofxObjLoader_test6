//
//  oscRouter.cpp
//  ofxObjLoader_test10
//
//  Created by bob white III on 4/28/16.
//
//

#include "oscRouter.h"

//--------------------------------------------------------------
oscRouter::oscRouter(){
    
    
}

//--------------------------------------------------------------
void oscRouter::processOSCmessage(ofxOscMessage &m, vector<vboMeshObj> &tracks, int num_tracks){
    
    //what channel/track
    idx = m.getArgAsInt32(0);
    
    
    if (m.getAddress() == "/noteOn"){
        //cout << "----------------------------------------------------------------" << endl;
        //the message sets the buffer, and general data on the notes.
        
        //VMMnoteID = <string>+<midiNote>;
        VMMnoteID = ofToInt(ofToString(m.getArgAsInt32(2)) + ofToString(m.getArgAsInt32(4)));
        //ofLogNotice("OSC") << "VMMnoteID(ON)------------------------------: " << VMMnoteID;
        
        ofLogVerbose("OSC") << "-------->" << m.getAddress() <<
        " [track:" << m.getArgAsInt32(0) <<
        ", buffer:" << m.getArgAsInt32(1) <<
        ", string:" << m.getArgAsInt32(2) <<
        ", noteId:" << m.getArgAsInt32(3) <<
        ", midiNote:" << m.getArgAsInt32(4) <<
        ", velocity:" << m.getArgAsInt32(5) <<
        ", durration(last):" << m.getArgAsInt32(6) <<
        ", delta:" << m.getArgAsInt32(7) <<
        ", noteOn|Off:" << m.getArgAsInt32(8) <<
        "]";
        
        //----------noteOn(_buffer           |VMMnoteID|_midiNote         |_velocity         |_delta )
        tracks[idx].noteOn(m.getArgAsInt32(1),VMMnoteID,m.getArgAsInt32(4),m.getArgAsInt32(5),m.getArgAsInt32(7));
        
    } else if (m.getAddress() == "/play"){
        
        VMMnoteID = ofToInt(ofToString(m.getArgAsInt32(2)) + ofToString(m.getArgAsInt32(4)));
        
        //This message launches the animation clip and adds
        //the segment to play(coming soon), durration, and tweentype.
        ofLogVerbose("OSC") << ">>>>---->" << m.getAddress() <<
        " [track:" << m.getArgAsInt32(0) <<
        ", buffer:" << m.getArgAsInt32(1) <<
        ", string:" << m.getArgAsInt32(2) <<
        ", noteId:" << m.getArgAsInt32(3) <<
        ", midiNote:" << m.getArgAsInt32(4) <<
        ", duration:" << m.getArgAsInt32(5) <<
        ", cue:" << m.getArgAsInt32(6) <<
        ", tween:" << m.getArgAsInt32(7) <<
        "]";
        
        //----------play(_buffer           | VMMnoteID| _duration         | _tweenType)
        tracks[idx].play(m.getArgAsInt32(1), VMMnoteID, m.getArgAsInt32(5), m.getArgAsInt32(7));
        
    } else if (m.getAddress() == "/noteOff"){
        
        ofLogVerbose("OSC") << "-------->" << m.getAddress() <<
        " [track:" << m.getArgAsInt32(0) <<
        ", string:" << m.getArgAsInt32(1) <<
        ", noteId:" << m.getArgAsInt32(2) <<
        ", midiNote:" << m.getArgAsInt32(3) <<
        ", velocity:" << m.getArgAsInt32(4) <<
        ", real-duration:" << m.getArgAsInt32(5) <<
        ", delta:" << m.getArgAsInt32(6) <<
        ", note On|Off:" << m.getArgAsInt32(7) <<
        "]";
        
        VMMnoteID = ofToInt(ofToString(m.getArgAsInt32(1)) + ofToString(m.getArgAsInt32(3)));
        //ofLogNotice("OSC") << "------------------------------VMMnoteID(OFF): " << VMMnoteID;
        
        //----------noteOff(VMMnoteId| _durration)
        tracks[idx].noteOff(VMMnoteID, m.getArgAsInt32(5));
        
    } else if (m.getAddress() == "/randomTrans"){
        
        ofLogVerbose("OSC") << m.getAddress() <<
        "track:" << m.getArgAsInt32(0) <<
        ", low:" << m.getArgAsInt32(1) <<
        ", high:" << m.getArgAsInt32(2) <<
        ", durration:" << m.getArgAsInt32(3) <<
        ", delay:" << m.getArgAsInt32(4);
        
        //float _start(low), float _end(high), int _durration, int _delay
        tracks[idx].randLocalPosition(m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3),m.getArgAsInt32(4));
        
    } else if (m.getAddress() == "/clear"){
        
        ofLogVerbose("OSC") << m.getAddress() << endl << "track:" << m.getArgAsInt32(0);
        
        //int idx = m.getArgAsInt32(0);
        tracks[idx].clear();
    
        
    //trackParams::setOSCtoggle
    } else if (m.getAddress() == "/loaded"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/still"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/playNoteOff"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/playAll"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/finalize"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/mirror"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/mirrorX"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/mirrorY"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    } else if (m.getAddress() == "/mirrorZ"){
        tracks[idx].trackParameters.setOSCtoggle(tracks[idx].params, m.getAddress(), m.getArgAsInt32(1));
    
    
    } else if (m.getAddress() == "/mirrorDistance"){
        
        tracks[idx].setMirrorDistance(float(m.getArgAsInt32(1)));
        
        //tracks[idx].setPlayAll(m.getArgAsInt32(1));
        
    } else if (m.getAddress() == "/localCopies"){
        //ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << m.getArgAsInt32(1);
        tracks[idx].setLocalCopies(m.getArgAsInt32(1));
        
    } else if (m.getAddress() == "/localSlices"){
        
        tracks[idx].setLocalSlices(m.getArgAsInt32(1));
        
    } else if (m.getAddress() == "/localScale"){
        
        cout << "/localScale " << m.getArgAsFloat(1) << endl;
        tracks[idx].setLocalScale(m.getArgAsFloat(1));
        
    } else if (m.getAddress() == "/randlocalRotX"){
        // LOCAL ROTATE --------------------------------------
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenLocRotX(float(m.getArgAsInt32(1)),1000.0);
    } else if (m.getAddress() == "/tweenlocalRotX"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenLocRotX(float(m.getArgAsInt32(1)),1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomLocalRotX, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 0);
        
    } else if (m.getAddress() == "/setlocalRotX"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocRotX(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randlocalRotY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocRotY(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/tweenlocalRotY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocRotY(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/setlocalRotY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocRotY(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randlocalRotZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocRotZ(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/tweenlocalRotZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocRotZ(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/setlocalRotZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocRotZ(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randLocalTransX"){
        // LOCAL TRANSLATE --------------------------------------
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransX(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/tweenLocalTransX"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransX(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/setLocalTransX"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocTransX(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randLocalTransY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransY(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/tweenLocalTransY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransY(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/setLocalTransY"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocTransY(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randLocalTransZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransZ(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/tweenLocalTransZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].tweenLocTransZ(float(m.getArgAsInt32(1)),1000.0);
        
    } else if (m.getAddress() == "/setLocalTransZ"){
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        tracks[idx].setLocTransZ(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randObjRotX") {
        // OBJECT ROTATE --------------------------------------
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotX(float(m.getArgAsInt32(1)), 1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotX, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);
        
    } else if (m.getAddress() == "/tweenObjRotX") {
        
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotX(float(m.getArgAsInt32(1)),1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotX, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);
        
    } else if (m.getAddress() == "/setObjRotX") {
        tracks[idx].setObjRotX(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randObjRotY") {
        
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotY(float(m.getArgAsInt32(1)), 1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotY, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);

        
    } else if (m.getAddress() == "/tweenObjRotY") {
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotY(float(m.getArgAsInt32(1)),1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotY, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);

        
    } else if (m.getAddress() == "/setObjRotY") {
        tracks[idx].setObjRotY(float(m.getArgAsInt32(1)));
        
    } else if (m.getAddress() == "/randObjRotZ") {
        
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotZ(float(m.getArgAsInt32(1)), 1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotZ, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);

        
    } else if (m.getAddress() == "/tweenObjRotZ") {
        
        ofLogVerbose("OSC") << m.getAddress() << " " << m.getArgAsInt32(0) << " " << float(m.getArgAsInt32(1));
        //tracks[idx].tweenObjRotZ(float(m.getArgAsInt32(1)),1000.0);
        tracks[idx].trackParameters.setOSCtween(tracks[idx].params, tracks[idx].posRandomObjRotZ, m.getAddress(), tracks[idx].easingquad, float(m.getArgAsInt32(1)), 1000.0);

        
    } else if (m.getAddress() == "/setObjRotZ") {
        tracks[idx].setObjRotZ(float(m.getArgAsInt32(1)));
        
    }  else if (m.getAddress() == "/bass"){
        //this mesage streams in a amplitude
        //data from an envelop filter in MAX/LIVE
        
        //m.getArgAsInt32(0) = value from REF filter.
        //m.getArgAsString(1) = note length(sampling rate of Envelope filter)
        
        amplitude = (float)m.getArgAsInt32(0);
        
        noteLength = 4;
        
        /*
         if(m.getArgAsString(1) == "4n"){
         noteLength = 4;
         } else if (m.getArgAsString(1) == "8n"){
         noteLength = 8;
         } else if (m.getArgAsString(1) == "16n"){
         noteLength = 16;
         } else if (m.getArgAsString(1) == "32n"){
         noteLength = 32;
         } else if (m.getArgAsString(1) == "64n"){
         noteLength = 64;
         } else if (m.getArgAsString(1) == "128n"){
         noteLength = 128;
         } else {
         noteLength = 4;
         }
         */
        
        //pass in the data from the bass osc stream.  We only want to pull this in once. Only send if it's loaded
        for(int t=1; t<num_tracks;t++){
            if(tracks[t].params.isLoaded){tracks[t].bassControl(amplitude, noteLength);}
        }
        
    }
    
    
}