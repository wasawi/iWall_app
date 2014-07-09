#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	knect.setup();

	apps.setup();
	
	//events
	ofAddListener(knect.trigger,					//the ofEvent that we want to listen to.
				  this,								//pointer to the class that is going to be listening.
				  &ofApp::switcher);				//pointer to the method that's going to be called when a new event is broadcasted (callback method).

}


//--------------------------------------------------------------
void ofApp::switcher(string &e){

	if (e=="gotoBlender"){
		cout << "GO TO BLENDER"<< endl;
		apps.switchToBlender();
	}

	if (e=="gotoVLC"){
		cout << "GO TO BLENDER"<< endl;
		apps.switchToVLC();
	}

}


//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);
	
	knect.update();
	
	apps.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	knect.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	knect.keyPressed(key);
	
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
	knect.exit();
	apps.exit();
}
