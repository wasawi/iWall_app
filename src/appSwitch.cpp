#include "appSwitch.h"

//--------------------------------------------------------------
void appSwitch::setup(){
	isBlenderOnFocus=true;
	isOnInit = true;
	username = getUsername();
	fileCount = dir.listDir("/SMBShare/videos");

	// setup Blender
	openBlender();
		
	// setup VLC
	openVLC();
	vlc.setup("/Users/"+username+"/vlc.sock");
	
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

}

//--------------------------------------------------------------
void appSwitch::openBlender(){
//	string iWallApp="/Users/"+username+"/Desktop/iWall/soft/Blender/blender.app/Contents/MacOS/blender /Users/"+username+"/Desktop/iWall/BlenderFiles/Hallway_01.blend -noaudio -W";
	string iWallApp="open /iWall/BlenderFiles/Hallway_01.app &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void appSwitch::focusBlender(){
	cout << "****** resume blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -CONT &");
	
	// focus on BLender app
	string iWallApp="open -a Hallway_01 &";
	ofSystem(iWallApp);
}
//--------------------------------------------------------------
void appSwitch::stopBlender(){
	cout << "****** stop blender" << endl;
	cout << ofSystem("ps aux | grep blenderplayer | awk {'print $2'} | xargs KILL -STOP &");
}

//--------------------------------------------------------------
void appSwitch::initVLC(){
	vlc.run("clear");
	
	ofLogNotice("video file count ")<< fileCount;
	
	for (int i =0; i < fileCount; i++) {
		string addfile="add "+dir.getPath(i);
		ofLogNotice("VLC")<< addfile;
		vlc.run(addfile);
	}
	
	vlc.run("goto 1");
	vlc.run("menu off");
	vlc.run("loop");
	vlc.run("play");
	vlc.run("pause");
}

//--------------------------------------------------------------
void appSwitch::openVLC(){
//	string vlcPath="open /iWall/soft/VLC.app/Contents/MacOS/VLC --args";
//	string vlcPath="/iWall/soft/VLC.app/Contents/MacOS/VLC";
	
	string vlcPath="open -a VLC --args";
	
	string options;
	
	options+=" --no-video-title-show";
	options+=" --no-video-deco";
	options+=" --mouse-hide-timeout=1000";
	options+=" --fullscreen";
	options+=" --no-audio";

//	following do not work:
	//	options+=" --no-mouse-events";
	//	options+=" --no-keyboard-events";
	//	options+="-I rc";
	//	options+=" -I dummy";
	//	options+=" --qt-minimal-view";
	//	options+=" --rc-quiet";
	//	options+=" --one-instance";
	
	ofLogNotice("VLC")<< vlcPath+options;
	ofSystem(vlcPath+options+" &");
}

//--------------------------------------------------------------
void appSwitch::focusVLC(){

//	vlc.run("fullscreen on");

	
	// focus on VLC app
	string vlcPath="open -a VLC";

	// this will open a new instance each time... no good.
//	string vlcPath="/iWall/soft/VLC.app/Contents/MacOS/VLC";
	ofSystem(vlcPath+" &");
	
	
	vlc.run("pause");
	vlc.run("f on");

//	vlc.run("fullscreen on");
//	vlc.run("fullscreen on");
}
//--------------------------------------------------------------
void appSwitch::stopVLC(){
	vlc.run("pause");
	vlc.run("f off");
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


