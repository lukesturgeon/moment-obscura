#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCsv.h"
#include "ofxDelaunay.h"
#include "ofxGeometricStuff.h"

using namespace wng;

class ofApp : public ofBaseApp{

    public:
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
    
        void radiusChanged(float & n);
        void attractionChanged(float & n);
        void repulsionChanged(float & n);
        void rampChanged(float & n);
        void frictionChanged(float & n);
    
        void createPointNear(float x, float y, float radius);
        void stopRecording();
        void startRecording();
        void updateSimulation();
        void updateTriangulation();
    
        //GeometricStuff
        Attractor myAttractor;
        Attractor myRepeller;
        vector<Node> myNodes;
        ofxDelaunay triangulation;
    
        //forces
        float a_angle_deg_x;
        float a_angle_deg_y;
        float a_speed;
        float r_angle_deg_x;
        float r_angle_deg_y;
        float r_speed;
    
        //internal settings
        int                 currentFrame;
        float               camWidth;
        float               camHeight;
        float               clippingDistance;
        ofPoint             clippingAnchor;
        ofRectangle         outputWindow;
        ofxCsv              pointRecorder;
        ofVideoPlayer 		fingerMovie;
        vector<ofPoint>     pointData;
        bool                showHelp;
        bool                doSave;
        bool                isRecording;
        bool                isRendering;    
        unsigned long       lastTimer;

    
        // Parameters to change
        ofxPanel gui;
        ofParameter<float>  radius;
        ofParameter<float>  attraction;
        ofParameter<float>  repulsion;
        ofParameter<float>  ramp;
        ofParameter<float>  friction;
        ofParameter<float>  scatter;
        ofParameter<float>  maxTriangleArea;
};
