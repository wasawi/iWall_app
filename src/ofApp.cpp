#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	isBlenderOnFocus=false;

	openVLC();
	openBlender();
	vlc.setup("/Users/ntnu/vlc.sock");

	timer.setup(20000, false);
	timer.startTimer();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);
	
//	kin.update();

	if (timer.isTimerFinished()){
	
		if (isBlenderOnFocus){
		
			vlc.run("loop");
			vlc.run("play");
			
			focusVLC();
			isBlenderOnFocus= false;
			
		}else{
			vlc.run("stop");
			
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
	string iWallApp="/Users/ntnu/Desktop/iWall/soft/Blender/blender.app/Contents/MacOS/blender /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.blend -noaudio -W";
//	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void ofApp::openVLC(){
	string vlcPath="/Applications/VLC.app/Contents/MacOS/VLC --fullscreen --noaudio &";
	ofSystem(vlcPath);
}

//--------------------------------------------------------------
void ofApp::focusBlender(){
	string iWallApp="open -a blender &";
//	string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void ofApp::focusVLC(){
	string vlcPath="open -a VLC &";
	ofSystem(vlcPath);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	switch (key){
			
		case 'a':{
			string dataPath="../../../data/";
			vlc.run("add "+dataPath+"bball.mov");
			vlc.run("loop");
		}break;
			
		case 's':{
			vlc.run("stop");
		}break;

		case 'i':{
			string iWallApp="open /Users/ntnu/Desktop/iWall/BlenderFiles/Hallway_01.app &";
			ofSystem(iWallApp);
		}break;

	}
}

//--------------------------------------------------------------
void ofApp::exit() {

	ofSystem("pkill -9 VLC");
	ofSystem("pkill -9 blenderplayer");
	
}
