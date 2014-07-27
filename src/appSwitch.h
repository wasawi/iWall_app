#pragma once

#include "ofMain.h"

#include "ofxVLCRemote.h"
#include "ofxTimer.h"

class appSwitch {

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
	
	void switchApp();
	
	string getUsername();
	
	ofxVLCRemote	vlc;

	bool			isBlenderOnFocus;
	bool			isOnInit;
	
	string			username;
	
	ofDirectory		dir;
	int				fileCount;


};
