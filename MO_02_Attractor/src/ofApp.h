#pragma once

#include "ofMain.h"
#include "Node.h"
#include "Attractor.h"

#define NUM_NODES 90601

class ofApp : public ofBaseApp{

	public:
    
        // Methods
        void setup();
        void update();
        void draw();
        void keyPressed(int key);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void initGrid();
    
    
        // Grid
        int xCount = 301;
        int yCount = 301;
        float gridSize = 600;
    
    
        // Nodes
        Node myNodes[NUM_NODES];
    
    
        // Attractor
        Attractor myAttractor;
    
    
        // Output
        bool doSaveImage = false;
        bool doPrintImage = false;
    
        // Input
        bool isMousePressed = false;
		
};
