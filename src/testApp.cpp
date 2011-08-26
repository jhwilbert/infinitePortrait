#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	bufferSize = 10; // define what is the buffer size to detect face
	
	verdana.loadFont("verdana.ttf",32);
	
	foundFace = false;
	isTracked = false;
	// timer
	this->counter = new TimedCounter(5);
	//this->counter->startCount();	
	
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(320,240);
	#else
		vidPlayer.loadMovie("fingers.mp4");
		vidPlayer.play();
	#endif
	
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	bLearnBakground = true;
	threshold = 80;
	

	
	//lets load in our face xml file
	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	

	
//	if(this->counter->isCounting()){
//		this->counter->update();
//		if(this->counter->hasChanged())
//			cout << "counter changed:" << this->counter->getCurrentCount() << endl;
//		if(this->counter->isCountComplete())
//			cout << "counter complete!" << endl;
//		
//	}
	
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
	
	//bufferTotal = 0;
	
	// sum numbers
	for (int i=0; i < bufferFace.size(); i++) {
		if(bufferFace[i] == 1) {
			//cout << "all one";
			foundFace = true;
		} else {
			foundFace = false;
		}
	}
	
	if(foundFace == true && isTracked == false) {
		isTracked = true;
		cout << "found";
	} else if(foundFace == false && isTracked == true) {
		cout << "lost it";
		isTracked = false;
	} else {
		// don't do anything
	}
	
	
	ofBackground(100,100,100);
    
    bool bNewFrame = false;
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.grabFrame();
		bNewFrame = vidGrabber.isFrameNew();
	#else
		vidPlayer.idleMovie();
		bNewFrame = vidPlayer.isFrameNew();
	#endif
	
	if (bNewFrame){
		
		#ifdef _USE_LIVE_VIDEO
			colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		#else
			colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
		#endif
		
        grayImage = colorImg;
		
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		

		
		haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);
		

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	

	ofSetColor(0x00FF00);
	verdana.drawString("hello, this is franklin book calling\nanyone home?", 100,100);
	
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	colorImg.draw(20,20);	
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
	
	//haarFinder.draw(20, 20);	
	
	int numFace = haarFinder.blobs.size();
	
	
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
	
	// then draw the contours:
	
	ofFill();
	ofSetColor(0x333333);
	ofRect(360,540,320,240);
	ofSetColor(0xffffff);
    //contourFinder.draw(360,540);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }
	
	// finally, a report:
	
	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i", threshold, contourFinder.nBlobs);
	ofDrawBitmapString(reportStr, 20, 600);
	
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
