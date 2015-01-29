#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "Attractor.h"
#include "Node.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    // Methods
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void attractorRadiusChanged(float & attractorRadius);
    void attractorStrengthChanged(float & attractorStrength);
    void attractorRampChanged(float & attractorRamp);
    void initGrid();
    
    //video
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int 				camHeight;
    
    // Delaunay
    ofxDelaunay triangulation;
    
    // Grid
    int xCount = 20;
    int yCount = 20;
    float gridSize = 600;
    
    // Nodes
    vector< ofPtr<Node> > myNodes;
    // look at ofPtr example
    
    // Attractor
    Attractor myAttractor;
    float angleX;
    float angleY;
    
    // Input
    bool isMousePressed = false;
    
    // GUI
    ofxFloatSlider radius;
    ofxFloatSlider strength;
    ofxFloatSlider ramp;
    ofxFloatSlider attractorMoveX;
    ofxFloatSlider attractorMoveY;
    ofxFloatSlider attractorSpeedX;
    ofxFloatSlider attractorSpeedY;
    
    ofxPanel gui;
    
    // display
    bool doShowHelp = true;
    bool doShowFaces = true;
    bool doShowWireframe = false;
    bool doShowVertices = false;
    bool doSaveImage = false;
    bool doPrintImage = false;
    
};