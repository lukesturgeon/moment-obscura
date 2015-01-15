#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"

#define NUM_IMAGES 12

class ofApp : public ofBaseApp{

	public:
        // Methods
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void mousePressed(int x, int y, int button);
    
        // background images
        ofImage imgs[NUM_IMAGES];
        int imgIndex;
    
        // Delaunary
        ofxDelaunay triangulation;
        int numTriangles;
    
        // display
        bool doShowImage = false;
        bool doShowFaces = true;
        bool doShowWireframe = false;
        bool doSaveImage = false;
        bool doPrintImage = false;
		
};
