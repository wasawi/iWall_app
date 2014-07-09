#include "appSwitch.h"

//--------------------------------------------------------------
void appSwitch::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	isBlenderOnFocus=true;
	isOnInit = true;
	
	openVLC();
	vlc.setup("/Users/ntnu/vlc.sock");
	
	timer.setup(10000, false);
	timer.startTimer();
	
	openBlender();
	ofSetLogLevel(OF_LOG_NOTICE);
}

//--------------------------------------------------------------
void appSwitch::switchToBlender(){
	if (!isBlenderOnFocus){
	stopVLC();
	focusBlender();
	isBlenderOnFocus= true;
	}
}
//--------------------------------------------------------------
void appSwitch::switchToVLC(){
	if (isBlenderOnFocus){
		stopBlender();
		focusVLC();
		isBlenderOnFocus= false;
	}
}

//--------------------------------------------------------------
void appSwitch::update(){
	if (timer.isTimerFinished()){
		if (isOnInit){
			cout << "****** init VLC" << endl;
			isOnInit=false;
			initVLC();
		}
	}
}

//--------------------------------------------------------------
void appSwitch::openBlender(){
//	string iWallApp="/Users/ntnu/Desktop/iWall/soft/Blender/blender.app/Contents/MacOS/blender /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.blend -noaudio -W";
	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void appSwitch::focusBlender(){
	cout << "****** resume blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -CONT &");
	
	string iWallApp="open -a Hallway_01 &";
//	string iWallApp="open -a blender &";
//	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void appSwitch::stopBlender(){
	cout << "****** stop blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -STOP &");
}

//--------------------------------------------------------------
void appSwitch::initVLC(){
	string dataPath="../../../data/";
	vlc.run("add "+dataPath+"bball.mov");
	
	vlc.run("loop");
	vlc.run("play");
	vlc.run("fullscreen");
	vlc.run("pause");
}

//--------------------------------------------------------------
void appSwitch::openVLC(){
	string vlcPath="/Applications/VLC.app/Contents/MacOS/VLC --fullscreen --noaudio &";
	ofSystem(vlcPath);
}
//--------------------------------------------------------------
void appSwitch::focusVLC(){
	vlc.run("pause");
	
	string vlcPath="open -a VLC &";
	ofSystem(vlcPath);
}
//--------------------------------------------------------------
void appSwitch::stopVLC(){
	vlc.run("pause");
}

//--------------------------------------------------------------
void appSwitch::exit() {

	ofSystem("pkill -9 VLC");
	ofSystem("pkill -9 blenderplayer");
	
}
