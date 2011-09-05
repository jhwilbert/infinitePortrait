#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	// define what is the buffer size to detect face
	bufferSize = 50; 
	faceCounter = 0;
	
	verdana.loadFont("verdana.ttf",32);
	
	foundFace = false;
	isTracked = false;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);
	
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	
	copyImgs = new ofTexture[100];

	
	// Haar XML objects
	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");	
	eyeFinder.setup("haarXML/haarcascade_eye.xml");
	
}

//--------------------------------------------------------------
void testApp::update(){
		
	ofBackground(200,200,200);
    
    bool bNewFrame = false;
	takePicture = false;


	
	// Finding Face /////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Adds to array to check persistance
	if(haarFinder.facesFound() == 1) {
		if(bufferFace.size() < bufferSize) {
			bufferFace.push_back(1); 		
		} else {
			bufferFace.clear();
		}		
	} else {
		if(bufferFace.size() < bufferSize) {
			bufferFace.push_back(0);
		} else {
			bufferFace.clear();
		}		
	}		
	// Check if all elements in array are 1 to find face
	for (int i=0; i < bufferFace.size(); i++) {
		if(bufferFace[i] == 1) {
			foundFace = true;			
		} else {
			foundFace = false;
			takePicture = false;
		}
	}
	
	// Face Found ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(foundFace == true && isTracked == false) {
		isTracked = true;		
		countdown = "Found Face";
		faceCounter++;
		// Drawing Stack of ofTextures
		copyImgs[faceCounter].allocate(320, 240,GL_RGB);		
		newPixels = vidGrabber.getPixels();
		copyImgs[faceCounter].loadData(newPixels,320,240,GL_RGB);	
		
		

	} else if(foundFace == false && isTracked == true) {
		isTracked = false;
		countdown = "Searching...";
	} else {
		
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
		
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		grayImage = colorImg;
		
		// finding face
		haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);
		
		// finding eyes
		eyeFinder.findHaarObjects(grayImage,0, 0, 0);		
		
	}	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// onscreen print
	ofSetColor(0x00FF00);
	verdana.drawString(countdown, 40,700);
	
	// drawing video canvas
	ofSetColor(0xffffff);
	colorImg.draw(20,20);	

	//grayImage.draw(20 ,420);
	
	ofEnableAlphaBlending();  
	
	for(int i=0; i < faceCounter; i++) {
		ofSetColor(255,255,255,127);   // RGBA  
		copyImgs[i].draw(50,260);
	}
	ofDisableAlphaBlending();  
	

	// Drawing Blobs /////////////////////////////////////////////////////////////////////////////////////////////////
	
	haarFinder.draw(20, 20);	

	int numFace = haarFinder.blobs.size();
	int numEyes = eyeFinder.blobs.size();

	glPushMatrix();	
	
	glTranslatef(20, 20, 0);
	
	for(int i = 0; i < numFace; i++){
		float x = haarFinder.blobs[i].boundingRect.x;
		float y = haarFinder.blobs[i].boundingRect.y;
		float w = haarFinder.blobs[i].boundingRect.width;
		float h = haarFinder.blobs[i].boundingRect.height;		
		float cx = haarFinder.blobs[i].centroid.x;
		float cy = haarFinder.blobs[i].centroid.y;
		
		ofSetColor(0xFFFFFF);
		ofRect(x, y, w, h);
		
		ofSetColor(0xFFFFFF);
		ofDrawBitmapString("face "+ofToString(i), cx, cy);		
	}	
	
	for(int i = 0; i < numEyes; i++) {  			
		float ex = eyeFinder.blobs[i].boundingRect.x;
		float ey = eyeFinder.blobs[i].boundingRect.y;
		float ew = eyeFinder.blobs[i].boundingRect.width;
		float eh = eyeFinder.blobs[i].boundingRect.height;		
		float ecx = eyeFinder.blobs[i].centroid.x;
		float ecy = eyeFinder.blobs[i].centroid.y;

		if(numEyes == 2) {
			ofSetColor(0xFF0000);
			ofRect(ex, ey, ew, eh);	
			ofLine(eyeFinder.blobs[0].centroid.x,eyeFinder.blobs[0].centroid.y,eyeFinder.blobs[1].centroid.x,eyeFinder.blobs[1].centroid.y);			
		}
	}  

	glPopMatrix();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}	

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}
