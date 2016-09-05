//
//  ofxLaunchpadMk2Controls.cpp
//  launchpadTest
//
//  Created by Henry Betts on 08/06/2016.
//
//

#include "ofxLaunchpad2Controls.hpp"

//// Base ////

bool ofxLaunchpad2ControlBase::init(vector<ofxLaunchpad2ControlComponent> components){
    
    if (_isInitialized){
        ofLogError("ofxLaunchpad2ControlBase", "Control can only be initialized once.");
        return false;
    }
    
    for (ofxLaunchpad2ControlComponent &component : components){
        
        if (component.getX() > 8 || component.getY() > 8 || (component.getX() == 8 && component.getY() == 8)){
            ofLogError("ofxLaunchpad2ControlBase", "Component position is out of range.");
            return false;
        }
        
        _components.push_back(new ofxLaunchpad2ControlComponent(component));
        
    }
    
    _isInitialized = true;
    return true;
    
}

bool ofxLaunchpad2ControlBase::init(ofxLaunchpad2ControlComponent component){
    vector<ofxLaunchpad2ControlComponent> components = {component};
    return init(components);
}

bool ofxLaunchpad2ControlBase::init(unsigned x, unsigned y, ofColor color){
    return init(ofxLaunchpad2ControlComponent(x, y, color));
}

bool ofxLaunchpad2ControlBase::init(unsigned x, unsigned y, unsigned width, unsigned height, ofColor color){
    
    vector<ofxLaunchpad2ControlComponent> components;
    
    for (int xx=x; xx<x+width; xx++){
    
        for (int yy=y; yy<y+height; yy++){

            components.push_back(ofxLaunchpad2ControlComponent(xx, yy, color));

        }
    }
    
    return init(components);
    
}

ofxLaunchpad2ControlBase::~ofxLaunchpad2ControlBase(){
    
    for (ofxLaunchpad2ControlComponent* component : _components){
        
        delete component;
        
    }
    
    _components.clear();
    
}

void ofxLaunchpad2ControlBase::update(){
    
    if (_launchpad){
    
        for (ofxLaunchpad2ControlComponent *component : _components){
            
            _launchpad->setPixel(component->getX(), component->getY(), component->color);
            
        }
        
    }
    
}

void ofxLaunchpad2ControlBase::_attach(ofxLaunchpad2 *launchpad){
    _launchpad = launchpad;
    update();
}

void ofxLaunchpad2ControlBase::_detach(){
    
    for (ofxLaunchpad2ControlComponent *component : _components){
        component->color = ofColor::black;
    }
    
    update();
    _launchpad = NULL;
}


//// Push Control ////
bool ofxLaunchpad2Control::init(unsigned x, unsigned y, ofColor offColor, ofColor onColor) {
    
    _offColor = offColor;
    _onColor = onColor;
    
    return ofxLaunchpad2ControlBase::init(x, y, offColor);
    
}

void ofxLaunchpad2Control::receivedLaunchpadEvent(ofxLaunchpad2Event &event){
    
    ofxLaunchpad2ControlComponent* component = getComponents()[0];
    
    if (event.x == component->getX() && event.y == component->getY()){
        _isOn = event.isPressed;
        component->color = _isOn ? _onColor : _offColor;
    }
    
    update();
    
}


//// Indicator Control ////
bool ofxLaunchpad2IndicatorControl::init(unsigned x, unsigned y, ofColor offColor, ofColor onColor, bool isOn){
    
    if (!ofxLaunchpad2Control::init(x, y, offColor, onColor)){
        return false;
    }
    
    setIsOn(isOn);
    
}

void ofxLaunchpad2IndicatorControl::setIsOn(bool isOn){
    
    if (isInitialized()){
    
        _isOn = isOn;
        getComponents()[0]->color = isOn ? _onColor : _offColor;
        update();
        
    }
    
    
}


//// Toggle Control ////
void ofxLaunchpad2ToggleControl::receivedLaunchpadEvent(ofxLaunchpad2Event &event){
    
    ofxLaunchpad2ControlComponent* component = getComponents()[0];
    
    if (event.isPressed && event.x == component->getX() && event.y == component->getY()){
        
        setIsOn(!_isOn);
        
    }
    
}


//// Group Toggle Control ////
bool ofxLaunchpad2GroupToggleControl::init(vector<ofVec2f> positions, ofColor offColor, ofColor onColor, unsigned int selected){
    
    vector<ofxLaunchpad2ControlComponent> components;
    
    for (ofVec2f & pos : positions){
        
        components.push_back(ofxLaunchpad2ControlComponent(pos.y, pos.x, offColor));
        
    }
    
    if (!ofxLaunchpad2ControlBase::init(components)){
        return false;
    }
    
    _offColor = offColor;
    _onColor = onColor;
    setSelected(selected);
    return true;
    
}

bool ofxLaunchpad2GroupToggleControl::init(unsigned x, unsigned y, unsigned width, unsigned height, ofColor offColor, ofColor onColor, unsigned selected){
    
    if (!ofxLaunchpad2ControlBase::init(x, y, width, height, offColor)){
        return false;
    }
    
    _offColor = offColor;
    _onColor = onColor;
    setSelected(selected);
    return true;
    
}

void ofxLaunchpad2GroupToggleControl::setSelected(unsigned selected){
    
    if (isInitialized()){
        
        if (selected >= getComponents().size()){
            ofLogError("ofxLaunchpad2GroupToggleControl", "Selected index is out of range.");
            return;
        }
        
        getComponents()[_selected]->color = _offColor;
        getComponents()[selected]->color = _onColor;
        
        _selected = selected;
        update();
        
    }
    
}

void ofxLaunchpad2GroupToggleControl::receivedLaunchpadEvent(ofxLaunchpad2Event &event){
    
    if (event.isPressed){
    
        for (int i=0; i<getComponents().size(); i++){
            
            ofxLaunchpad2ControlComponent* component = getComponents()[i];
            if (event.x == component->getX() && event.y == component->getY()){
                
                setSelected(i);
                return;
                
            }
            
        }
        
    }
    
}