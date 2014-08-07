#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(30);
	
	// VLC and Blender manager
	apps.setup();

	//kinect
	knect.setup();
	knect.enableOSC(true);
	
	//events
	// manual switch
	ofAddListener(knect.trigger,					//the ofEvent that we want to listen to.
				  this,								//pointer to the class that is going to be listening.
				  &ofApp::manualSwitcher);			//pointer to the method that's going to be called when a new event is broadcasted (callback method).

	// gate switch
	ofAddListener(knect.gate.gateEvent,				//the ofEvent that we want to listen to.
				  this,								//pointer to the class that is going to be listening.
				  &ofApp::switcher);				//pointer to the method that's going to be called when a new event is broadcasted (callback method).

	// a timer to switch to VLC
//	timer.setup(20000, false);
//	timer.startTimer();
//	ofAddListener(timer.TIMER_REACHED, this, &ofApp::getNotification);

	
	ofSetLogLevel(OF_LOG_NOTICE);
}

//--------------------------------------------------------------
void ofApp::getNotification(unsigned int &e){
	ofLog()<< "timer reached count =" << ofToString(e);
	apps.switchToVLC();
}

//--------------------------------------------------------------
void ofApp::switcher(float &e){
	
	if (knect.enableSwitch){
		if (e>0){
			ofLogNotice("switcher") << "GO TO BLENDER"<< endl;
			apps.switchToBlender();
			knect.enableOSC(true);
		}
		
		else{
			ofLogNotice("switcher") << "GO TO VLC"<< endl;
			apps.switchToVLC();
			knect.enableOSC(false);
		}
	}
}


//--------------------------------------------------------------
void ofApp::manualSwitcher(string &e){

	if (e=="gotoBlender"){
		cout << "GO TO BLENDER"<< endl;
		apps.switchToBlender();
		knect.enableOSC(true);
	}

	if (e=="gotoVLC"){
		cout << "GO TO VLC"<< endl;
		apps.switchToVLC();
		knect.enableOSC(false);
	}

}


//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(80);
	
	knect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	knect.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	knect.keyPressed(key);
	
	switch (key){
			
		case 'k':{
			// this works well
			ofKillApp("iWall");
		}break;
			
		case 's':{
			/*
			// wait 3 seconds before sending the shutdown command
			shutdown.setDelay(3);
			// shutdown command: shutdown in 1 minute (to let OF close well)
			shutdown.setParams("-h +10");
			// set admin password if user is not admin
			shutdown.setUserIsAdmin(false, "show");
			// close OF before shutting dowmn
			shutdown.exitOFAfterShutdownCommand(true);
			// execute
			shutdown.shutdownExecute();
			*/
		}break;

		case 'i':{
		}break;

			
	}
}


//--------------------------------------------------------------
void ofApp::listenEvent( ofxTimeEventData& data )
{
    ofLogNotice("listenEvent") << "TIME EVENT DISPATCHED" <<
	" hour:" << data.hour<<
	" minute:" << data.minutes<<
	" second:" << data.second<<	endl;

	// attention. shutDown does not work.
	// we will only close the app and do the shutdown in system preferences.
	
	/*
	// wait 3 seconds before sending the shutdown command
	shutdown.setDelay(3);
	// shutdown command: shutdown in 1 minute (to let OF close well)
	shutdown.setParams("-h +10");
	// set admin password if user is not admin
	shutdown.setUserIsAdmin(false, "show");
	// close OF before shutting dowmn
	shutdown.exitOFAfterShutdownCommand(true);
	// execute
	shutdown.shutdownExecute();
	 */
	
	ofKillApp("iWall");
}


//--------------------------------------------------------------
void ofApp::exit() {

	apps.exit();
//	knect.exit();
	// kinect is not closing well, ugly fix here:
	ofKillApp("iWall");
}


