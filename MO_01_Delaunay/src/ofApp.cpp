#include "ofApp.h"


void ofApp::setup(){
    ofSetWindowTitle("Delaunay test");
    
    // setup the display
    ofEnableSmoothing();
    ofBackground(0);
    ofSetColor(255);
    
    // load a bunch of location images
    imgs[0].loadImage("_0000_24bc665e309b12e8f59a496af4af53b9.jpg");
    imgs[1].loadImage("_0001_39e90573cacf518a5f71a4a95ad66f7b.jpg");
    imgs[2].loadImage("_0002_81f1d42ca2e8be071aae8e64f592d752.jpg");
    imgs[3].loadImage("_0003_294e4ad17ba0c7e6f373c737135892e9.jpg");
    imgs[4].loadImage("_0004_05789a4e18790e09bd43d2a6fbbf8311.jpg");
    imgs[5].loadImage("_0005_5960dbf5380b34a647ab8cdf73f84361.jpg");
    imgs[6].loadImage("_0006_62267b18e7d18658e2f1d147c1d97873.jpg");
    imgs[7].loadImage("_0007_71071fdbd81176a8e6a7700c79414e12.jpg");
    imgs[8].loadImage("_0008_bdacb627207b209d6fd0efe825b8dc6d.jpg");
    imgs[9].loadImage("_0009_c97f0709e8908aa253d940c2e075b0ff.jpg");
    imgs[10].loadImage("_0010_f63520cc780b82c25fd91559b12f3cf6.jpg");
    imgs[11].loadImage("_0011_1bac80625b4caa65f34957b8e384af12.jpg");
    
    // start with a random image
    imgIndex = ofRandom(0, NUM_IMAGES-1);
}


void ofApp::update(){
}


void ofApp::draw(){
    
    if(doPrintImage){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    if(doShowFaces){
        vector<ofMeshFace> f = triangulation.triangleMesh.getUniqueFaces();
        int px, py;
        float cx, cy;
        
        for (int i = 0; i < f.size(); i++) {
            
            // calculate the centroid
            cx = (f[i].getVertex(0).x + f[i].getVertex(1).x + f[i].getVertex(2).x) / 3;
            cy = (f[i].getVertex(0).y + f[i].getVertex(1).y + f[i].getVertex(2).y) / 3;
            
            // get the colour for this triangle
            px = ofMap(cx, 0, ofGetWidth(), 0, imgs[imgIndex].getWidth());
            py = ofMap(cy, 0, ofGetHeight(), 0, imgs[imgIndex].getHeight());
            ofColor col = imgs[imgIndex].getColor(px, py);
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
    
    if(doShowImage){
        ofSetColor(255);
        ofFill();
        imgs[imgIndex].draw(0,0);
    }
    
    // show help text
    ofSetColor(255);
    
    ofDrawBitmapString("f: toggle faces", 10, 40);
    ofDrawBitmapString("w: toggle wireframe", 10, 60);
    ofDrawBitmapString("r : add random points", 10, 80);
    ofDrawBitmapString("x : clear all", 10, 100);
    ofDrawBitmapString("i: toggle image", 10, 120);
    ofDrawBitmapString("left : prev image", 10, 140);
    ofDrawBitmapString("right : next image", 10, 160);
    ofDrawBitmapString("' ' : save frame", 10, 180);
    ofDrawBitmapString("triangles: " + ofToString(numTriangles), 10, 200);
}


void ofApp::keyPressed(int key){
    
    switch (key) {
        case 356:
            imgIndex = (imgIndex == 0) ? NUM_IMAGES-1 : imgIndex-1;
            break;
            
        case 358:
            imgIndex = (imgIndex + 1) % NUM_IMAGES;
            break;
            
        case 'i':
            doShowImage = !doShowImage;
            break;
            
        case 'x':
            triangulation.reset();
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
            numTriangles = triangulation.triangulate();
            break;
            
        case ' ':
            doSaveImage = true;
            break;
    }
}

void ofApp::mousePressed(int x, int y, int button){
    triangulation.addPoint(ofPoint(x,y));
    numTriangles = triangulation.triangulate();
}