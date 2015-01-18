#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
	
	soundStream.listDevices();
    
    int bufferSize = 256;
	
	volHistory.assign(400, 0.0);
    
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
    triggerVol      = 0.03;
    doTrigger       = false;
    scaleSensitivity          = 0.04;
    
    // 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    //lets scale the vol up to a 0-1 range
	scaledVol = ofMap(smoothedVol, 0.0, scaleSensitivity, 0.0, 1.0, true);
    
    if (scaledVol < triggerVol && doTrigger) {
        doTrigger = false;
    }
    else if (scaledVol >= triggerVol && !doTrigger) {
        doTrigger = true;
    }
    
	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(225);
    ofDrawBitmapString("'s' to unpause the audio\n'e' to pause the audio\n'up'/'down' to change trigger volume\ndrag to change inputscale", 20, 92);
    
    ofNoFill();
    
    // draw the average volume:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(20, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 400);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);
    
    // draw the trigger point on the graph
    ofSetColor(255);
    ofNoFill();
    ofLine(0, 400 - triggerVol * 400, 400, 400 - triggerVol * 400);
    ofPopMatrix();
	ofPopStyle();
    
    // draw the trigger
    if (doTrigger) {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(500, 170);
        
        ofSetColor(245, 58, 135);
        ofFill();
        ofEllipse(200, 200, 100, 100);
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    // draw the force scale
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(500, 170);
    
    ofSetColor(255);
    ofRect(0, 0, 40, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofRect(0, 400-scaledVol*400, 40, scaledVol*400);
    
    ofPopMatrix();
    ofPopStyle();
    
    ofSetColor(225);
    ofDrawBitmapString("trigger volume = " + ofToString(triggerVol) + "\nsensitivity = " + ofToString(scaleSensitivity), 20, 600);
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    float left, right;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left	= input[i*2]*0.5;
		right	= input[i*2+1]*0.5;
        
		curVol += left * left;
		curVol += right * right;
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
    
    if( key == 357 ){
        triggerVol += 0.005;
    }
    
    if( key == 359 ){
        triggerVol -= 0.005;
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

    if (y > pMouseY) {
        scaleSensitivity = ofClamp(scaleSensitivity -= 0.001, 0.0001, 1.0);
    }
    else if (y < pMouseY) {
        scaleSensitivity += 0.001;
    }
    
    pMouseY = y;
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
