#ifndef _TEST_APP
#define _TEST_APP



#define OF_ADDON_USING_OFXOPENCV

#include "ofMain.h"
#include "ofAddons.h"
#include "ofxTimer.h"


#define _USE_LIVE_VIDEO		// uncomment this to use a live camera

//this is not part of the addon yet
//so include it seperately
#include "ofxCvHaarFinder.h"


class testApp : public ofSimpleApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void exit();
	
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer 		vidPlayer;
#endif
	
	ofxCvColorImage			colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder		contourFinder;
	ofxCvHaarFinder			haarFinder;
	TimedCounter*			counter;
	
	vector<int>				bufferFace;
	
	int						bufferSize;
	int						bufferTotal;
	int						threshold;
	bool					bLearnBakground;
	bool					foundFace;
	bool					isTracked;
	
	ofTrueTypeFont verdana;
	
};

#endif