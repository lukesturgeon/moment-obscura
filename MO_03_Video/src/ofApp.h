#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"

class ofApp : public ofBaseApp{
    
public:
    // Methods
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    //video
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int 				camHeight;
    
    // Delaunary
    ofxDelaunay triangulation;
    
    // display
    bool doShowFaces = true;
    bool doShowWireframe = false;
    bool doShowVideo = false;
    bool doSaveImage = false;
    bool doPrintImage = false;
    
};
