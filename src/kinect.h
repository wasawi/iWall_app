#pragma once

#include "ofMain.h"

// COUNTOUR FINDER
#define		MAX_NUM_CONTOURS_TO_FIND	1

// official addons
#include "ofxGui.h"
#include "ofxOpenCv.h"				// computer vision library
#include "ofxOsc.h"					// Open Sound Control communication library
#include "ofxXmlSettings.h"			// read/write on/to xml file library
#include "ofxKinectV2.h"

// imageAnalysis
#include "ofContourAnalysis.h"		// contour geometry analysis [from Chris Sugrue]
#include "contourSimplify.h"		// contour simplify [from Theodore Watson]
//#include "ofxVideoGrabberProsilica.h"//Prosilica frame grabber

// my addons
#include "ofxStableGate.h"
#include "ofxRestartOFApp.h"

class kinect {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
    // OpenCV
	void computeContourAnalysis();										// compute contour analysis
	void drawContourAnalysis(float x, float y, float w, float h);		// draw contour analysis
//	void smoothingValues();												// smoothing numerical data
//	void normalizeValues();												// normalize numerical data
	void calculateMaxMin();												// get Max min position of a contour
	
	ofxKinectV2					kinectV2;
//	ofxKinect					videoCam;
//	ofxVideoGrabberProsilica	videoCam;
		
	int w, h, W, H, WBig, HBig,  border;
//	bool						bLearnBakground;
//	int							threshold;
	
	// kinect
	ofVec3f						*warpMult;
	ofVec3f						*destPoints;
	ofxCvGrayscaleImage			grayImage;
	ofxCvColorImage				colorImg;
	ofxCvGrayscaleImage			grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage			grayThreshFar; // the far thresholded image
	
	// contour finder
	ofxCvContourFinder			contourFinder;
	contourSimplify				contourS;
	vector<ofVec2f>				*contourReg;
	vector<ofVec2f>				*contourSmooth;
	vector<ofVec2f>				*contourSimple;
	vector<ofVec2f>				*contourHull;
	CvBox2D32f					*box;
	float						*blobAngle;
	float						smoothPct;
	float						tolerance;
	int							runningBlobs;
	float						runningBlobsF;
	vector<ofVec4f>				geomLines;										// geometry lines find into countour
	CvBox2D32f					*_s_blobInfo;									// smoothed blobs info
	vector<ofVec4f>				_s_blobGeom;									// smoothed blobs geometry
	int							cfDetail;										// contour detection detail selector
	
	// Draws
	bool						bShowEllipse;
	bool						bShowAngle;
	bool						bShowLines;
	bool						bDraw;
	
	// Output values
	ofVec2f						left, right, center, angle;
	float						rotX, rotY, rotZ;
	float						rotYf, rotZf;	// increment rotation
	float						handsDist, smoothDegree;
	
	//Events
	ofEvent <string>			trigger;

	//GUI
	ofxPanel gui;
	ofParameter<bool>		enableSwitch;
	ofParameter<float>		nearThreshold,
							farThreshold,
							smoothFactor,
							fadeFactor,
							handsDistThresh,
							speedFactor,
							speed,
							gateOpenDelay,
							gateCloseDelay,
							rotYfactor,
							rotZfactor,
							rotXfactor,
							rotationLimit;
	
	// switch gate
	ofxStableGate			gate;
	ofxRestartOFApp			restarter;
		
	void					enableOSC(bool enable);
	
private:

	// OSC
	void setupOSC();
	void sendOscData();
	void receiveOscData();
	ofxOscSender			sender;											// sending OSC data object
	ofxOscReceiver			receiver;
	bool					sendingSocketReady;
	ofxXmlSettings			host_data;
	string					host_ip;
	string					host_send_port;
	string					host_receive_port;
	
	// OSC contour finder
	CvBox2D32f				*_osc_blobInfo;									// normalized blobs info
	vector<ofVec4f>			_osc_blobGeom;									// normalized blobs geometry
	bool					bOSCenabled;
	
	
	// stratup attempts
	ofxPanel			StartupAttempts;
	ofParameter<int>	attempts;
	
};
