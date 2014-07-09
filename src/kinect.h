#pragma once

#include "ofMain.h"

// COUNTOUR FINDER
#define		MAX_NUM_CONTOURS_TO_FIND	1

// official addons
////#include "ofxVectorMath.h"			// vectorial math structures library
#include "ofxOpenCv.h"				// computer vision library
#include "ofxOsc.h"					// Open Sound Control communication library
#include "ofxXmlSettings.h"			// read/write on/to xml file library
#include "ofxKinect.h"

// imageAnalysis
#include "ofContourAnalysis.h"		// contour geometry analysis [from Chris Sugrue]
#include "contourSimplify.h"		// contour simplify [from Theodore Watson]
//#include "ofxVideoGrabberProsilica.h"//Prosilica frame grabber

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
// otherwise, we'll use a movie file
#define PORT 9000
#define NUM_MSG_STRINGS 20


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
	void smoothingValues();												// smoothing numerical data
	void normalizeValues();												// normalize numerical data
	void calculateMaxMin();												// get Max min position of a contour
	
	
	// OSC function
	void getIPfromXML();
	void sendOscData();
	void receiveOscData();
	
#ifdef _USE_LIVE_VIDEO
	ofxKinect					videoCam;
//	ofxVideoGrabberProsilica	videoCam;
	
#else
	//ofVideoPlayer 		vidPlayer;
#endif
	
	int w, h, W, H, WBig, HBig,  border;
	bool						bLearnBakground;
	ofVec3f					*warpMult;
	ofVec3f					*destPoints;
	ofxCvGrayscaleImage		grayImage;
	int							threshold;
	
	// kinect
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	int nearThreshold;
	int farThreshold;

	// old cam
/*	ofxCvGrayscaleImage		colorImage;
	ofxCvColorImage			colorImgWarped;
	ofxCvGrayscaleImage		colorImageSmall;
	ofxCvGrayscaleImage		grayBg;
	ofxCvGrayscaleImage		grayDiff;
*/
	
	// contour finder
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;
	vector<ofVec2f>			*contourReg;
	vector<ofVec2f>			*contourSmooth;
	vector<ofVec2f>			*contourSimple;
	vector<ofVec2f>			*contourHull;
	CvBox2D32f					*box;
	float						*blobAngle;
	float						smoothPct;
	float						tolerance;
	int							runningBlobs;
	
	vector<ofVec4f>				geomLines;										// geometry lines find into countour
	
	// OSC vars
	ofxXmlSettings				host_data;
	string						host_number;
	string						host_port;
	
	// smoothed vars
	CvBox2D32f					*_s_blobInfo;									// smoothed blobs info
	vector<ofVec4f>				_s_blobGeom;									// smoothed blobs geometry
	
	// OSC sending vars
	CvBox2D32f					*_osc_blobInfo;									// normalized blobs info
	vector<ofVec4f>				_osc_blobGeom;									// normalized blobs geometry
	bool						sendOsc_CF;
	
	int							cfDetail;										// contour detection detail selector
	bool						sendingSocketReady;
	bool						bShowEllipse;
	bool						bShowAngle;
	bool						bShowLines;
	bool						bDraw;
	
	// Flickflock values
	ofVec2f						left, right, center, angle;
	float						degree, incrementDegree, distance, smoothDegree, smoothFactor, distanceLimit, speed;
	float						rotX, rotY, rotZ;

	//Events
	ofEvent <string> trigger;

private:
	ofxOscSender				sender;											// sending OSC data object
	ofxOscReceiver				receiver;

	
};
