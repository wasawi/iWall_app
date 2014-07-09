#pragma once

#include "ofMain.h"

#include "ofxVLCRemote.h"
#include "ofxTimer.h"

class appSwitch : public ofBaseApp {

public:
	
	void setup();
	void update();
	void exit();

	void openBlender();
	void focusBlender();
	void stopBlender();
	
	void initVLC();
	void openVLC();
	void focusVLC();
	void stopVLC();
	
	void switchToBlender();
	void switchToVLC();
	
	ofxVLCRemote	vlc;
	ofxTimer		timer;
	bool			isBlenderOnFocus;
	bool			isOnInit;
};
