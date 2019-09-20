#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(255);
    // ofSetBackgroundAuto(false);
    ofSetColor(0, 5, 25);
    ofSetLineWidth(2);
    ofSetCircleResolution(360);

    pal = {
        ofColor::fromHex(0xf9f9f9),
        ofColor::fromHex(0x0cce6b),
        ofColor::fromHex(0xdced31),
        ofColor::fromHex(0xef2d56),
        ofColor::fromHex(0x140d4f),
    };

    ofSetBackgroundColor(pal[4]);

    shader.load("", "shader.frag");
    effect.load("", "effect.frag");

    mesh.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(0.5);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA,GL_ONE);

    num = 1e+5;
    setPoints();

    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();  ofClear(pal[4]);  fbo.end();

    seed = ofRandom(1e+4);
    bUpdate = true;
    noiseArg = ofRandom(1e+4);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSeedRandom(seed);
    ofRandomuf();

    updatePoints();
}
 
//--------------------------------------------------------------
void ofApp::draw(){
    /**
     * main visual
     */
    fbo.begin();

    // -- vector field (flare)
    // mesh.draw();
    shader.begin();
    shader.setUniform2f("resolution", 720, 720);
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform2f("mouse", mouseX, mouseY);
    shader.setUniform1i("hex1", pal[0].getHex());
    shader.setUniform1i("hex2", pal[1].getHex());
    shader.setUniform1i("hex3", pal[2].getHex());
    shader.setUniform1i("hex4", pal[3].getHex());
    shader.setUniform1i("hex5", pal[4].getHex());
    mesh.draw();
    shader.end();

    // -- clip (for drawing outline)
    ofSetColor(pal[4]);
    ofDrawCircle(360, 360, 85);


    // -- fragment shader
    shader.begin();

    shader.setUniform2f("resolution", 720, 720);
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform2f("mouse", mouseX, mouseY);
    shader.setUniform1i("hex1", pal[0].getHex());
    shader.setUniform1i("hex2", pal[1].getHex());
    shader.setUniform1i("hex3", pal[2].getHex());
    shader.setUniform1i("hex4", pal[3].getHex());
    shader.setUniform1i("hex5", pal[4].getHex());

    ofDrawCircle(360, 360, 80);

    shader.end();

    fbo.end();


    /**
     * post effect
     */
    effect.begin();

    effect.setUniform2f("resolution", 720, 720);
    effect.setUniform1f("time", ofGetElapsedTimef());
    effect.setUniform2f("mouse", mouseX, mouseY);
    effect.setUniformTexture("texture", fbo, 0);
    effect.setUniform1f("ang", ofRandom(TWO_PI));
    effect.setUniform1f("bias", ofRandom(-200, 200));
    // effect.setUniform1f("ang", TWO_PI / 360 * 130);
    // effect.setUniform1f("bias", 0);

    ofDrawRectangle(0, 0, 720, 720);
    effect.end();

    drawWindow(20, 20, 720 - 20 * 2, 720 - 20 * 2);

    noiseArg += 1e-3;
}

//--------------------------------------------------------------
void ofApp::drawWindow(int x, int y, int w, int h){
    ofFill();
    ofSetColor(255);
    ofBeginShape();
    ofVertex(0, 0);
    ofVertex(720, 0);
    ofVertex(720, 720);
    ofVertex(0, 720);
    ofVertex(0, 0);
    ofVertex(x, y);
    ofVertex(x + w, y);
    ofVertex(x + w, y + h);
    ofVertex(x, y + h);
    ofVertex(x, y);
    ofEndShape();
}

//--------------------------------------------------------------
void ofApp::setPoints(){
    num = 720 * 20;
    p = new ofVec3f[num];
    float angle = TWO_PI / 720;
    for (int j = 0; j < 20; j++) {
        for (int i = 0; i < 720; i++) {
            p[j * 720 + i] = ofVec3f(cos(angle * i) * (90 - j * 2) + 360, sin(angle * i) * (90 - j * 2) + 360);
        }
    }
}

//--------------------------------------------------------------
void ofApp::updatePoints() {
    for (int i = 0; i < num; i++) {
        float n = TWO_PI * ofNoise(p[i].x * 1e-2, p[i].y * 1e-2, noiseArg);
        ofVec3f v = ofVec3f(cos(n + HALF_PI), sin(n + HALF_PI));

        p[i] += v;
    }

    mesh.clear();
    for (int i = 0; i < num; i++) { mesh.addVertex(p[i]);  mesh.addColor(ofColor(pal[0], 10)); }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        img.grabScreen(0, 0, 720, 720);
        img.save("screenshot.png");
    } else if (key == ' ') {
        random_shuffle(pal.begin(), pal.end());

        ofSetColor(pal[4]);
        ofDrawRectangle(0, 0, 720, 720);

        fbo.begin();  ofClear(pal[4]);  fbo.end();
        setPoints();

        seed = ofRandom(1e+4);
        bUpdate = true;
        noiseArg = ofRandom(1e+4);
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
