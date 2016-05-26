#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    
    bounds.set(0,0, ofGetWidth(), ofGetHeight());
    voronoi.setup(bounds);
    
    
    int n = 100;
    for(int i=0; i<n; i++) {
        ofPoint pt(bounds.x + ofRandom(bounds.width), bounds.y + ofRandom(bounds.height));// ofRandomWidth(), ofRandomHeight());
        pts.push_back(pt);
    }
    
    //generateTheVoronoi();
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    
  
    voronoi.clear();
    

    for(int i=0; i<pts.size(); i++) {
        ofPoint modPt;
        if(ofGetKeyPressed('0')){
        modPt.x = pts[i].x + 100*ofSignedNoise(ofGetElapsedTimef()+50+ i*50);
        modPt.y =pts[i].y + 100*ofSignedNoise(ofGetElapsedTimef()+50+ i*20);
            voronoi.addPoint(modPt);
        }else{
        pts[i].x = ofClamp(pts[i].x + 20*ofSignedNoise(0.02*ofGetElapsedTimef()+50+ i*20), 0,ofGetWidth());
        pts[i].y = ofClamp(pts[i].y + 20*ofSignedNoise(0.02*ofGetElapsedTimef()+50+ i*50), 0, ofGetHeight());
            voronoi.addPoint(pts[i]);
        }

        //voronoi.addPoint(pts[i]);
    }
    voronoi.generateVoronoi();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // the bounds of all the points
    ofSetColor(255);
    ofNoFill();
    //ofRect(bounds);
    
    
    // draw the raw points
    for(int i=0; i<pts.size(); i++) {
        ofSetColor(255);
        //ofCircle(pts[i], 2);
    }
    
    int numCopies = 1;
    bool smoothFlip;
    ofEnableAlphaBlending();
    fbo.begin();
    fadeAmnt = 255;
    if(ofGetKeyPressed('1')){
        fadeAmnt = 1;
    }else if(ofGetKeyPressed('2')){
        fadeAmnt = 5;
    }else if(ofGetKeyPressed('3')){
        fadeAmnt = 15;
    }else if(ofGetKeyPressed('4')){
        fadeAmnt = 50;
    }else if(ofGetKeyPressed('5')){
        fadeAmnt = 255;
    }
    
    //1 - Fade Fbo
    
    //this is where we fade the fbo
    //by drawing a rectangle the size of the fbo with a small alpha value, we can slowly fade the current contents of the fbo.
    ofFill();
    ofSetColor(0,0,0, fadeAmnt);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    smoothFlip = true;
    for (int i=0; i<numCopies; i++){
        ofSetColor(ofMap(i,0,numCopies,255,255));
        ofPushMatrix();
        if(i>0){
        //ofTranslate(ofGetWidth(),0);
        }
        //ofTranslate(i*ofGetMouseX(), i*ofGetMouseY());
//ofTranslate(0,0,i*5*ofGetMouseX());
        
        //smoothFlip = !smoothFlip;
        //ofRotateZ(90*i);
        ofFill();
        voronoi.draw(smoothFlip);
        ofPopMatrix();
    }
    fbo.end();
    
    fbo.draw(0,0);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if(ofGetFrameNum()%3==0) {
        pts.erase(pts.begin());
        pts.push_back(ofPoint(x, y));
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    pts.erase(pts.begin());
    pts.push_back(ofPoint(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
