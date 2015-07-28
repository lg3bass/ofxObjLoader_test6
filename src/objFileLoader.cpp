#include "objFileLoader.h"

//--------------------------------------------------------------
objFileLoader::objFileLoader(){
    
    
}


//--------------------------------------------------------------
void objFileLoader::setup(){
    
    loadObjFiles("obj");
    
}


//--------------------------------------------------------------
void objFileLoader::loadObjFiles(string _path){
    
    //get the directories
    string path = _path;
    ofDirectory dir(path);
    dir.listDir();
    
    //run through the dir to collect all the files.
    for(int j=0;j<dir.numFiles();j++){
        
        int fileCounter = 0;
        
        externalObjFiles.push_back(extObjFile());
        //get metadata on file folders
        externalObjFiles[j].name = dir.getName(j);
        externalObjFiles[j].path = path+"/"+dir.getName(j)+"/";
        
        ofDirectory objDirectory(dir.getPath(j));
        objDirectory.allowExt("obj");
        objDirectory.allowExt("mtl");
        objDirectory.listDir();
        
        //create a vector of files
        vector<ofFile> objDirectoryFiles = objDirectory.getFiles();
        
        for(int k=0;k<objDirectory.size();k++){
            if(objDirectoryFiles[k].getExtension() == "obj"){
                externalObjFiles[j].objs.push_back(ofFile(objDirectoryFiles[k]));
                fileCounter++;
            }
            if(objDirectoryFiles[k].getExtension() == "mtl"){
                externalObjFiles[j].mtls.push_back(ofFile(objDirectoryFiles[k]));
            }
            
        }
        
        ofSort(externalObjFiles[j].objs);
        ofSort(externalObjFiles[j].mtls);
        
        //struct: How many files.
        externalObjFiles[j].numFiles = fileCounter;
        
        //list obj directories and number of files
        cout << externalObjFiles[j].name << ":" << objDirectory.size() << endl;
        //list files in obj directory
        cout << externalObjFiles[j].name << "-obj:" << externalObjFiles[j].objs.size() << endl;
        cout << externalObjFiles[j].name << "-mtl:" << externalObjFiles[j].mtls.size() << endl;
        //display externalObjFiles[].numFiles
        cout << "externalObjFiles[<var>].numFiles " << externalObjFiles[j].numFiles << endl;
    }
    

    
}
