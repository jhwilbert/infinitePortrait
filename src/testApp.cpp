#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	// define what is the buffer size to detect face
	bufferSize = 50; 
	
	verdana.loadFont("verdana.ttf",32);
	
	foundFace = false;
	isTracked = false;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);

	
    colorImg.allocate(320,240);
	copyImg.allocate(320, 240);
	grayImage.allocate(320,240);

	
	// Haar XML objects
	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");	
	eyeFinder.setup("haarXML/haarcascade_eye.xml");
	
}

//--------------------------------------------------------------
void testApp::update(){
		
	ofBackground(200,200,200);
    
    bool bNewFrame = false;
	takePicture = false;

	
	// checks if face is detected for some time	
	if(haarFinder.facesFound() == 1) {
		if(bufferFace.size() < bufferSize) {
			bufferFace.push_back(1); // adds to array to check persistance			
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
	
	// Starts counter if it finds
	
	if(foundFace == true && isTracked == false) {
		isTracked = true;
		//this->counter->startCount();
		countdown = "Found Face";
		copyImg = colorImg;
	} else if(foundFace == false && isTracked == true) {
		cout << "Lost";
		isTracked = false;
		countdown = "Searching...";
	} else {
		
	}
	
	// Video Control

	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
		
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		grayImage = colorImg;
		
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		if(takePicture == true) {
			countdown = "Picture Taken";
			copyImg = colorImg;
			takePicture = false;
			pictureTaken = true;
		}
		
		
		// finding face
		haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);
		
		// finding eyes
		eyeFinder.findHaarObjects(grayImage,10, 99999999, 10);

	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// onscreen print
	ofSetColor(0x00FF00);
	verdana.drawString(countdown, 40,400);
	
	// drawing video canvas
	ofSetColor(0xffffff);
	colorImg.draw(20,20);	
	copyImg.draw(360,20);
	grayImage.draw(20 ,420);

	// draw Haar object findings
	haarFinder.draw(20, 20);	
	
	int numFace = haarFinder.blobs.size();
	
	
	// finding face
	glPushMatrix();
	
	glTranslatef(20, 20, 0);
	
	for(int i = 0; i < numFace; i++){
		float x = haarFinder.blobs[i].boundingRect.x;
		float y = haarFinder.blobs[i].boundingRect.y;
		float w = haarFinder.blobs[i].boundingRect.width;
		float h = haarFinder.blobs[i].boundingRect.height;
		
		float cx = haarFinder.blobs[i].centroid.x;
		float cy = haarFinder.blobs[i].centroid.y;
		
		ofSetColor(0xFF0000);
		ofRect(x, y, w, h);
		
		ofSetColor(0xFFFFFF);
		ofDrawBitmapString("face "+ofToString(i), cx, cy);
		
	}
	
	glPopMatrix();

	
	// finding eyes
	glPushMatrix();
	
	glTranslatef(20, 20, 0);
	
	for(int i = 0; i < eyeFinder.blobs.size(); i++) {  
		
		ofxCvBlob cur = eyeFinder.blobs[i];  
		
		float ex = cur.boundingRect.x;
		float ey = cur.boundingRect.y;
		
		ofSetColor(0xFF0000);
		ofCircle(ex, ey, 10);		

	}  

	glPopMatrix();

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 's':
			vidGrabber.videoSettings();
			break;
	}
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

void testApp::exit(){
	delete this->counter;
}
