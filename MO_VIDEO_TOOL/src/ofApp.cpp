#include "ofApp.h"

// write our boolean remove function
bool shouldRemove(Node &n){
    return n.outOfBounds();
}



//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    
    fingerMovie.loadMovie("movies/C.mov");
//    fingerMovie.setLoopState(OF_LOOP_NONE);
    isRecording = false;
    doSave = false;
    
    // in/out measurements
    outputWindow = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
    camWidth = fingerMovie.width;
    camHeight = fingerMovie.height;
    showHelp = true;
    
    // Setup GeometricStuff
    a_speed = .5;
    r_speed = 1.5;
    
    radius.addListener(this, &ofApp::radiusChanged);
    attraction.addListener(this, &ofApp::attractionChanged);
    repulsion.addListener(this, &ofApp::repulsionChanged);
    ramp.addListener(this, &ofApp::rampChanged);
    friction.addListener(this, &ofApp::frictionChanged);
    
    //GUI
    gui.setup();
    gui.setPosition(20, 20+camHeight+80);
    gui.add(radius.set( "radius", 900, 10, 1280 ));
    gui.add(attraction.set( "attraction", 1, 0, +5 ));
    gui.add(repulsion.set( "repulsion", -1.2, 0, -5 ));
    gui.add(ramp.set( "ramp", 0.3, 0.001, 0.9 ));
    gui.add(friction.set( "friction", 0.001, 0, 0.1 ));
    gui.add(scatter.set( "scatter", 30, 2, 250 ));
    gui.add(maxTriangleArea.set( "max size", 100, 0, 400 ));

}

//--------------------------------------------------------------
void ofApp::exit(){
	radius.removeListener(this, &ofApp::radiusChanged);
    attraction.removeListener(this, &ofApp::attractionChanged);
    repulsion.removeListener(this, &ofApp::repulsionChanged);
    ramp.removeListener(this, &ofApp::rampChanged);
    friction.removeListener(this, &ofApp::frictionChanged);
}

void ofApp::radiusChanged(float & n){
    myAttractor.radius = n;
    myRepeller.radius = n;
}

void ofApp::attractionChanged(float & n){
    myAttractor.strength = n;
}

void ofApp::repulsionChanged(float & n){
    myRepeller.strength = n;
}

void ofApp::rampChanged(float & n){
    myAttractor.ramp = n;
    myRepeller.ramp = n;
}

void ofApp::frictionChanged(float & n){
    for (int i = 0; i < myNodes.size(); i++) {
        myNodes[i].setDamping(n);
    }
}

void ofApp::startRecording() {
    // start recording from beginning
    fingerMovie.setPosition(0);
    fingerMovie.play();
    isRecording = true;
    cout << "START RECORDING" << endl;
}

void ofApp::stopRecording() {
    // stop recording and save data
    fingerMovie.stop();
    isRecording = false;
    pointRecorder.saveFile( ofToDataPath( "MyDataFile.csv" ) );
    cout << "STOP RECORDING" << endl;
}

void ofApp::createPointNear(float x, float y, float radius) {
    Node n = Node();
    n.setPosition( x+ofRandom(-radius,radius), y+ofRandom(-radius,radius) );
    n.setBoundary( 0,0,ofGetWidth(),ofGetHeight() );
    n.setDamping(0.001);
    myNodes.push_back( n );
}

void ofApp::updateTriangulation() {
    // update the mesh with new points
    triangulation.reset();
    triangulation.addPoints(pointData);
    triangulation.triangulate();
}

void ofApp::updateSimulation(){
    //math to move the attractor
    myAttractor.x = ofGetWidth()/2 + (100 * cos( ofDegToRad(a_angle_deg_x) ));
    myAttractor.y = ofGetHeight()/2 + (100 * sin( ofDegToRad(a_angle_deg_y) ));
    a_angle_deg_x += a_speed;
    a_angle_deg_y += a_speed;
    
    //math to move the repeller
    myRepeller.x = ofGetWidth()/2 + (100 * cos( ofDegToRad(r_angle_deg_x) ));
    myRepeller.y = ofGetHeight()/2 + (100 * sin( ofDegToRad(r_angle_deg_y) ));
    r_angle_deg_x += r_speed;
    r_angle_deg_y += r_speed;
    
    
    // update all the nodes
    pointData.clear();
    for (int i = 0; i < myNodes.size(); i++) {
        myAttractor.attract( &myNodes[i] );
        myRepeller.attract( &myNodes[i] );
        myNodes[i].update();
        pointData.push_back(myNodes[i].position);
    }
    ofRemove(myNodes, shouldRemove);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    fingerMovie.update();
    
    // record the points as a string "x:y&x:y"
    if (isRecording)
    {
        updateSimulation();
        
        unsigned long now = ofGetElapsedTimeMillis();
        if(now - lastTimer > 33.333)
        {
            string data = "";
            for (int i = 0; i < pointData.size(); i++) {
                data += ofToString(pointData[i].x)+":"+ofToString(pointData[i].y)+"&";
            }
            
            // remove last "&"
            data = data.substr(0, data.size()-1);
            
            // save in CSV
            int row = pointRecorder.numRows;
            pointRecorder.setInt(row, 0, currentFrame);
            pointRecorder.setString(row, 1, data);
            
            currentFrame++;
            lastTimer = now;
        }
        
        // change the triangle maths
        updateTriangulation();
    }
    else if(isRendering){
        
        unsigned long now = ofGetElapsedTimeMillis();
        if(now - lastTimer > 60)
        {
            // parse the data in to pointData
            pointData.clear();
            vector <string> s = ofSplitString(pointRecorder.getString(currentFrame, 1), "&");
            
            for(int i = 0; i < s.size(); i++)
            {
                if(!s[i].empty())
                {
                    vector <string> ss = ofSplitString(s[i], ":");
                    pointData.push_back( ofPoint(ofToFloat(ss[0]),
                                                 ofToFloat(ss[1])) );
                }
            }
            
            // update the triangle mesh
            updateTriangulation();
            
            if(currentFrame < pointRecorder.numRows-1)
            {
                currentFrame++;
                lastTimer = now;
            }
            else {
                isRendering = false;
                cout << "FINISHED RENDERING" << endl;
            }
        }
    }
    
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);

    // draw coloured shapes
    ofPixelsRef pixelsRef = fingerMovie.getPixelsRef();
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
            px = ofMap(cx, 0, ofGetWidth(), 0, camWidth);
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
    
    
    if (doSave){
        ofSaveScreen(ofToString(currentFrame, 4) +".png");
        doSave = false;
    }
    
    if (isRendering) {
        ofSaveScreen(ofToString(currentFrame, 4) +".png");
    }
    
    
    if (showHelp) {
        
        // draw geometry
        ofPushStyle();
        
        ofSetColor(200, 100, 200);
        ofNoFill();
        ofCircle( myAttractor.x, myAttractor.y, myAttractor.radius );
        ofCircle( myRepeller.x, myRepeller.y, myRepeller.radius );
        triangulation.triangleMesh.drawWireframe();
        
        //draw the help window
        ofSetColor(255);
        ofFill();
        ofRect(0, 0, camWidth+40, ofGetHeight());
        fingerMovie.draw(20, 20, camWidth, camHeight);
        
        ofSetColor(0);
        
        ofPushMatrix();
        ofTranslate(20, 20+camHeight+20);
        
        ofDrawBitmapString("frame: " + ofToString(currentFrame)+"/" + ofToString(pointRecorder.numRows), 0, 0);
        ofDrawBitmapString("points: " + ofToString(pointData.size()),0,20);
        
        if (isRecording)
        {
            ofDrawBitmapString("recording... ", 0, 40);
        }
        else if(isRendering)
        {
            ofDrawBitmapString("rendering... ", 0, 40);
        }
        else
        {
            ofDrawBitmapString("ready...", 0, 40);
        }
        
        ofPopMatrix();
        
        gui.draw();
        
        ofPushMatrix();
        ofTranslate(0, ofGetHeight()-220);
        ofDrawBitmapString("'<' reset video", 20, 60);
        ofDrawBitmapString("'>' end video", 20, 80);
        ofDrawBitmapString("'n' node", 20, 100);
        ofDrawBitmapString("'x' clear", 20, 120);
        ofDrawBitmapString("'s' save image", 20, 140);
        ofDrawBitmapString("'e' export frames", 20, 160);
        ofDrawBitmapString("' ' record", 20, 180);
        ofDrawBitmapString("'?' toggle help", 20, 200);
        ofPopMatrix();
        
        ofPopStyle();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'n' :
            if (isRecording)
            {
                // insert 3 new points (i triangle)
                int qty;
                qty = 3*ofRandom(30);
                for (int i = 0; i < qty; i++) {
                    createPointNear(myRepeller.x, myRepeller.y, 600);
                }
            }
            break;
            
        case 'x' :
            currentFrame = 0;
            pointData.clear();
            pointRecorder.clear();
            myNodes.clear();
            triangulation.reset();
            break;
            
        case 's' :
            doSave = true;
            break;
            
            case 'e':
            if(!isRecording && pointRecorder.numRows>0)
            {
                currentFrame = 0;
                isRendering = true;
                cout << "RENDER" << endl;
            }
            break;
            
        case '?' :
            showHelp = !showHelp;
            break;
            
        case ' ':
            
            if(isRecording)
            {
                stopRecording();
            }
            else
            {
                startRecording();
            }
            
            break;
            
        case OF_KEY_LEFT:
            
            if(!isRecording && pointRecorder.numRows>0 && currentFrame > 0)
            {
                // go back in time
                currentFrame--;
                
                // parse the data in to pointData
                pointData.clear();
                vector <string> s = ofSplitString(pointRecorder.getString(currentFrame, 1), "&");
                
                for(int i = 0; i < s.size(); i++)
                {
                    if(!s[i].empty())
                    {
                        vector <string> ss = ofSplitString(s[i], ":");
                        pointData.push_back( ofPoint(ofToFloat(ss[0]),
                                                     ofToFloat(ss[1])) );
                    }
                }
                
                // update the triangle mesh
                updateTriangulation();
            }
            
            break;
            
        case OF_KEY_RIGHT:
           
            if(!isRecording && pointRecorder.numRows>0 && currentFrame < pointRecorder.numRows-1)
            {
                // go forward in time
                currentFrame++;
                
                // parse the data in to pointData
                pointData.clear();
                vector <string> s = ofSplitString(pointRecorder.getString(currentFrame, 1), "&");
                
                for(int i = 0; i < s.size(); i++)
                {
                    if(!s[i].empty())
                    {
                        vector <string> ss = ofSplitString(s[i], ":");
                        pointData.push_back( ofPoint(ofToFloat(ss[0]),
                                                     ofToFloat(ss[1])) );
                    }
                }
                
                // update the triangle mesh
                updateTriangulation();
            }
            
            
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(isRecording)
    {
        createPointNear(x, y, scatter);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}