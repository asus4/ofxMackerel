//
//  ofxMackerel.cpp
//
//  Copyright 2015 asus4
//
//

#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main() {
    ofAppGlutWindow window;  // create a window
    ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
    ofRunApp(new testApp());  // start the app
}
