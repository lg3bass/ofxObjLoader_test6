#pragma once

#include "ofMain.h"
#include "bwUtil.h"

class objFileLoader {
    
    public:

    //meshes and objects
    struct extObjFile {
        string name;
        string path;
        int numFiles;
        vector<ofFile> objs;
        vector<ofFile> mtls;
    };
    
    //constructor
    objFileLoader();
    
    void setup(); 
    void loadObjFiles(string _path);
    vector<extObjFile> externalObjFiles;
    
    void loadMatCapFiles(string _path);
    vector<string> externalMatCapFiles;
    
    private:
    
};