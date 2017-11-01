#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"
#include "ofProtonect2.h"
#include "ofxJson.h"


class ofApp : public ofBaseApp{
    
    ofxMultiKinectV2 kinect;
    ofTexture depthTex;
    ofFloatPixels depthPix;
    ofShader depthShader;
    
    // pointcloud
    ofEasyCam ecam;
    ofVboMesh mesh;
    bool ispointcloud = false;
    
    // record
    bool isRecording = false;
    string current_time;
    string output_dir;
    string save_dir;


	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
