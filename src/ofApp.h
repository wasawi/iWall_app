#pragma once

#include "ofMain.h"

#include "kinect.h"
#include "appSwitch.h"
#include "ofxTimer.h"

class ofApp : public ofBaseApp {

public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
	ofxTimer		timer;
	kinect			knect;
	appSwitch		apps;
	
	void switcher(float &e);
	void manualSwitcher(string &e);
};
