#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	// Variables
	bufferSize = 50; 
	faceCounter = 0;
	

	foundFace = false;
	isTracked = false;
	
	// Screen Fonts
	verdana.loadFont("verdana.ttf",32);

	// Video Player
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);
	
	// Allocations
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	staticImage.allocate(320,240);
	
	copyImgs = new ofTexture[100];
	
	// Haar XML objects
	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");
	staticFinder.setup("haarXML/haarcascade_frontalface_default.xml");	

	
}

//--------------------------------------------------------------
void testApp::update(){
	
	cout << processFace;
	
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
			
		processFace = true;
		isTracked = true;		
		countdown = "Found Face";
		faceCounter++;
		
		/* Drawing Stack of ofTextures */
		copyImgs[faceCounter].allocate(320, 240,GL_RGB);		
		newPixels = vidGrabber.getPixels();
		copyImgs[faceCounter].loadData(newPixels,320,240,GL_RGB);	
		
		staticImage = colorImg;
		staticFinder.findHaarObjects(grayImage, 10, 99999999, 10);


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
		haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);		
	}	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	/* Drawing Video Print */
	ofSetColor(0xffffff);
	colorImg.draw(20,20);	
	staticImage.draw(340 ,20);
	//grayImage.draw(20 ,420);
	
	// Drawing Blobs /////////////////////////////////////////////////////////////////////////////////////////////////
	
	haarFinder.draw(20, 20);	
	
	int numFace = haarFinder.blobs.size();
//	int numEyes = eyeFinder.blobs.size();

	glPushMatrix();	
	
	glTranslatef(20, 20, 0);
	
	// draws on moving image
	for(int i = 0; i < numFace; i++){
		x = haarFinder.blobs[i].boundingRect.x;
		y = haarFinder.blobs[i].boundingRect.y;
		w = haarFinder.blobs[i].boundingRect.width;
		h = haarFinder.blobs[i].boundingRect.height;		
		cx = haarFinder.blobs[i].centroid.x;
		cy = haarFinder.blobs[i].centroid.y;
		
		ofSetColor(0xFFFFFF);
		ofRect(x, y, w, h);
		
		ofSetColor(0xFFFFFF);
		ofDrawBitmapString("face "+ofToString(i), cx, cy);		
	}	

	glPopMatrix();
	
	
	if(processFace == true) {
		glPushMatrix();	
		
		glTranslatef(340, 20, 0);
		
		// draws on static image
		for(int i = 0; i < numFace; i++){
			sx = staticFinder.blobs[i].boundingRect.x;
			sy = staticFinder.blobs[i].boundingRect.y;
			sw = staticFinder.blobs[i].boundingRect.width;
			sh = staticFinder.blobs[i].boundingRect.height;		
			scx = staticFinder.blobs[i].centroid.x;
			scy = staticFinder.blobs[i].centroid.y;
			
			ofSetColor(0xFFFFFF);
			ofRect(sx, sy, sw, sh);
				
		}
		
		glPushMatrix();	
		//processFace = false;
	}

	



	// Drawing Images /////////////////////////////////////////////////////////////////////////////////////////////////

	ofEnableAlphaBlending();  
	
	for(int i=0; i < faceCounter; i++) {
		ofSetColor(255,255,255,127);   // RGBA  
		copyImgs[faceCounter].draw(sx,260);
	}
	
	ofDisableAlphaBlending();  
	
	
	// Drawing Prints /////////////////////////////////////////////////////////////////////////////////////////////////
	
	// onscreen print
	ofSetColor(0x00FF00);
	verdana.drawString(countdown, 40,700);
	

		
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
