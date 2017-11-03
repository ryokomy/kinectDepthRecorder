#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofHideCursor();
    CGDisplayHideCursor(NULL);
    
    // settings
    ofxJson json;
    ofFile file("settings.json", ofFile::ReadOnly);
    file >> json;
    output_dir = json["outputdir"];
    
    kinect.open(false, true, 0);
    // Note :
    // Default OpenCL device might not be optimal.
    // e.g. Intel HD Graphics will be chosen instead of GeForce.
    // To avoid it, specify OpenCL device index manually like following.
    // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
    
    kinect.start();
    
    depthShader.load("shader/depth_shader");
    
    mesh.setUsage(GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    ecam.setAutoDistance(false);
    ecam.rotate(180, 0, 1, 0);
    ecam.setDistance(200);
    
    // dump ir params
    ofxJson irjson;
    libfreenect2::Freenect2Device::IrCameraParams params = kinect.getProtonect()->getIrCameraParams();
    irjson["fx"] = params.fx;
    irjson["fy"] = params.fy;
    irjson["cx"] = params.cx;
    irjson["cy"] = params.cy;
    irjson["k1"] = params.k1;
    irjson["k2"] = params.k2;
    irjson["p1"] = params.p1;
    irjson["p2"] = params.p1;
    irjson["k3"] = params.k3;
    ofFile irfile("ir_params.json", ofFile::WriteOnly);
    irfile << irjson;
    irfile.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    current_time = ofGetTimestampString();
    
    kinect.update();
    if (kinect.isFrameNew()) {
        
        if(ispointcloud){
            mesh.clear();
            {
                int step = 2;
                int h = kinect.getDepthPixelsRef().getHeight();
                int w = kinect.getDepthPixelsRef().getWidth();
                for(int y = 0; y < h; y += step) {
                    for(int x = 0; x < w; x += step) {
                        float dist = kinect.getDistanceAt(x, y);
                        if(dist > 50 && dist < 500) {
                            ofVec3f pt = kinect.getWorldCoordinateAt(x, y, dist);
                            //                        ofColor c;
                            //                        float h = ofMap(dist, 50, 200, 0, 255, true);
                            //                        c.setHsb(h, 255, 255);
                            //                        mesh.addColor(c);
                            mesh.addVertex(pt);
                        }
                    }
                }
            }
        }
        else{
            depthTex.loadData(kinect.getDepthPixelsRef());
            depthPix = kinect.getDepthPixelsRef();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    float scale=2.0;
    
    if(ispointcloud){
        if (mesh.getVertices().size()) {
            ofPushView();
            ofViewport(ofRectangle(0, 0, 512*scale, 424*scale));
            ofPushStyle();
            glPointSize(2);
            ecam.begin();
            ofDrawAxis(100);
            ofPushMatrix();
            ofTranslate(0, 0, -100);
            mesh.draw();
            ofPopMatrix();
            ecam.end();
            ofPopStyle();
            ofPopView();
        }
    }
    else{
        ofPushMatrix();
        ofTranslate(512*scale, 0);
        ofScale(-scale, scale);
        depthShader.begin();
        depthTex.draw(0, 0);
        depthShader.end();
        if(isRecording){
            ofSaveImage(depthPix, save_dir+"/"+current_time+".tif");
            ofPushStyle();
            ofSetColor(255,0,0);
            ofSetLineWidth(10);
            ofNoFill();
            ofDrawRectangle(2, 2, 512-4, 424-4);
            ofPopStyle();
        }
        ofPopMatrix();
    }
    
    ofDrawBitmapStringHighlight("Fps: " + ofToString(ofGetFrameRate()), 10, 900);
    ofDrawBitmapStringHighlight("CurrentTime : " + current_time, 10, 920);
    if(isRecording){
        ofPushStyle();
        ofSetColor(255,0,0);
        ofDrawBitmapString("Recording in \"" + save_dir + "\"", 10, 940);
        ofPopStyle();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='p'){
        if(!isRecording) ispointcloud = !ispointcloud;
    }
    if(key=='r'){
        if(!ispointcloud){
            if(!isRecording){
                // start recording
                save_dir = output_dir + "/" + current_time;
                ofDirectory dir;
                dir.createDirectory(save_dir);
                isRecording = true;
            }
            else{
                // stop recording
                save_dir = "";
                isRecording = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
