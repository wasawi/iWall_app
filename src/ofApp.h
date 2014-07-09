#pragma once

#include "ofMain.h"

#include "kinect.h"
#include "ofxVLCRemote.h"
#include "ofxTimer.h"

class ofApp : public ofBaseApp {

public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

	void openBlender();
	void openVLC();
	void focusBlender();
	void focusVLC();
	
	ofxVLCRemote	vlc;
	ofxTimer		timer;
	kinect			kin;
	bool isBlenderOnFocus;
};
