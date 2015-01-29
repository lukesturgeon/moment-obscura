#include "ofApp.h"


/**
 
 TODO
 
 Ðrandom range with focus area, like toxi - then random within the center of the screen
 Ðmaybe include an opposite attractor?
 **/


void ofApp::setup(){
    ofSetWindowTitle("Moment Obscura");
    
    // setup the display
    ofEnableSmoothing();
    ofBackground(0);
    ofSetColor(255);
    
    // we add this listener before setting up so the initial value is correct
	radius.addListener(this, &ofApp::attractorRadiusChanged);
    strength.addListener(this, &ofApp::attractorStrengthChanged);
    ramp.addListener(this, &ofApp::attractorRampChanged);
    
    // setup GUI
    gui.setup();
    gui.add( attractorMoveX.setup( "move x", 200, 0, ofGetWidth()/2) );
    gui.add( attractorMoveY.setup( "move y", 200, 0, ofGetHeight()/2) );
    
    gui.add( attractorSpeedX.setup( "speed x", 0.3, 0.1, 2) );
    gui.add( attractorSpeedY.setup( "speed y", 0.3, 0.1, 2) );
    
    gui.add( radius.setup( "radius", 200, 10, ofGetWidth()) );
    gui.add( strength.setup( "strength", -5, -10, 10) );
    gui.add( ramp.setup( "ramp", 0.1, 0.01, 0.5) );
    gui.add( maxTriangleArea.setup( "max area", 300, 1, ofGetHeight()) );
    
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


//--------------------------------------------------------------
void ofApp::attractorRadiusChanged(float &attractorRadius){
	myAttractor.radius = attractorRadius;
}

//--------------------------------------------------------------
void ofApp::attractorStrengthChanged(float &attractorStrength){
	myAttractor.strength = attractorStrength;
}

//--------------------------------------------------------------
void ofApp::attractorRampChanged(float &attractorRamp){
	myAttractor.ramp = attractorRamp;
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    float ang1 = ofDegToRad(angleX);
    float ang2 = ofDegToRad(angleY);
    
    myAttractor.x = ofGetWidth()/2 + (attractorMoveX * cos(ang1));
    myAttractor.y = ofGetHeight()/2 + (attractorMoveY * sin(ang2));
    
    angleX += attractorSpeedX;
    angleY += attractorSpeedY;
    
//    myAttractor.x = ofGetMouseX();
//    myAttractor.y = ofGetMouseY();
    
    vector<ofPoint> points;
    
    // update all the nodes
    for (int i = 0; i < myNodes.size(); i++) {
        myAttractor.attract(myNodes[i]);
        myNodes[i]->update();
        points.push_back(myNodes[i]->position);
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
            
            // get the size of the triangle
            if (abs(f[i].getVertex(0).x - cx) < maxTriangleArea) {
                
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
    }
    
    if(doPrintImage){
        doPrintImage = false;
        ofEndSaveScreenAsPDF();
    }
    
    if (doShowWireframe) {
        ofPushStyle();
        ofSetColor(255);
        triangulation.triangleMesh.drawWireframe();
        
        ofNoFill();
        ofCircle( myAttractor.x, myAttractor.y, myAttractor.radius );
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
            ofRect(myNodes[i]->position.x, myNodes[i]->position.y, 1, 1);
        }
    }
    
    if(doShowHelp){
        // show the GUI
        gui.draw();
        
        
        // show help text
        ofPushMatrix();
            ofTranslate(0, 200);
            ofSetColor(255);
            ofDrawBitmapString("v: toggle vertices", 10, 20);
            ofDrawBitmapString("f: toggle faces", 10, 40);
            ofDrawBitmapString("w: toggle wireframe", 10, 60);
            ofDrawBitmapString("r: add random points", 10, 80);
            ofDrawBitmapString("x: reset grid", 10, 100);
            ofDrawBitmapString("p: save pdf", 10, 120);
            ofDrawBitmapString("' ' : save frame", 10, 140);
            ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 170);
        ofPopMatrix();
    }
}


void ofApp::initGrid(){
    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount; x++) {
            float xpos = x * (gridSize/(xCount-1)) + (ofGetWidth()-gridSize)/2;
            float ypos = y * (gridSize/(yCount-1)) + (ofGetHeight()-gridSize)/2;
            
            ofPtr<Node> n(new Node());
            n->setPosition(xpos, ypos);
            n->setVelocity(0,0);
            n->setBoundary(0, 0, ofGetWidth(), ofGetHeight());
            n->setDamping(0.02);
            myNodes.push_back(n);
        }
    }
}

void ofApp::keyPressed(int key){
    switch (key) {
            
        case 'h':
            doShowHelp = !doShowHelp;
            break;
            
        case 'x':
            myNodes.clear();
//            initGrid();
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
                ofPtr<Node> n(new Node());
                n->setPosition(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
                n->setVelocity(0,0);
                n->setBoundary(0, 0, ofGetWidth(), ofGetHeight());
                n->setDamping(0.02);
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