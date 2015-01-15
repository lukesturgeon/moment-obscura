#include "ofApp.h"


/*
 
 TODO
 ----
 c = remove 3 points
 clean code
 measure code performance
 improve code performance
 
 */

void ofApp::setup(){
    ofSetWindowTitle("Moment Obscura");
    
    // setup the display
    ofEnableSmoothing();
    ofBackground(0);
    ofSetColor(255);
    
    // setup myNodes array
    initGrid();
    
    // setup attractor
    myAttractor.strength = -5;
    myAttractor.ramp = 0.1;
    
    // setup camera
    camWidth 		= 320;
	camHeight 		= 240;
    vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
}


void ofApp::update(){
    vidGrabber.update();
    
    myAttractor.x = ofGetMouseX();
    myAttractor.y = ofGetMouseY();
    
    vector<ofPoint> points;
    
    for (int i = 0; i < myNodes.size(); i++) {
        
        if(isMousePressed){
            myAttractor.attract(&myNodes[i]);
        }
        
        myNodes[i].update();
        
        points.push_back(myNodes[i].position);
    }
    
    // update the mesh with new points
    triangulation.reset();
    triangulation.addPoints(points);
    triangulation.triangulate();
}


void ofApp::draw(){
    
    if(doPrintImage){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    if(doShowFaces){
        ofPixelsRef pixelsRef = vidGrabber.getPixelsRef();
        
        vector<ofMeshFace> f = triangulation.triangleMesh.getUniqueFaces();
        int px, py;
        float cx, cy;
        
        for (int i = 0; i < f.size(); i++) {
            
            // calculate the centroid
            cx = (f[i].getVertex(0).x + f[i].getVertex(1).x + f[i].getVertex(2).x) / 3;
            cy = (f[i].getVertex(0).y + f[i].getVertex(1).y + f[i].getVertex(2).y) / 3;
            
            // get the colour for this triangle
            px = ofMap(cx, 0, ofGetWidth(), camWidth, 0);
            py = ofMap(cy, 0, ofGetHeight(), 0, camHeight);
            ofColor col = pixelsRef.getColor(px, py);
            ofSetColor(col);
            ofFill();
            
            // draw the triangle
            ofTriangle(f[i].getVertex(0).x, f[i].getVertex(0).y,
                       f[i].getVertex(1).x, f[i].getVertex(1).y,
                       f[i].getVertex(2).x, f[i].getVertex(2).y);
        }
    }
    
    if(doPrintImage){
        doPrintImage = false;
        ofEndSaveScreenAsPDF();
    }
    
    if (doShowWireframe) {
        ofPushStyle();
        ofSetColor(255);
        triangulation.triangleMesh.drawWireframe();
        ofPopStyle();
    }
    
    if (doSaveImage){
        doSaveImage = false;
        ofSaveScreen("screenshot-"+ofGetTimestampString()+".png");
    }
    
    if(doShowVertices){
        ofSetColor(255);
        ofFill();
        for (int i = 0; i < myNodes.size(); i++) {
            ofRect(myNodes[i].position.x, myNodes[i].position.y, 1, 1);
        }
    }
    
    // show help text
    ofSetColor(255);
    ofDrawBitmapString("v: toggle vertices", 10, 20);
    ofDrawBitmapString("f: toggle faces", 10, 40);
    ofDrawBitmapString("w: toggle wireframe", 10, 60);
    ofDrawBitmapString("r: add random points", 10, 80);
    ofDrawBitmapString("x: reset grid", 10, 100);
    ofDrawBitmapString("p: save pdf", 10, 120);
    ofDrawBitmapString("' ' : save frame", 10, 140);
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 170);
}


void ofApp::initGrid(){
    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount; x++) {
            float xpos = x * (gridSize/(xCount-1)) + (ofGetWidth()-gridSize)/2;
            float ypos = y * (gridSize/(yCount-1)) + (ofGetHeight()-gridSize)/2;
            
            Node n = *new Node();
            n.setPosition(xpos, ypos);
            n.setVelocity(0,0);
            n.setBoundary(0, 0, ofGetWidth(), ofGetHeight());
            n.setDamping(0.02);
            
            myNodes.push_back(n);
        }
    }
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'x':
            myNodes.clear();
            initGrid();
            break;
            
        case 'v':
            doShowVertices = !doShowVertices;
            break;
            
        case 'f':
            doShowFaces = !doShowFaces;
            break;
            
        case 'w':
            doShowWireframe = !doShowWireframe;
            break;
            
        case 'p':
            doPrintImage = true;
            break;
            
        case 'r':
            // insert 3 new points (i triangle)
            for (int i = 0; i < 3; i++) {
                Node n = *new Node();
                n.setPosition(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
                n.setVelocity(0,0);
                n.setBoundary(0, 0, ofGetWidth(), ofGetHeight());
                n.setDamping(0.02);
                
                myNodes.push_back(n);
            }
            triangulation.triangulate();
            break;
            
        case ' ':
            cout << "save" << endl;
            doSaveImage = true;
            break;
    }
}

void ofApp::mousePressed(int x, int y, int button){
   isMousePressed = true;
}

void ofApp::mouseReleased(int x, int y, int button){
    isMousePressed = false;
}