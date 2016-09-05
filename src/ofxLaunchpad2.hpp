//
//  ofxLaunchpad2.hpp
//  launchpadTest
//
//  Created by Henry Betts on 07/06/2016.
//
//

#ifndef ofxLaunchpad2_hpp
#define ofxLaunchpad2_hpp

#include <stdio.h>
#include "ofxMidi.h"
#include "ofPixels.h"

class ofxLaunchpad2ControlBase;

class ofxLaunchpad2Event : ofEventArgs{
public:
    ofxMidiMessage midiMessage;
    unsigned x;
    unsigned y;
    bool isPressed;
};

class ofxLaunchpad2Listener{
public:
    virtual void receivedLaunchpadEvent(ofxLaunchpad2Event &event) = 0;
};

class ofxLaunchpad2 : public ofxMidiListener{
    
public:
    
    ofxLaunchpad2();
    ~ofxLaunchpad2();
    
    bool open();
    bool open(int port);
    bool open(string name);
    
    void close();
    
    bool isOpen(){return _isOpen;};
    
    ofPixels getPixels(){return _pixels;};
    void setPixels(ofPixels & pixels);
    
    ofColor getPixel(unsigned x, unsigned y);
    void setPixel(unsigned x, unsigned y, ofColor color);
    
    void clearPixels();
    
    void addListener(ofxLaunchpad2Listener* listener);
    void removeListener(ofxLaunchpad2Listener* listener);
    
    void addControl(ofxLaunchpad2ControlBase *control);
    void removeControl(ofxLaunchpad2ControlBase *control);
    void removeAllControls();
    vector<ofxLaunchpad2ControlBase*> getControls(){return _controls;};
    
    void newMidiMessage(ofxMidiMessage& msg);
    
protected:
    
    void setup();
    
    //midi messages
    void enterSessionMode();
    void sendClearMessage();
    void sendPixelMessage(unsigned x, unsigned y, ofColor color);
    void sendSysexMessage(vector<unsigned char> body);
    
    bool getEvent(ofxLaunchpad2Event &event, ofxMidiMessage msg);
    
    ofEvent<ofxLaunchpad2Event> _newMessageEvent;
    
    vector<ofxLaunchpad2ControlBase*> _controls;
    
    ofxMidiIn _midiIn;
    ofxMidiOut _midiOut;
    
    bool _isOpen = false;
    
    ofPixels _pixels;
    
};

#endif /* ofxLaunchpad2_hpp */
