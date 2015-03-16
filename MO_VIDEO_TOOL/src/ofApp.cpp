#include "ofApp.h"

// write our boolean remove function
bool shouldRemove(Node &n){
    if( n.outOfBounds() )return true;
    else return false;
}



//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    
    fingerMovie.loadMovie("movies/C.mp4");
    fingerMovie.setLoopState(OF_LOOP_NONE);
    fingerMovie.play();
    isPlaying = true;
    
    // in/out measurements
    outputWindow = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
    camWidth = fingerMovie.width;
    camHeight = fingerMovie.height;
    showHelp = true;
    
    // Setup GeometricStuff
    myAttractor.strength = -5;
    myAttractor.ramp = 0.1;
    a_speed = 1;
    myRepeller.strength = +5;
    myRepeller.ramp = 0.1;
    r_speed = 2;
    maxTriangleArea = 300;
    
    radius.addListener(this, &ofApp::radiusChanged);
    
    //GUI
    gui.setup();
    gui.setPosition(20, 20+camHeight+80);
    gui.add(radius.set( "radius", 300, 10, 900 ));

}

//--------------------------------------------------------------
void ofApp::exit(){
	radius.removeListener(this, &ofApp::radiusChanged);
}

void ofApp::radiusChanged(float &r){
    myAttractor.radius = r;
    myRepeller.radius = r;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (isPlaying && fingerMovie.getIsMovieDone()) {
        isPlaying = false;
        cout << "finished playing" << endl;
    }
    
    // ELSE
    
    fingerMovie.update();
    
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
    
    vector<ofPoint> points;
    
    
    // update all the nodes
    for (int i = 0; i < myNodes.size(); i++) {
        myAttractor.attract( &myNodes[i] );
        myNodes[i].update();
        points.push_back(myNodes[i].position);
    }
    
    ofRemove(myNodes, shouldRemove);
    
    
    // update the mesh with new points
    triangulation.reset();
    triangulation.addPoints(points);
    triangulation.triangulate();
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
    
    
    // draw geometry
    ofPushStyle();
    ofSetColor(200, 100, 200);
    ofNoFill();
    ofCircle( myAttractor.x, myAttractor.y, myAttractor.radius );
    ofCircle( myRepeller.x, myRepeller.y, myRepeller.radius );
    triangulation.triangleMesh.drawWireframe();
    ofPopStyle();
    
    
    if (showHelp) {
        //draw the help window
        ofSetColor(255);
        ofRect(0, 0, camWidth+40, ofGetHeight());
        fingerMovie.draw(20, 20, camWidth, camHeight);
        
        ofSetColor(0);
        ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/"+ofToString(fingerMovie.getTotalNumFrames()),20,20+camHeight+20);
        ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition()*fingerMovie.getDuration(),2) + "/"+ofToString(fingerMovie.getDuration(),2),20,20+camHeight+40);
        
        gui.draw();
        
        ofPushMatrix();
        ofTranslate(0, ofGetHeight()-160);
        ofDrawBitmapString("< reset video", 20, 60);
        ofDrawBitmapString("> end video", 20, 80);
        ofDrawBitmapString("n node", 20, 100);
        ofDrawBitmapString("x clear", 20, 120);
        ofDrawBitmapString("? toggle help", 20, 140);
        ofPopMatrix();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'n' :
            // insert 3 new points (i triangle)
            for (int i = 0; i < 3; i++) {
                Node n = Node();
                n.setPosition(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
                n.setVelocity(0,0);
                n.setBoundary( 0,0,ofGetWidth(),ofGetHeight() );
                n.setDamping(0.001);
                myNodes.push_back( n );
            }
            break;
            
        case 'x' :
            myNodes.clear();
            break;
            
        case '?' :
            showHelp = !showHelp;
            break;
            
        case OF_KEY_LEFT:
            // jump to beginning
            fingerMovie.setPosition(0);
            fingerMovie.play();
            isPlaying = true;
            cout << "reset" << endl;
            break;
            
        case OF_KEY_RIGHT:
            // jump to end
            fingerMovie.setPosition(1);
            isPlaying = true;
            cout << "end" << endl;
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
