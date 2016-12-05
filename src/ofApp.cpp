#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetEscapeQuitsApp(false);
#ifdef TARGET_OPENGLES
	crtShader.load("crtES/shader");
#else
	ofDisableArbTex();
	crtShader.load("crt/shader");
#endif
	fbo.allocate(ofGetWidth(), ofGetHeight());
	passwordBuffer = "";
	found = false;
	shouldDrawCursor = true;
	shouldReset = false;
	// sounds
	vroum.load("sounds/vroum.wav");
	stop.load("sounds/stop.wav");
}

//--------------------------------------------------------------
// void ofApp::setupGPIOs(){
// 	previousButtonState = true;
// 	currentButtonState = true;
// 	// setup wiring pi
// 	if(wiringPiSetup() == -1){
// 		ofLogNotice(__func__) << "Error on wiringPi setup";
// 	}
// 	// relay pin
// 	pinMode(RELAY_PIN, OUTPUT);
// 	digitalWrite(RELAY_PIN, HIGH);
// 	// button pin
// 	pinMode(BUTTON_PIN, INPUT);
// 	pullUpDnControl(BUTTON_PIN, PUD_UP);
// }

//--------------------------------------------------------------
void ofApp::update(){
	// deactivate the relay if password is found
	if (found) {
		found = false;
		// digitalWrite(RELAY_PIN, LOW);
		ofLog(OF_LOG_NOTICE, "Relay off");
		vroum.setLoop(true);
		vroum.play();
	}
	// reset the program
	if (shouldReset) {
		shouldReset = false;
		found = false;
		// digitalWrite(RELAY_PIN, HIGH);
		ofLog(OF_LOG_NOTICE, "Relay on");
		attempts.clear();
		vroum.stop();
		stop.play();
	}
	// currentButtonState = digitalRead(BUTTON_PIN);
	// // button is on
	// if (currentButtonState != previousButtonState && !currentButtonState) {
	// 	attempts.clear();
	// 	vroum.stop();
	// 	stop.play();
	// }

}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.begin();
	ofSetColor(0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
	fbo.begin();
	ofSetColor(50, 50, 255);
	drawHeader();
	drawAttempts();
	drawBuffer();
	drawCursor();
	fbo.end();
	crtShader.begin();
	crtShader.setUniform1f("currentTime", ofGetElapsedTimeMillis());
	crtShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	fbo.draw(0, 0);
	crtShader.end();
}

//--------------------------------------------------------------
void ofApp::drawHeader(){
	ofDrawBitmapString("***********************************", 0, LINE_HEIGHT);
	ofDrawBitmapString("* Veuillez entrer le mot de passe *", 0, 2*(LINE_HEIGHT));
	ofDrawBitmapString("***********************************", 0, 3*(LINE_HEIGHT));
}

//--------------------------------------------------------------
void ofApp::drawAttempts(){
	int offset = 3*(LINE_HEIGHT);
	// only handle the last 13 attempts in order to fit the GUI
	if (attempts.size() > 14) attempts.erase(attempts.begin());
	// draw the last 13 or less attempts
	for (int i = 0; i < attempts.size(); i++){
		offset += LINE_HEIGHT;
		ofDrawBitmapString(">" + attempts[i].password, 0, offset);
		offset += LINE_HEIGHT;
		if (attempts[i].found) {
			ofSetColor(50, 255, 50);
			ofDrawBitmapString("/!\\ Mot de passe OK, deverouillage /!\\", 0, offset);
		}
		else {
			ofSetColor(255, 50, 50);
			ofDrawBitmapString("/!\\ Erreur systeme /!\\", 0, offset);
		}
		ofSetColor(50, 50, 255);
	}
}

//--------------------------------------------------------------
void ofApp::drawBuffer(){
	ofDrawBitmapString(">" + passwordBuffer, 0, 3*(LINE_HEIGHT) + attempts.size()*2*(LINE_HEIGHT) + LINE_HEIGHT);
}

//--------------------------------------------------------------
void ofApp::drawCursor(){
	int currentTime = ofGetElapsedTimeMillis();
	if (currentTime - timeElapsed > INTERVAL) {
		shouldDrawCursor = !shouldDrawCursor;
		timeElapsed = currentTime;
	}
	if (shouldDrawCursor) {
		ofDrawRectangle(
			getBitmapStringBoundingBox(passwordBuffer).getWidth() + 8 + 2,
			2*(LINE_HEIGHT) + attempts.size()*2*(LINE_HEIGHT) + LINE_HEIGHT + LINE_OFFSET,
			8,
			FONT_HEIGHT
		);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLog(OF_LOG_NOTICE, ofToString(key));
	// if we didn't hit return, add the key to our string
	if (key != OF_KEY_RETURN) {
		// some trickery: ignore the backspace key
		if (key == OF_KEY_BACKSPACE && passwordBuffer.size() > 0) passwordBuffer.erase(passwordBuffer.end() - 1);
		else if (!isSpecialChar(key)) passwordBuffer += key;
	}
	// hit Return, check password
	else {
		ofLog(OF_LOG_NOTICE, passwordBuffer);
		attempt currentAttempt;
		currentAttempt.password = passwordBuffer;
		if (passwordBuffer == "resetTheMatrix") shouldReset = true;
		else if (passwordBuffer == "toto") {
			found = true;
			currentAttempt.found = true;
		}
		else currentAttempt.found = false;
		attempts.push_back(currentAttempt);
		passwordBuffer = "";
	}
}

//--------------------------------------------------------------
bool ofApp::isSpecialChar(int key) {
	switch (key) {
		case OF_KEY_ESC:
		case OF_KEY_TAB:
		case OF_KEY_DEL:
		case OF_KEY_BACKSPACE:
		case OF_KEY_F1:
		case OF_KEY_F2:
		case OF_KEY_F3:
		case OF_KEY_F4:
		case OF_KEY_F5:
		case OF_KEY_F6:
		case OF_KEY_F7:
		case OF_KEY_F8:
		case OF_KEY_F9:
		case OF_KEY_F10:
		case OF_KEY_F11:
		case OF_KEY_F12:
		case OF_KEY_LEFT:
		case OF_KEY_UP:
		case OF_KEY_RIGHT:
		case OF_KEY_DOWN:
		case OF_KEY_PAGE_UP:
		case OF_KEY_PAGE_DOWN:
		case OF_KEY_HOME:
		case OF_KEY_END:
		case OF_KEY_INSERT:
		case OF_KEY_CONTROL:
		case OF_KEY_ALT:
		case OF_KEY_SHIFT:
		case OF_KEY_SUPER:
		case OF_KEY_LEFT_SHIFT:
		case OF_KEY_RIGHT_SHIFT:
		case OF_KEY_LEFT_CONTROL:
		case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_LEFT_ALT:
		case OF_KEY_RIGHT_ALT:
		case OF_KEY_LEFT_SUPER:
		case OF_KEY_RIGHT_SUPER:
			return true;
			break;
		default:
			return false;
	}
}

//--------------------------------------------------------------
ofRectangle ofApp::getBitmapStringBoundingBox(string text) {
	vector<string> lines = ofSplitString(text, "\n");
	int maxLineLength = 0;
	for(int i = 0; i < (int)lines.size(); i++) {
		// tabs are not rendered
		const string & line(lines[i]);
		int currentLineLength = 0;
		for(int j = 0; j < (int)line.size(); j++) {
			if (line[j] == '\t') {
				currentLineLength += 8 - (currentLineLength % 8);
			}
			else {
				currentLineLength++;
			}
		}
		maxLineLength = MAX(maxLineLength, currentLineLength);
	}

	int fontSize = 8;
	float leading = 1.7;
	int height = lines.size() * fontSize * leading - 1;
	int width = maxLineLength * fontSize;
	return ofRectangle(0, 0, width, height);
}
