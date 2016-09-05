//
//  ofxLaunchpad2Controls.hpp
//  launchpadTest
//
//  Created by Henry Betts on 08/06/2016.
//
//

#ifndef ofxLaunchpad2Controls_hpp
#define ofxLaunchpad2Controls_hpp

#include <stdio.h>
#include "ofxLaunchpad2.hpp"

//// Control Component ////
class ofxLaunchpad2ControlComponent{
    
public:
    ofxLaunchpad2ControlComponent(unsigned x, unsigned y){_x=x;_y=y;};
    ofxLaunchpad2ControlComponent(unsigned x, unsigned y, ofColor _color){_x=x;_y=y;color=_color;};
    
    unsigned getX(){return _x;};
    unsigned getY(){return _y;};
    ofColor color;
    
protected:
    unsigned _x, _y;
    
};

//// Base Control ////
class ofxLaunchpad2ControlBase {
    
    friend ofxLaunchpad2;
    
public:
    virtual ~ofxLaunchpad2ControlBase();
    
    bool isInitialized(){return _isInitialized;};
    
protected:
    
    ofxLaunchpad2ControlBase(){};
    
    bool init(vector<ofxLaunchpad2ControlComponent> components);
    bool init(ofxLaunchpad2ControlComponent component);
    bool init(unsigned x, unsigned y, ofColor color);
    bool init(unsigned x, unsigned y, unsigned width, unsigned height, ofColor color);
    
    const vector<ofxLaunchpad2ControlComponent*> & getComponents(){return _components;};
    
    void update();
    virtual void receivedLaunchpadEvent(ofxLaunchpad2Event &event) = 0;
    
    
private:
    void _attach(ofxLaunchpad2 *launchpad);
    virtual void _detach();
    
    ofxLaunchpad2 *_launchpad;
    vector<ofxLaunchpad2ControlComponent*> _components;
    bool _isInitialized = false;
    
};



//// Push Control ////
class ofxLaunchpad2Control : public ofxLaunchpad2ControlBase{
    
public:
    ofxLaunchpad2Control(){};
    //ofxLaunchpad2Control(unsigned row, unsigned col, ofColor offColor, ofColor onColor);
    
    bool isOn(){return _isOn;};
    
    bool init(unsigned x, unsigned y, ofColor offColor, ofColor onColor);
    
protected:
    bool _isOn = false;
    ofColor _offColor, _onColor;
    
    virtual void receivedLaunchpadEvent(ofxLaunchpad2Event &event);
    
    
};

//// Indicator Control ////
class ofxLaunchpad2IndicatorControl : public ofxLaunchpad2Control{
    
public:
    ofxLaunchpad2IndicatorControl(){};
    
    bool init(unsigned x, unsigned y, ofColor offColor, ofColor onColor, bool isOn);
    
    void setIsOn(bool isOn);
    
protected:
    virtual void receivedLaunchpadEvent(ofxLaunchpad2Event &event){};
    
};


//// Toggle Control ////
class ofxLaunchpad2ToggleControl : public ofxLaunchpad2IndicatorControl{
    
public:
    ofxLaunchpad2ToggleControl(){};
    
protected:
    virtual void receivedLaunchpadEvent(ofxLaunchpad2Event &event);
    
};


//// Multi Toggle Control ////
class ofxLaunchpad2GroupToggleControl : public ofxLaunchpad2ControlBase{
    
public:
    
    ofxLaunchpad2GroupToggleControl(){};

    bool init(vector<ofVec2f> positions, ofColor offColor, ofColor onColor, unsigned selected);
    bool init(unsigned x, unsigned y, unsigned width, unsigned height, ofColor offColor, ofColor onColor, unsigned selected);
    
    unsigned getSelected(){return _selected;};
    void setSelected(unsigned selected);

    
protected:
    
    void receivedLaunchpadEvent(ofxLaunchpad2Event & event);
    
    unsigned _selected = 0;
    ofColor _onColor, _offColor;
    
};

#endif /* ofxLaunchpad2Controls_hpp */
