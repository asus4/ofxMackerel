ofxMackerel
================

The simple wrapper for mackerel.io  
https://mackerel.io/

Tested on openFrameworks v0.9.3 OSX

USAGE
--------
```cpp
// testApp.h
#include "ofxMackerel.hpp"
ofxMackerel mackerel;
```

```cpp****
// testApp.cpp
//--------------------------------------------------------------
void testApp::setup() {
    #warning set your API KEY
    mackerel.setup("****");
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update() {
    mackerel.queueMetrics("custom.framerate", ofGetFrameRate());
    mackerel.queueMetrics("custom.testnumber", ofRandom(0, 100));
}

```


LISENCE
--------
MIT
