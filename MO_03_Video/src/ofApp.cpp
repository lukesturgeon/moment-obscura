#include "ofApp.h"


void ofApp::setup(){
    ofSetWindowTitle("Delaunay Video test");
    
    // setup the display
    ofEnableSmoothing();
    ofBackground(0);
    ofSetColor(255);
    
    // setup camera
    camWidth 		= 320;
	camHeight 		= 240;
    vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
}


void ofApp::update(){
    vidGrabber.update();
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
        ofSaveFrame();
    }
    
    if(doShowVideo){
        ofSetColor(255);
        ofFill();
        vidGrabber.draw(0,0);
    }
    
    // show help text
    ofSetColor(255);
    ofDrawBitmapString("v: toggle video", 10, 20);
    ofDrawBitmapString("f: toggle faces", 10, 40);
    ofDrawBitmapString("w: toggle wireframe", 10, 60);
    ofDrawBitmapString("r: add random points", 10, 80);
    ofDrawBitmapString("x: clear all", 10, 100);
    ofDrawBitmapString("p: save pdf", 10, 120);
    ofDrawBitmapString("' ' : save frame", 10, 140);
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 170);
}


void ofApp::keyPressed(int key){
    
    switch (key) {
            
        case 'x':
            triangulation.reset();
            break;
            
        case 'v':
            doShowVideo = !doShowVideo;
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
                triangulation.addPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 0);
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
    triangulation.addPoint(ofPoint(x,y));
    triangulation.triangulate();
}