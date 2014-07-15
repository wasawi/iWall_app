#include "appSwitch.h"

//--------------------------------------------------------------
void appSwitch::setup(){
	isBlenderOnFocus=true;
	isOnInit = true;
	username = getUsername();
	
	// setup VLC
//	openVLC();
	vlc.setup("/Users/"+username+"/vlc.sock");
	
	// setup Blender
//	openBlender();
	
	//	timer.setup(20000, false);
	//	timer.startTimer();
}

//--------------------------------------------------------------
void appSwitch::switchApp(){
	if (isBlenderOnFocus){
		switchToVLC();
	}else{
		switchToBlender();
	}
}

//--------------------------------------------------------------
void appSwitch::switchToBlender(){
	if (!isBlenderOnFocus){
		cout << "****** focus Blender" << endl;
		stopVLC();
		focusBlender();
		isBlenderOnFocus= true;
	}
}

//--------------------------------------------------------------
void appSwitch::switchToVLC(){
	if (isBlenderOnFocus){
		if (isOnInit){
			cout << "****** init VLC" << endl;
			isOnInit=false;
			initVLC();
		}
		cout << "****** focus VLC" << endl;
		stopBlender();
		focusVLC();
		isBlenderOnFocus= false;
	}
}

//--------------------------------------------------------------
void appSwitch::update(){
	if (timer.isTimerFinished()){
		
		switchApp();
		
		// for testing keep switching
		timer.setup(10000, false);
		timer.startTimer();
	}
}

//--------------------------------------------------------------
void appSwitch::openBlender(){
//	string iWallApp="/Users/"+username+"/Desktop/iWall/soft/Blender/blender.app/Contents/MacOS/blender /Users/"+username+"/Desktop/iWall/BlenderFiles/Hallway_01.blend -noaudio -W";
	string iWallApp="open /Users/"+username+"/Desktop/iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void appSwitch::focusBlender(){
	cout << "****** resume blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -CONT &");
	
	string iWallApp="open -a Hallway_01 &";
//	string iWallApp="open -a blender &";
//	string iWallApp="open /Users/"+username+"/Desktop/iWall/BlenderFiles/Hallway_01.app &";
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
	vlc.run("fullscreen");
	
	string vlcPath="open -a VLC &";
	ofSystem(vlcPath);
}
//--------------------------------------------------------------
void appSwitch::stopVLC(){
	vlc.run("pause");
}

string appSwitch::getUsername(){
	string username = "";
	username = ofSystem("whoami");
	vector <string> cleanString;
	cleanString = ofSplitString(username, "\n");
	username = cleanString[0];
	ofLogNotice() << "username = /"<< username<<"/";
	
	return username;
}

//--------------------------------------------------------------
void appSwitch::exit() {

	ofSystem("pkill -9 VLC");
	ofSystem("pkill -9 blenderplayer");
	
}


