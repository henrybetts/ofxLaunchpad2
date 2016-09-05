//
//  ofxLaunchpad2.cpp
//  launchpadTest
//
//  Created by Henry Betts on 07/06/2016.
//
//

#include "ofxLaunchpad2.hpp"
#include "ofxLaunchpad2Controls.hpp"

ofxLaunchpad2::ofxLaunchpad2(){
    
    _midiIn.addListener(this);
    _pixels.allocate(9, 9, OF_PIXELS_RGB);
    
}

ofxLaunchpad2::~ofxLaunchpad2(){
    
    _midiIn.removeListener(this);
    
    for (ofxLaunchpad2ControlBase *control : _controls){
        control->_detach();
    }
    _controls.clear();
    
    close();
    _pixels.clear();
    
}

void ofxLaunchpad2::close(){
    
    clearPixels();
    _midiIn.closePort();
    _midiOut.closePort();
    _isOpen = false;
    
}

bool ofxLaunchpad2::open(){
    return open("Launchpad MK2");
}

bool ofxLaunchpad2::open(string name){
    
    close();
    
    if (!_midiOut.openPort(name)) return false;
    if (!_midiIn.openPort(_midiOut.getPort())){
        close();
        return false;
    }
    
    setup();
    _isOpen = true;
    
    return true;
    
}

bool ofxLaunchpad2::open(int port){
    
    close();
    
    if (!_midiOut.openPort(port)) return false;
    if (!_midiIn.openPort(port)){
        close();
        return false;
    }
    
    setup();
    _isOpen = true;
    
    return true;
    
}

void ofxLaunchpad2::setup(){
    
    enterSessionMode();
    clearPixels();
    
    for (ofxLaunchpad2ControlBase *control : _controls){
        control->update();
    }
    
}

void ofxLaunchpad2::setPixels(ofPixels & pixels){
    
    if (!_isOpen) return;
    
    //data is split into groups of 16, as core midi seems to fail if the messages are too large
    
    bool startedMessage = false;
    unsigned msgSize = 0;
    
    for (int x=0; x<9; x++){
        
        for (int y=0; y<9; y++){
            
            if (!(x==8 && y==8)){
                
                ofColor color = pixels.getColor(x, y);
                
                if (color != _pixels.getColor(x, y)){

                    if (startedMessage && msgSize >= 16){
                        startedMessage = false;
                        _midiOut << FinishMidi();
                    }
                    
                    if (!startedMessage){
                        startedMessage = true;
                        msgSize = 0;
                        _midiOut << StartMidi();
                    }
                    
                    msgSize++;
                    sendPixelMessage(x, y, color);
                    _pixels.setColor(x, y, color);
                
                }
                
            }
            
        }
        
    }
    
    if (startedMessage){
        _midiOut << FinishMidi();
    }
    
}

ofColor ofxLaunchpad2::getPixel(unsigned x, unsigned y){
    
    if (x > 8){
        ofLogError("ofxLaunchpad2", "x position cannot be greater than 8.");
        return ofColor::black;
    }
    
    if (y > 8){
        ofLogError("ofxLaunchpad2", "y position cannot be greater than 8.");
        return ofColor::black;
    }
    
    if (x == 8 && y == 8){
        ofLogError("ofxLaunchpad2", "There is no pixel at 8,8!");
        return ofColor::black;
    }
    
    return _pixels.getColor(x, y);
    
}

void ofxLaunchpad2::setPixel(unsigned x, unsigned y, ofColor color){
    
    if (x > 8){
        ofLogError("ofxLaunchpad2", "x position cannot be greater than 8.");
        return;
    }
    
    if (y > 8){
        ofLogError("ofxLaunchpad2", "y position cannot be greater than 8.");
        return;
    }
    
    if (x == 8 && y == 8){
        ofLogError("ofxLaunchpad2", "There is no pixel at 8,8!");
        return;
    }
    
    if (color != _pixels.getColor(x, y)){
        
        sendPixelMessage(x, y, color);
        
    }
    
    _pixels.setColor(x, y, color);
    
}

void ofxLaunchpad2::clearPixels(){
    
    _pixels.setColor(ofColor::black);
    
    //if (needsUpdating){
    sendClearMessage();
    //}
    
}

void ofxLaunchpad2::addListener(ofxLaunchpad2Listener *listener){
    
    ofAddListener(_newMessageEvent, listener, &ofxLaunchpad2Listener::receivedLaunchpadEvent);
    
}

void ofxLaunchpad2::removeListener(ofxLaunchpad2Listener *listener){
    
    ofRemoveListener(_newMessageEvent, listener, &ofxLaunchpad2Listener::receivedLaunchpadEvent);
    
}

void ofxLaunchpad2::addControl(ofxLaunchpad2ControlBase *control){
    _controls.push_back(control);
    control->_attach(this);
}

void ofxLaunchpad2::removeControl(ofxLaunchpad2ControlBase *control){
    
    vector<ofxLaunchpad2ControlBase*>::iterator pos = find(_controls.begin(), _controls.end(), control);
    if (pos != _controls.end()){
        _controls.erase(pos);
        control->_detach();
    }
    
}

void ofxLaunchpad2::removeAllControls(){
    
    for (ofxLaunchpad2ControlBase *control : _controls){
        control->_detach();
    }
    _controls.clear();
    
}

void ofxLaunchpad2::newMidiMessage(ofxMidiMessage& msg){
    
        
    ofxLaunchpad2Event event;
    
    if (getEvent(event, msg)){
        
        ofNotifyEvent(_newMessageEvent, event, this);
        
        vector<ofxLaunchpad2ControlBase*> copy = vector<ofxLaunchpad2ControlBase*>(_controls);
        
        for (ofxLaunchpad2ControlBase *control : copy){
            
            if ( find(_controls.begin(), _controls.end(), control) != _controls.end() )
            control->receivedLaunchpadEvent(event);
            
        }
        
    }
    
}


bool ofxLaunchpad2::getEvent(ofxLaunchpad2Event &event, ofxMidiMessage msg){
    
    if (msg.status == MIDI_CONTROL_CHANGE && msg.control >=104 && msg.control <= 111){
        
        event.y = 8;
        event.x = msg.control - 104;
        event.isPressed = msg.value == 127;
        
    }else if (msg.status == MIDI_NOTE_ON && msg.pitch >= 11 && msg.pitch <= 89 && msg.pitch % 10 != 0){
        
        event.y = (msg.pitch / 10) - 1;
        event.x = (msg.pitch - ((event.y+1)*10)) - 1;
        event.isPressed = msg.velocity == 127;
        
    }else{
        return false;
    }
    
    event.midiMessage = msg;
    return true;
    
}

void ofxLaunchpad2::enterSessionMode(){
    
    vector<unsigned char> bytes = {0x22, 0x00};
    sendSysexMessage(bytes);
    
}

void ofxLaunchpad2::sendClearMessage(){
    
    sendSysexMessage({0x0E, 0x00});
    
}

void ofxLaunchpad2::sendPixelMessage(unsigned int x, unsigned int y, ofColor color){
    
    if (x > 8) return;
    if (y > 8) return;
    if (x == 8 && y == 8) return;
    
    unsigned char r,g,b;
    r = ofMap(color.r, 0, 255, 0, 63);
    g = ofMap(color.g, 0, 255, 0, 63);
    b = ofMap(color.b, 0, 255, 0, 63);
    
    unsigned char pitch = y==8 ? 104 + x : 11+(y*10)+x;
    
    vector<unsigned char> bytes = {0x0B, pitch, r, g, b};
    sendSysexMessage(bytes);
    
}

void ofxLaunchpad2::sendSysexMessage(vector<unsigned char> body){
    
    if (_isOpen){
        
        vector<unsigned char> bytes = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x18}; //header
        bytes.insert(bytes.end(), body.begin(), body.end()); //body
        bytes.insert(bytes.end(), 0xF7); //footer
        
        _midiOut.sendMidiBytes(bytes);
        
    }else{
        // warning?
    }
    
}