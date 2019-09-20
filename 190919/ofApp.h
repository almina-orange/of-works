#pragma once

#include "ofMain.h"
#include "algorithm"

class ofApp : public ofBaseApp{

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

		void drawWindow(int x, int y, int w, int h);
		void setPoints();
		void updatePoints();
		ofVec3f rect_hyperbola(float n);
		ofVec3f astroid(float n);
		ofVec3f vexp(ofVec3f v, float weight);

		ofShader shader, effect;

		ofFbo fbo;
		ofImage ibo;

		int num;
		ofVec3f *p;
		ofVboMesh mesh;

		ofImage img;
		vector<ofColor> pal;
		float seed;
		bool bUpdate;
		float noiseArg;
};
