//
//  ofxMackerel.cpp
//
//  Copyright 2015 asus4
//
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    #warning set your API KEY
    // https://mackerel.io/orgs/YOUR_ORGANIZATION?tab=apikeys
    mackerel.setup("****");
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update() {
    mackerel.queueMetrics("custom.framerate", ofGetFrameRate());
    mackerel.queueMetrics("custom.testnumber", ofRandom(0, 100));

    if (ofGetFrameNum() - counter > 1) {
        std::cout << ofGetFrameNum() - counter << std::endl;
    }
    counter = ofGetFrameNum();
}

//--------------------------------------------------------------
void testApp::draw() {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}
