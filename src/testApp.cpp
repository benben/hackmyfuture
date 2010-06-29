#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    //ofSetFrameRate(60);
	vidGrabber.setVerbose(true);
	//vidGrabber.setDeviceID(1);
	vidGrabber.initGrabber( WIDTH, HEIGHT );

	colorLargeImage.allocate( WIDTH, HEIGHT );
	colorSmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
    grayLargeImage.allocate( WIDTH, HEIGHT );
	graySmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );

	sourceToSampleScale = WIDTH / SAMPLE_WIDTH;
	sampleToSourceScale	= SAMPLE_WIDTH / (float)WIDTH;

	haarFinder.setup( "default.xml" );
	haarTracker.setup( &haarFinder );

    ofEnableAlphaBlending() ;
	pic.loadImage("future.png");
	tex.allocate(pic.width,pic.height,GL_RGBA);
	tex.loadData(pic.getPixels(),pic.width,pic.height,GL_RGBA);
	printf("w: %d, h: %d\n",ofGetWidth(),ofGetHeight());
	eeg.loadImage("eeg.png");
	subversion.loadImage("subversion.jpg");

	grabbed = false;
	showit = true;
	cangrab=false;
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground( 10, 10, 10 );

    bool bNewFrame = false;

	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();

	if( bNewFrame )
	{
		colorLargeImage.setFromPixels( vidGrabber.getPixels(), WIDTH, HEIGHT );

		colorLargeImage.mirror( false, true );
		colorSmallImage.scaleIntoMe(colorLargeImage);

		grayLargeImage	= colorLargeImage;
        graySmallImage	= colorSmallImage;

		haarTracker.findHaarObjects( graySmallImage );
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
    if(showit) {
	int i;
	float x, y, w, h;
	float wRatio, hRatio, scale;
	int faceID;
	float faceMode;

    wRatio = ofGetWidth() / (float)WIDTH;
    hRatio = ofGetHeight() / (float)HEIGHT;

    scale = MIN( wRatio, hRatio );

    glPushMatrix();
    glTranslatef
    (
        (int)( ( ofGetWidth() - ( WIDTH * scale ) ) * 0.5f ),
        (int)( ( ofGetHeight() - ( HEIGHT * scale ) ) * 0.5f ),
        0
    );
    glScalef( scale, scale, 0 );

	ofSetColor( 0xffffff );

	grayLargeImage.draw( 0, 0 );

	//colorLargeImage.draw(0,0);

	while( haarTracker.hasNextHaarItem() )
	{
		faceID		= haarTracker.getHaarItemID();
		faceMode	= ( faceID % 10 ) / 10.0f;

		haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );

		x	*= sourceToSampleScale;
		y	*= sourceToSampleScale;
		w	*= sourceToSampleScale;
		h	*= sourceToSampleScale;

		tex.draw(x,y,w,h);
	}

    glPopMatrix();

	//ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	//eeg.draw(ofGetWidth()-40-eeg.width,ofGetHeight()-2*eeg.height);

    if (ofGetMinutes() % 10 == 0) {
        if (!grabbed) {
            takePicture();
            grabbed = true;
        }
    } else {
        grabbed = false;
    }



    }
    else {
    subversion.draw(0,0);
    }
}

void testApp::takePicture() {
        grab.grabScreen(0,0,ofGetWidth(),ofGetHeight());
        grab.saveImage(  ofToString(ofGetYear()) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + "_" + ofToString(ofGetSeconds()) + ".png" );
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
            takePicture();
		break;
		case 's':
            if(showit) {
                showit = false;
            } else {
                showit = true;
            }
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
void testApp::mouseReleased()
{
	//
}
