#include "ofMain.h"
#include "testApp.h"

//#define	FULL_SCREEN

//========================================================================
int main( ){


    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800,600, OF_GAME_MODE);
	ofSetWindowPosition(1440,0);
    //ofSetWindowPosition(ofGetScreenWidth(),0);


	ofRunApp( new testApp() );
}
