#ifndef _TEST_APP
#define _TEST_APP

#define OF_ADDON_USING_OFXOPENCV

#include "ofMain.h"
#include "ofAddons.h"
#include "ofxTimer.h"
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

	

	ofVideoGrabber			vidGrabber;
	ofTexture*				copyImgs;
	
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	staticImage;

	
	// Finds in moving image
	ofxCvHaarFinder			haarFinder;
	ofxCvHaarFinder			staticFinder;
	
	//ofxCvHaarFinder		eyeFinder;
	
	// Finds in static image
	
	TimedCounter*			counter;
	
	vector<int>				bufferFace;
	
	string					countdown;
	int						bufferSize;
	int						faceCounter;
	int						r,g,b;
	int						totalPixels;
	int						bufferTotal;
	int						threshold;
	bool					bLearnBakground;
	bool					foundFace;
	bool					isTracked;
	bool					takePicture;
	bool					pictureTaken;
	bool					processFace;
	// tracking points
	float					sx,sy,sw,sh,scx,scy;
	float					ex,ey,ew,eh,ecx,ecy;
	float					x,y,w,h,cx,cy;
	
	unsigned char *			newPixels;
		
	ofTrueTypeFont			verdana;
	
};

#endif
