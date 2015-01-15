#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowTitle("MO_02_Attractor");
    
    // setup graphics
    ofEnableSmoothing();
    ofFill();
    ofSetColor(0);
    ofBackground(255);
    
    // setup myNodes array
    initGrid();
    
    // setup attractor
    myAttractor.strength = -5;
    myAttractor.ramp = 0.2;
}

void ofApp::update(){
    myAttractor.x = ofGetMouseX();
    myAttractor.y = ofGetMouseY();
    
    for (int i = 0; i < NUM_NODES; i++) {
        
        if(isMousePressed){
            myAttractor.attract(&myNodes[i]);
        }
        
        myNodes[i].update();
    }
}

void ofApp::draw(){
    
    ofSetBackgroundColor(255);
    
    if(doPrintImage){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        if (doPrintImage) {
            ofEllipse(myNodes[i].x, myNodes[i].y, 1, 1);
        }
        else {
            ofRect(myNodes[i].x, myNodes[i].y, 1, 1);
        }
    }
    
    if(doPrintImage){
        doPrintImage = false;
        ofEndSaveScreenAsPDF();
    }
    
    if (doSaveImage){
        doSaveImage = false;
        ofSaveFrame();
    }
}

void ofApp::initGrid(){
    int i = 0;
    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount; x++) {
            float xpos = x * (gridSize/(xCount-1)) + (ofGetWidth()-gridSize)/2;
            float ypos = y * (gridSize/(yCount-1)) + (ofGetHeight()-gridSize)/2;
            myNodes[i].setPosition(xpos, ypos);
            myNodes[i].setVelocity(0,0);
            myNodes[i].setBoundary(0, 0, ofGetWidth(), ofGetHeight());
            myNodes[i].setDamping(0.02);
            i++;
        }
    }    
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'r':
        initGrid();
        break;
        
        case 'p':
        cout << "print" << endl;
        doPrintImage = true;
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


