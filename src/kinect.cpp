#include "kinect.h"

//--------------------------------------------------------------
void kinect::setup(){

	//////////////////////////////////////////////
	// CREATE ATTEMPTS XML
	if (!ofFile("attempts.xml").exists()){
		StartupAttempts.add(attempts.set("att",	0, 0, 255 ));
		attempts = 0;
		StartupAttempts.saveToFile("attempts.xml");
	}
	
	//////////////////////////////////////////////
	// kinectV2 settings
	bool succeed = kinectV2.open();		// opens first available kinect
	
	if (!succeed) {
		StartupAttempts.add(attempts.set("att",	0, 0, 255 ));
		StartupAttempts.loadFromFile("attempts.xml");
		attempts ++;
		StartupAttempts.saveToFile("attempts.xml");

		if (attempts < 10){
			ofLogError("kinect") << "kinect did not start in attempt:" << attempts;
			ofLogError("kinect") << "restarting OF.";
			// restart app
			restarter.initRestart(10);
		}else{
			ofLogError("kinect") << "tried to start kinect 10 times without success.";
			StartupAttempts.add(attempts.set("att",	0, 0, 255 ));
			attempts = 0;
			StartupAttempts.saveToFile("attempts.xml");
			ofLogError("kinect") << "closing OF.";
			ofExit();
		}
	}else{
		StartupAttempts.add(attempts.set("att",	0, 0, 255 ));
		attempts = 0;
		StartupAttempts.saveToFile("attempts.xml");
	}

	//////////////////////////////////////////////
	// interface
	w = 160;
	h = 120;
	W = 320;
	H = 240;
	WBig = 512;
	HBig = 424;
	border= 50;
	
	//////////////////////////////////////////////
	//	kinect pixels
//	colorImg.allocate(kinectV2.width, kinectV2.height);
//	ofGrayImg.allocate(kinectV2.width, kinectV2.height, OF_IMAGE_GRAYSCALE);
	grayImage.allocate(kinectV2.width, kinectV2.height);
	grayThreshNear.allocate(kinectV2.width, kinectV2.height);
	grayThreshFar.allocate(kinectV2.width, kinectV2.height);
//	kinectV2.setCameraTiltAngle(15);
	
	//////////////////////////////////////////////
	//	Parameters
	gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.setPosition(border, 340);
	gui.setSize(W+w, 20);
	gui.add(enableSwitch.set(	"enableSwitch",	true ));

	gui.add(kinectV2.minDistance.set(	"minDistance",		250, 0, 12000 ));
	gui.add(kinectV2.maxDistance.set(	"maxDistance",		200, 0, 12000 ));

	gui.add(nearThreshold.set(	"nearThreshold",	250, 0, 255 ));
	gui.add(farThreshold.set(	"farThreshold",		200, 0, 255 ));

	gui.add(minBlobSize.set(	"minBlobSize",		10,	 0, 5000));
	gui.add(maxBlobSize.set(	"maxBlobSize",		2000,0, 50000));
	
	gui.add(smoothPct.set(		"smoothPct",		0.13,0, 1 ));
	gui.add(tolerance.set(		"tolerance",		40,	 0, 100 ));
	
	gui.add(smoothFactor.set(	"smoothFactor",		0.99, 0, 1 ));
	gui.add(fadeFactor.set(		"fadeFactor",		0.98, 0.8, 1 ));
	
	gui.add(handsDistThresh.set("handsDistThresh",	0.5,  0, 1 ));
	gui.add(speed.set(			"current speed",	0.01, 0, 1 ));
	
	gui.add(gateOpenDelay.set(	"gateOpenDelay",	2000, 0, 10000));
	gui.add(gateCloseDelay.set( "gateCloseDelay",	2000, 0, 10000));
	
	gui.add(rotXfactor.set(		"rotX factor",		0.1, 0, 3));
	gui.add(rotYfactor.set(		"rotY factor",		1.0, 0, 10));
	gui.add(rotZfactor.set(		"rotZ factor",		1.0, 0, 10));
	gui.add(speedFactor.set(	"speed factor",		1.01, 0, 2 ));
	gui.add(rotationLimit.set(	"rotation limit",	45,	 0, 180 ));
	
	gui.add(cropX.set(	"crop X",	45,	 0, WBig ));
	gui.add(cropY.set(	"crop Y",	45,	 0, HBig ));
	gui.add(cropW.set(	"crop W",	180,	 0, WBig ));
	gui.add(cropH.set(	"crop H",	120,	 0, HBig ));
	
	gui.loadFromFile("settings.xml");
	
	rotZf		= 0;
	smoothDegree= 0.0;
	rotYf		= 0;
	rotX		= 0;
	rotY		= 0;
	rotZ		= 0;
	int WCrop	= 80;
	int HCrop	= 60;
	
	warpMult = new ofVec3f[4];
	warpMult[0] = ofVec2f(WCrop,HCrop);
	warpMult[1] = ofVec2f(WBig-WCrop,HCrop);
	warpMult[2] = ofVec2f(WBig-WCrop,HBig-HCrop);
	warpMult[3] = ofVec2f(WCrop,HBig-HCrop);
	
	destPoints  = new ofVec3f[4];
	destPoints[0] = ofVec2f(0.0f, 0.0f);
	destPoints[1] = ofVec2f(W, 0.0f);
	destPoints[2] = ofVec2f(W, H);
	destPoints[3] = ofVec2f(0.0f, H);
	
	//////////////////////////////////////////////
	// OSC
	setupOSC();
	bOSCenabled					= false;
	
	//////////////////////////////////////////////
	// contour finder
	contourReg = new vector<ofVec2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSmooth = new vector<ofVec2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSimple = new vector<ofVec2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourHull = new vector<ofVec2f>[MAX_NUM_CONTOURS_TO_FIND];
	box = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	blobAngle = new float[MAX_NUM_CONTOURS_TO_FIND];
	_s_blobInfo = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	_osc_blobInfo = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
//	smoothPct = 0.13f;
//	tolerance = 40.0f;
	runningBlobsF=0;
	
	cfDetail					= 1;// contour detection detail selector
	
	//////////////////////////////////////////////
	// contour viewer
	bShowEllipse				= false;
	bShowAngle					= false;
	bShowLines					= false;
	bDraw						= true;
	
	//////////////////////////////////////////////
	// switch gate
	gate.setup(runningBlobsF);
	gate.setPosition(border, 800);
	gate.useDelay(gateOpenDelay, gateCloseDelay);
	gate.setSize(W+w, 20);
	
}

//--------------------------------------------------------------
void kinect::resizeImages(){
	
//	colorImg.resize				(cropW, cropH);
	grayImage.resize			(cropW, cropH);
	grayThreshNear.resize		(cropW, cropH);
	grayThreshFar.resize		(cropW, cropH);
	
	lcW=cropW;
	lcH=cropH;
}

//--------------------------------------------------------------
void kinect::update(){
	//////////////////////////////////////////////
	// receive OSC data
	receiveOscData();

	//////////////////////////////////////////////
	// receive video
    bool bNewFrame = false;
	kinectV2.update();
	bNewFrame = kinectV2.isFrameNew();

	// kinect 2 is too slow.. we need to process every frame
	if (true){ //bNewFrame
		
		//////////////////////////////////////////////
		// Grab Camera
//		grayImage.setFromPixels(kinectV2.getDepthPixels());
//		grayImage.setFromPixels(videoCam.getDepthPixels(), videoCam.width, videoCam.height);
		ofGrayImg.setFromPixels(kinectV2.getDepthPixels());
		
		//////////////////////////////////////////////
		// Crop
		if (lcW!=cropW || lcH!=cropH){
			resizeImages();
		}
		cropped.cropFrom(ofGrayImg, cropX, cropY, cropW ,cropH);
		
		//////////////////////////////////////////////
		// to CV grayscale
//		colorImg.setFromPixels(ofGrayImg);
//		grayImage.setFromColorImage(colorImg);
		grayImage.setFromPixels(cropped);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		runningBlobs = contourFinder.findContours(grayImage, minBlobSize, maxBlobSize, MAX_NUM_CONTOURS_TO_FIND, false);
		runningBlobsF = runningBlobs;

		if(runningBlobs > 0 && bOSCenabled){
//			cout << ".";
			computeContourAnalysis();
		}else {
			// if there is nobody
			rotZf=rotZf*.98;
			handsDist=handsDistThresh * W/2;
			speed=speed*.98;
			if (speed<0.01)speed=0.01;
		}

		
		//////////////////////////////////////////////
		// smoothing & normalize numerical variables(prepare it for sending via OSC)
		//		smoothingValues();
		//		normalizeValues();
		
		
		//////////////////////////////////////////////
		// send OSC data
		if(sendingSocketReady){
			if (bOSCenabled){
				sendOscData();
			}
		}
	
	}
	
	//////////////////////////////////////////////
	// switch gate
	gate.update();
	
}

//--------------------------------------------------------------
void kinect::draw(){
	if (bDraw) {
		//////////////////////////////////////////////
		// Kinect
//		kinectV2.drawDepth		(border, border, WBig/2, HBig/2);		// draw from the live kinect		
		ofGrayImg.draw		(border, border, WBig/2, HBig/2);		// Main img
		grayImage.draw		(border+W+1, border, w, h);		// Top right
		contourFinder.draw	(border+W+1, border, w, h);		// Bottom Right
		
		// blob detection + contours
		if(sendingSocketReady){
			drawContourAnalysis( border+W+1, border+h+1, w, h);			// Main image
		}
		
		//crop
		ofPushStyle();
		ofNoFill();
		ofSetColor(ofColor::yellow);
		ofRect(border+cropX/2, border+cropY/2, cropW/2, cropH/2);
		ofPopStyle();
		
		// GUI
		gui.draw();

		// switch gate
		gate.draw();
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "press h to hide/show image previews" << endl
//	<< "press k to stop/start Kinect, current status: " << kinectV2.isConnected() << endl
//	<< "num blobs found " << contourFinder.nBlobs << endl
	<< "press ESC to close" << endl
	<< "fps: " << ofGetFrameRate() << endl;
	ofDrawBitmapString(reportStream.str(), border, border+H+20);

}

//--------------------------------------------------------------
void kinect::computeContourAnalysis(){
	for (int i = 0; i < contourFinder.nBlobs; i++){
		
		int length_of_contour = contourFinder.blobs[i].pts.size();

		// if blob is too small
		if (length_of_contour<200){
			rotZf *= .98;
			handsDist=handsDistThresh * W/2;
			speed *= .98;
			if (speed<0.01)speed=0.01;
			break;
		}
		
		// ellipse
		fitEllipse(contourFinder.blobs[i].pts, box[i]);
		
		// angles
		blobAngle[i] =  getOrientation(contourFinder.blobs[i].pts);
		
		// lines
		findLines(contourFinder.blobs[i].pts,geomLines,30,40,30);
		
		// assign smoothed and normalized blobs geometry vectors vars
		_s_blobGeom.clear();
		_s_blobGeom.assign(geomLines.size(), ofVec4f());
		_osc_blobGeom.clear();
		_osc_blobGeom.assign(geomLines.size(), ofVec4f());
		
		// smoothed, simple and convexHull contours computing
		contourReg[i].clear();
		contourReg[i].assign(length_of_contour, ofVec2f());
		contourSmooth[i].clear();
		contourSmooth[i].assign(length_of_contour, ofVec2f());
		
		for(int j = 0; j < length_of_contour; j++){
			contourReg[i].at(j) = contourFinder.blobs[i].pts[j];
		}
		
		contourS.smooth(contourReg[i], contourSmooth[i], smoothPct);
		contourSimple[i].clear();
		
		contourS.simplify(contourSmooth[i], contourSimple[i], tolerance);
		contourS.convexHull(contourSimple[i], contourHull[i]);
		
		left.x=9999.0,right.x=0.0;//,yMin.y=9999.0,yMax.y=0.0;
		for( int k=0; k<length_of_contour; k++ ){
			ofVec4f tmpPos = contourFinder.blobs[0].pts[k];
            if( tmpPos.x < left.x){
                left.x = tmpPos.x;
                left.y = tmpPos.y;
            }
			
			if( tmpPos.x > right.x){
                right.x = tmpPos.x;
                right.y = tmpPos.y;
            }
			
        }
		
		center.x = (left.x + right.x) / 2.0;
		center.y = (left.y + right.y) / 2.0;
		angle.x = (right.x - center.x)/(W/2);
		angle.y = (right.y - center.y)/(H/2);
		handsDist = sqrt(angle.x * angle.x + angle.y * angle.y);
		
		if (handsDist<handsDistThresh){			// if the arms are closed

			rotZf *= fadeFactor;				// slowly decrease rotation angles
			smoothDegree = rotZf;

			speed *= fadeFactor;				// slowly decrease speed
			if (speed<0.01)speed=0.01;
			
			handsDist = handsDistThresh;
		
		}else {									// if the arms are open
			
			rotZf = atan2 (angle.y, angle.x);	// get rotation from kinect
			rotZf = (rotZf / PI) * 180;			// to degrees
			rotZf *= rotZfactor;				// factor
//			rotZf = rotZf * -1.0;				//invert

			// low pass filter
			rotZf = rotZf - rotZf * smoothFactor + smoothDegree * smoothFactor;
			smoothDegree = rotZf;
			
			// increment speed
			speed = speed * speedFactor;
			if (speed>1) speed = 1;
		}

		// apply rotation limits
		rotZf = MIN(rotZf, (float) rotationLimit);
		rotZf = MAX(rotZf, (float) rotationLimit * -1);
		
		// calc rotation in Y
		rotYf		= (rotZf*- rotYfactor)+rotYf;
		handsDist	*= W/2;
	}
}

//--------------------------------------------------------------
void kinect::drawContourAnalysis(float x, float y, float w, float h){
	
	ofFill();
	ofSetColor(ofColor::grey);
	ofRect(x,y, w, h);
	ofSetColor(ofColor::white);

	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(0.5,0.5,0.0);
	
	//printf("cfDetail %i \n", cfDetail);
	for (int i = 0; i < contourFinder.nBlobs; i++){
		//-------------------  draw the contours
		if(cfDetail == 0){
			ofSetColor(ofColor::red);
			ofNoFill();
			drawBlob(0,0,contourFinder.blobs[i]);
		}else if(cfDetail == 1){
			ofSetColor(ofColor::red);
			ofNoFill();
			ofBeginShape();
			for(unsigned int j = 0; j < contourSmooth[i].size(); j++){
				ofVertex(contourSmooth[i].at(j).x, contourSmooth[i].at(j).y);
			}
			ofEndShape(true);
		}else if(cfDetail == 2){
			ofSetColor(ofColor::red);
			ofNoFill();
			ofBeginShape();
			for(unsigned int k = 0; k < contourSimple[i].size(); k++){
				ofVertex(contourSimple[i].at(k).x, contourSimple[i].at(k).y);
			}
			ofEndShape(true);
		}else if(cfDetail == 3){
			ofSetColor(ofColor::red);
			ofNoFill();
			ofBeginShape();
			for(unsigned int z = 0; z < contourHull[i].size(); z++){
				ofVertex(contourHull[i].at(z).x, contourHull[i].at(z).y);
			}
			ofEndShape(true);
			for(unsigned int w = 0; w < contourHull[i].size(); w++){
				ofCircle(contourHull[i].at(w).x, contourHull[i].at(w).y, 2);
			}
		}
		//------------------- fit ellipse to that blob and draw it
		if(bShowEllipse){
			ofNoFill();
			ofSetColor(ofColor::white);
			
			glPushMatrix();
			glTranslatef(box[i].center.x, box[i].center.y,0);
			glRotatef(box[i].angle,0,0,1);
			ofEllipse( 0,0, box[i].size.width*.5,box[i].size.height*.5);
			glPopMatrix();
		}
		//------------------- fit angle of orientation
		if(bShowAngle){
			ofSetColor(ofColor::white);
			
			float x1,y1,x2,y2;
			
			x1 = contourFinder.blobs[i].centroid.x + 25 * cos(blobAngle[i]);
			y1 = contourFinder.blobs[i].centroid.y + 25 * sin(blobAngle[i]);
			x2 = contourFinder.blobs[i].centroid.x - 25 * cos(blobAngle[i]);
			y2 = contourFinder.blobs[i].centroid.y - 25 * sin(blobAngle[i]);
			
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
			
			x1 = contourFinder.blobs[i].centroid.x + 10 * cos(blobAngle[i]+HALF_PI);
			y1 = contourFinder.blobs[i].centroid.y + 10 * sin(blobAngle[i]+HALF_PI);
			x2 = contourFinder.blobs[i].centroid.x - 10 * cos(blobAngle[i]+HALF_PI);
			y2 = contourFinder.blobs[i].centroid.y - 10 * sin(blobAngle[i]+HALF_PI);
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
		}
		//------------------- fit geometry lines on countour
		if(bShowLines){
			ofSetColor(ofColor::green);
			ofNoFill();
			for(unsigned int j = 0; j < geomLines.size(); j++){
				ofLine(geomLines[j].x,geomLines[j].y,geomLines[j].z,geomLines[j].w);
				ofCircle(geomLines[j].x,geomLines[j].y,3);
				ofCircle(geomLines[j].z,geomLines[j].w,3);
			}
		}
	}
	
	ofSetColor(ofColor::yellow);
	ofCircle(left.x,left.y,3);
	ofCircle(right.x,right.y,3);

	ofSetColor(ofColor::white);
		ofCircle(center.x, center.y, speed/10+3);
	ofCircle(w, h, speed*10+3);
	
	ofSetColor(ofColor::white);
	glPushMatrix();
//		glTranslatef(center.x,center.y,0);
		glTranslatef(WBig/2,HBig/2,0);
		glRotatef((rotZf)*-1,0,0,1);
		ofLine(-handsDist,0,handsDist,0);
	glPopMatrix();
	
	glPopMatrix();
}

/*
//--------------------------------------------------------------
void kinect::smoothingValues(){
	
	// blobs info from contour finder
	for(int b=0;b<contourFinder.nBlobs;b++){
		_s_blobInfo[b].center.x		= _s_blobInfo[b].center.x*0.95 + box[b].center.x*0.05;
		_s_blobInfo[b].center.y		= _s_blobInfo[b].center.y*0.95 + box[b].center.y*0.05;
		_s_blobInfo[b].angle		= _s_blobInfo[b].angle*0.95 + box[b].angle*0.05;
		_s_blobInfo[b].size.width	= _s_blobInfo[b].size.width*0.95 + box[b].size.width*0.05;
		_s_blobInfo[b].size.height	= _s_blobInfo[b].size.height*0.95 + box[b].size.height*0.05;
	}
	for(unsigned int g = 0; g < geomLines.size(); g++){
		_s_blobGeom[g].x = _s_blobGeom[g].x*0.95 + geomLines[g].x*0.05;
		_s_blobGeom[g].y = _s_blobGeom[g].y*0.95 + geomLines[g].y*0.05;
		_s_blobGeom[g].z = _s_blobGeom[g].z*0.95 + geomLines[g].z*0.05;
		_s_blobGeom[g].w = _s_blobGeom[g].w*0.95 + geomLines[g].w*0.05;
	}
	
}

//--------------------------------------------------------------
void kinect::normalizeValues(){
	
	// blobs info from contour finder
	for(int b=0;b<contourFinder.nBlobs;b++){
		_osc_blobInfo[b].center.x = ofNormalize(_s_blobInfo[b].center.x,0.0f,320.0f);
		_osc_blobInfo[b].center.y = ofNormalize(_s_blobInfo[b].center.y,0.0f,240.0f);
		_osc_blobInfo[b].angle = ofNormalize(_s_blobInfo[b].angle,0.0f,360.0f);
		_osc_blobInfo[b].size.width = ofNormalize(_s_blobInfo[b].size.width,0.0f,320.0f);
		_osc_blobInfo[b].size.height = ofNormalize(_s_blobInfo[b].size.height,0.0f,240.0f);
	}
	for(unsigned int g = 0; g < geomLines.size(); g++){
		_osc_blobGeom[g].x = ofNormalize(_s_blobGeom[g].x,0.0f,320.0f);
		_osc_blobGeom[g].y = ofNormalize(_s_blobGeom[g].y,0.0f,240.0f);
		_osc_blobGeom[g].z = ofNormalize(_s_blobGeom[g].z,0.0f,320.0f);
		_osc_blobGeom[g].w = ofNormalize(_s_blobGeom[g].w,0.0f,240.0f);
	}
	sendingSocketReady = true; // start sending osc vars
}
 */

//--------------------------------------------------
void kinect::setupOSC(){
	////////////////////////////////// local xml capture
	host_data.loadFile("OSC_settings.xml");
	host_ip = host_data.getValue("ip"," ",0);
	host_send_port = host_data.getValue("send_port"," ",0);
	host_receive_port = host_data.getValue("receive_port"," ",0);
	
//	sender.setup(host_ip.c_str(),atoi(host_send_port.c_str()));
	sender.setup(host_ip, ofToInt(host_send_port));
	receiver.setup( ofToInt(host_receive_port));
	sendingSocketReady			= true;
}

//--------------------------------------------------
void kinect::sendOscData(){
	ofxOscMessage m;
		
	float finalSpeed = speed	* -0.02;
	float finalX = rotXfactor	+ rotX;		// current + initial
	float finalY = rotYf		+ rotY;		// current + initial
	float finalZ = rotZf		+ rotZ;		// current + initial
	
//	finalX = ofDegToRad(finalX);
	finalY = ofDegToRad(finalY);
	finalZ = ofDegToRad(finalZ);
	
	/*
	ofLogNotice("finalX")<<finalX;
	ofLogNotice("finalY")<<finalY;
	ofLogNotice("finalZ")<<finalZ;
	*/
	
	char temp[64];
	m.setAddress("/posRot");
	m.addFloatArg(0);
	m.addFloatArg(0);
	m.addFloatArg(finalSpeed);
	m.addFloatArg(finalX);
	m.addFloatArg(finalY);
	m.addFloatArg(finalZ);
	
	sender.sendMessage(m);
	m.clear();
}


//--------------------------------------------------
void kinect::receiveOscData(){
	// get the next message
	ofxOscMessage m;
	m.clear();
	receiver.getNextMessage( &m );
	
	//check for rotation flag
	if ( m.getAddress() == "/rotation" )
	{
		rotX = m.getArgAsFloat( 0 );
		rotY = m.getArgAsFloat( 1 );
		rotZ = m.getArgAsFloat( 2 );
		
		rotZf=rotYf=0;
		
		ofLogNotice()
		<< "rotation x = " << rotX
		<< " y = " << rotY
		<< " z = " << rotZ;
	}
	
}

 
//--------------------------------------------------------------
void kinect::keyPressed  (int key){
	
	string message;
	
	switch (key){

		case 'h':
			bDraw = !bDraw;
//			bShowEllipse= !bShowEllipse;
//			bShowAngle= !bShowAngle;
//			bShowLines= !bShowLines;

			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case ' ':
//			bLearnBakground = true;
			break;
		
		case 'i':
			handsDistThresh = handsDistThresh + 0.01;
			if (handsDistThresh > 1.0) handsDistThresh = 1.0;
			break;
		
		case 'o':
			handsDistThresh = handsDistThresh - 0.01;
			if (handsDistThresh < 0) handsDistThresh = 0;
			break;
			
		case 'x':
			cout << "left.x = " << left.x << " right.x = " << right.x << endl;
			cout << "left.y = " << left.y << " right.y = " << right.y << endl;
			//printf("x %i, y %i \n", varX, varY);
			break;
		
		case 'c':
			cout << "center.x = " << center.x << " center.y = " << center.y << endl;
			break;
		
		case 'd':
			cout << "rotZf = " << rotZf << endl;
			break;

		case 'b':
			message = "gotoBlender";
			ofNotifyEvent(trigger, message, this);
			break;
			
		case 'v':
			message = "gotoVLC";
			ofNotifyEvent(trigger, message, this);
			break;
	}
}
//--------------------------------------------------------------
void kinect::enableOSC(bool enable) {
	bOSCenabled = enable;
}

//--------------------------------------------------------------
void kinect::exit() {
//	kinectV2.setCameraTiltAngle(0); // zero the tilt on exit
	kinectV2.close();
}
