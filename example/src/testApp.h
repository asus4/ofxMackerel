//
//  ofxMackerel.hpp
//
//  Copyright 2015 asus4
//
//

#pragma once

#include "ofMain.h"
#include "ofxMackerel.hpp"

class testApp : public ofBaseApp {
 public:
    void setup();
    void update();
    void draw();

    void mouseMoved(int x, int y);
 private:
    ofxMackerel mackerel;
    int counter;
};
