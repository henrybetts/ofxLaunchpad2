#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundAuto(false);
    ofBackground(0);
    
    //allocate frame buffer
    fbo.allocate(9, 9);
    
    //allocate pixel buffer
    pixels.allocate(9, 9, OF_PIXELS_RGB);
    pixels.setColor(ofColor::black);
    
    //attempt to open launchpad
    if (launchpad.open()){
        ofLog(OF_LOG_NOTICE, "Opened launchpad!");
    }else{
        ofLog(OF_LOG_ERROR, "Failed to open launchpad");
    }

}

//--------------------------------------------------------------
void ofApp::update(){

    //increase the size of the circle
    size += 0.01;
    
    if (size > 1){
        //start a new circle
        size = 0;
        color.setHsb(ofRandom(255), ofRandom(100, 255), ofRandom(100, 255));
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    if (launchpad.isOpen()){
    
        //begin frame buffer
        fbo.begin();
        
        //draw circle
        ofSetColor(color);
        ofDrawCircle(4.5, 4.5, size*9);
        
        //end frame buffer
        fbo.end();
        
        //get pixels from frame buffer and send to launchpad
        fbo.readToPixels(pixels);
        launchpad.setPixels(pixels);
        
    }
    
    //draw circle on main display as well
    ofSetColor(color);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, size*ofGetWidth());
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
