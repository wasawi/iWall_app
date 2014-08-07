#pragma once

#include "ofMain.h"

#include "kinect.h"
#include "appSwitch.h"
#include "ofxTimer.h"
#include "ofxTimeEvent.h"
#include "ofxShutdown.h"

class ofApp : public ofBaseApp {

public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
	kinect			knect;
	appSwitch		apps;
	
	void switcher(float &e);
	void manualSwitcher(string &e);
	
	
	ofxTimer		timer;
	void getNotification(unsigned int &e);
	

	// scheduled shut down
//	ofxTimeEvent	*event;
	void listenEvent( ofxTimeEventData& data );
	ofxShutdown		shutdown;
	
	
	
	
	//__________________________________________
	// a scheduled event to close everything (not used)
	//	event = ofxTimeEvent::getInstance();
	//	event->registerSec(1, 30);
	/*
	 // event every even minute
	 for (int i=0;i<30;i++){
	 event->registerMin(i+1, i*2);
	 }*/
	//	event->registerHourMin(1, 17, 8);
	//	ofAddListener( event->eventData, this, &ofApp::listenEvent);

	
	void ofKillApp(string appname){
		
		/*
		 http://www.linux.org/threads/kill-commands-and-signals.4423/
		 
		 SIGHUP (1) - Hangup detected on controlling terminal or death of controlling process. Use SIGHUP to reload configuration files and open/close log files.
		 SIGKILL (9) - Kill signal. Use SIGKILL as a last resort to kill process. This will not save data or cleaning kill the process.
		 SIGTERM (15) - Termination signal. This is the default and safest way to kill process.
		 */
		
		ofLogNotice("ofKillApp()")<<"killing "+ appname;
		ofSystem("ps aux | grep "+appname+" | awk {'print $2'} | xargs KILL -9");
		
	}
};
