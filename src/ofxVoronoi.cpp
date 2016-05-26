#include "ofxVoronoi.h"

//static int cellColors[10000];
//--------------------------------------------------------------
ofxVoronoi::ofxVoronoi() {
    minDistance = 3;
    //for(int i=0; i<10000; i++) cellColors[i] = ofRandom(255);
}

//--------------------------------------------------------------
ofxVoronoi::~ofxVoronoi() {
    
}


void ofxVoronoi::setup() {
    setup(0,0,ofGetWidth(),ofGetHeight());
}

void ofxVoronoi::setup(const ofRectangle &rect) {
    setup(rect.x, rect.y, rect.width, rect.height);
}

void ofxVoronoi::setup(float x, float y, float w, float h) {
    v2d.setup(x/w,
              x/w+1,
              y/h,
              y/h+1,10,10,16);
    bounds.set(x,y,w,h);
    ofLog() << "settting up thing!";
}



//--------------------------------------------------------------
void ofxVoronoi::clear() {
    cells.clear();
    pts.clear();
    v2d.clear();
}

//--------------------------------------------------------------
void ofxVoronoi::setMinDistance(float minDis) {
    minDistance = minDis;
}

//--------------------------------------------------------------
void ofxVoronoi::addPoint(float x, float y) {
    pts.addVertex(x/bounds.width, y/bounds.height);
}

//--------------------------------------------------------------
void ofxVoronoi::addPoint(const ofVec2f &pt) {
    addPoint(pt.x, pt.y);    
}

//--------------------------------------------------------------
ofRectangle ofxVoronoi::getBounds() {
    return pts.getBoundingBox();
}

//--------------------------------------------------------------
vector<ofPoint>& ofxVoronoi::getPoints() {
    return pts.getVertices();    
}

//--------------------------------------------------------------
void ofxVoronoi::generateVoronoi() {
    
    for (int i=0; i<pts.size(); i++) {
        v2d.put(i, pts[i].x, pts[i].y);
    }
    
    float x = 0;
    float y = 0;
    float w = bounds.width;
    float h = bounds.height;
    
    if(v2d.start()) {
        do {
            if(v2d.computeCell()) {
                cells.push_back(ofxVoronoiCell());
                do {
                    v2d.getCellPoint(x,y);
                    cells.back().pts.push_back(ofVec2f(x*w,y*h));
                    //cells.back().outline.addVertex(x*w,y*h);
                } while(v2d.cellHasEdges());
                //cells.back().outline.close();
            }
        } while(v2d.next());
        
    }
}

//--------------------------------------------------------------
void ofxVoronoi::generateSmoothVoronoi() {

    for (int i=0; i<pts.size(); i++) {
        v2d.put(i, pts[i].x, pts[i].y);
    }
    
    float x = 0;
    float y = 0;
    float w = bounds.width;
    float h = bounds.height;
    ofPoint prevPt;
    prevPt.set(-1, -1);
    int btwnPts = 10;
    
    int ptCount = 0;
    
    if(v2d.start()) {
        do {
            if(v2d.computeCell()) {
                cells.push_back(ofxVoronoiCell());
                do {
                    v2d.getCellPoint(x,y);
                    
                    if(ptCount>0){
                        //Add extra points between each vertex for the purposes of smoothing them later
                        for(int i=0; i<btwnPts; i++){
                            
                            float middlePtX = ofMap(i, 0, btwnPts, prevPt.x,x);
                            float middlePtY = ofMap(i, 0, btwnPts, prevPt.y,y);

                            cells.back().pts.push_back(ofVec2f(middlePtX*w,middlePtY*h));
                        }
                    }else{
                        cells.back().pts.push_back(ofVec2f(x*w,y*h));
                    }
                    ptCount++;
                    prevPt.set(x, y);
                } while(v2d.cellHasEdges());
                prevPt.set(-1,-1);
                ptCount = 0;
            }
        } while(v2d.next());
        
    }
}


//--------------------------------------------------------------
void ofxVoronoi::draw() {
    
    for(int i=0; i<cells.size(); i++) {
        
        ofPolyline p;
        p.addVertices(cells[i].pts);
        p.draw();
    }
}
//--------------------------------------------------------------
void ofxVoronoi::drawSmooth(int _smoothAmt, bool _fill) {
    
    if(_fill){
        
            for(int i=0; i<cells.size(); i++) {
                ofPolyline p;
                p.addVertices(cells[i].pts);
                p.setClosed(true); //close before smoothing
                ofPolyline pSmooth = p.getSmoothed(_smoothAmt);
                ofPath path;
                for( int j = 0; j< pSmooth.getVertices().size(); j++) {
                    ofPoint temp;
                    temp.x = pSmooth.getVertices()[j].x;
                    
                    temp.y = pSmooth.getVertices()[j].y;
                    
                    //cout<<asin(ofMap(pSmooth.getVertices()[j].y, 0 ,ofGetHeight(), -1,1))<<endl;
                    
                    temp.z = 300*cos(2*ofGetElapsedTimef()+ofMap(temp.x, 0,ofGetWidth(), 0, 2*PI));
                    //temp.z = 100*ofSignedNoise(2*ofGetElapsedTimef()+ofMap(temp.y, 0,ofGetWidth(), 0, 2*PI))+100*ofSignedNoise(2*ofGetElapsedTimef()+ofMap(temp.x, 0,ofGetWidth(), 0, 2*PI));
                 
                    if(j == 0) {
                        path.newSubPath();
                        path.moveTo(temp );
                    } else {
                        path.lineTo( temp );
                    }
                }
                
                path.close();
                //path.simplify();
                //ofSetColor(ofMap(i, 0,cells.size(), 0,255));
                
                ofFill();
                path.setColor(ofColor(255,255,255,100));
                //path.setColor(ofMap(i, 0,cells.size(), 0,255));
                //path.setStrokeWidth(ofMap(i, 0,cells.size(), 0,10));
                path.setStrokeWidth(1);
                path.setStrokeColor(ofColor::black);
                
                path.draw();
              
            }
        

    }else{
        
        for(int i=0; i<cells.size(); i++){
            ofPolyline p;
            p.addVertices(cells[i].pts);
            p.setClosed(true); //close before smoothing
            ofPolyline pSmooth = p.getSmoothed(_smoothAmt);
            pSmooth.draw();
        }
        /*
         //Sanity check for position of vertices
         for (int j=0; j<cells[i].pts.size(); j++) {
         ofDrawCircle(cells[i].pts[j].x, cells[i].pts[j].y, 2);
         }
         */

    }
}
//--------------------------------------------------------------
void ofxVoronoi::drawMesh(int _smoothAmt , bool _fill){
    
    if(_fill){
        
        for(int i=0; i<cells.size(); i++) {
            ofPolyline p;
            p.addVertices(cells[i].pts);
            p.setClosed(true); //close before smoothing
            ofPolyline pSmooth = p.getSmoothed(_smoothAmt);
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
            ofSetLineWidth(1);
            for( int j = 0; j< pSmooth.getVertices().size(); j++) {
                ofPoint temp;
                temp.x = pSmooth.getVertices()[j].x;
                //temp.y = pSmooth.getVertices()[j].y;
                //temp.x = 200*sin(ofGetElapsedTimef()+ofMap(pSmooth.getVertices()[j].x, 0, ofGetWidth(),0,2*PI));
                
                temp.y = pSmooth.getVertices()[j].y;

                //temp.z = 50*ofSignedNoise(ofGetElapsedTimef()+ofMap(pSmooth.getVertices()[j].y, 0, ofGetHeight(),0,2*PI))+50*ofSignedNoise(ofGetElapsedTimef()+ofMap(pSmooth.getVertices()[j].x, 0, ofGetWidth(),0,2*PI));
                
                temp.z = 200*sin(ofMap(pSmooth.getVertices()[j].y, 0, ofGetHeight(),0,2*PI));
                
                
                mesh.addVertex(temp);
            }
            
            mesh.draw();
            
        }
        
        
    }else{
        
        for(int i=0; i<cells.size(); i++){
            ofPolyline p;
            p.addVertices(cells[i].pts);
            p.setClosed(true); //close before smoothing
            ofPolyline pSmooth = p.getSmoothed(_smoothAmt);
            pSmooth.draw();
        }
        /*
         //Sanity check for position of vertices
         for (int j=0; j<cells[i].pts.size(); j++) {
         ofDrawCircle(cells[i].pts[j].x, cells[i].pts[j].y, 2);
         }
         */
        
    }
}
