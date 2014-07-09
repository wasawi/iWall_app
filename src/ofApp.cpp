#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	isBlenderOnFocus=true;
	isOnInit = true;
	
	openVLC();
	vlc.setup("/Users/ntnu/vlc.sock");
	
	timer.setup(20000, false);
	timer.startTimer();
	
	openBlender();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);
	
//	kin.update();
	
	if (timer.isTimerFinished()){
		if (isOnInit){
			cout << "****** init VLC" << endl;
			initVLC();
			isOnInit=false;
		}
		
		if (isBlenderOnFocus){
			stopBlender();
			focusVLC();
			isBlenderOnFocus= false;
			
		}else{
			stopVLC();
			focusBlender();
			isBlenderOnFocus= true;
		}

		timer.setup(20000, false);
		timer.startTimer();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

//	kin.draw();
}

//--------------------------------------------------------------
void ofApp::openBlender(){
//	string iWallApp="/Users/ntnu/Desktop/iWall/soft/Blender/blender.app/Contents/MacOS/blender /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.blend -noaudio -W";
	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void ofApp::focusBlender(){
	cout << "****** resume blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -CONT &");
	
	string iWallApp="open -a Hallway_01 &";
//	string iWallApp="open -a blender &";
//	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void ofApp::stopBlender(){
	cout << "****** stop blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -STOP &");
}

//--------------------------------------------------------------
void ofApp::initVLC(){
	string dataPath="../../../data/";
	vlc.run("add "+dataPath+"bball.mov");
	
	vlc.run("loop");
	vlc.run("play");
	vlc.run("fullscreen");
	vlc.run("pause");
}

//--------------------------------------------------------------
void ofApp::openVLC(){
	string vlcPath="/Applications/VLC.app/Contents/MacOS/VLC --fullscreen --noaudio &";
	ofSystem(vlcPath);
}
//--------------------------------------------------------------
void ofApp::focusVLC(){
	vlc.run("pause");
	
	string vlcPath="open -a VLC &";
	ofSystem(vlcPath);
}
//--------------------------------------------------------------
void ofApp::stopVLC(){
	vlc.run("pause");
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	kin.keyPressed(key);
	
	switch (key){
			
		case 'a':{
		}break;
			
		case 's':{
		}break;

		case 'i':{
		}break;

	}
}

//--------------------------------------------------------------
void ofApp::exit() {

	ofSystem("pkill -9 VLC");
	ofSystem("pkill -9 blenderplayer");
	
}
