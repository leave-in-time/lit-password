#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetEscapeQuitsApp(false);
	ofHideCursor();
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
	// gpio setup
	setupGPIOs();
	// prevent SIGINT, SIGTERM and SIGKILL
	// signal(SIGINT, &ofApp::sighandler);
	// signal(SIGTERM, &ofApp::sighandler);
	// signal(SIGKILL, &ofApp::sighandler);
}

//--------------------------------------------------------------
void ofApp::setupGPIOs(){
	previousButtonState = true;
	currentButtonState = true;
	// setup wiring pi
	if(wiringPiSetup() == -1){
		ofLogNotice(__func__) << "Error on wiringPi setup";
	}
	// relay pin
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);
	// button pin
	pinMode(BUTTON_PIN, INPUT);
	pullUpDnControl(BUTTON_PIN, PUD_UP);
}

//--------------------------------------------------------------
void ofApp::sighandler(int signal) {
	// noop
	ofLog(OF_LOG_NOTICE, "SIGNAL!!");
}

//--------------------------------------------------------------
void ofApp::update(){
	// deactivate the relay if password is found
	if (found) {
		found = false;
		digitalWrite(RELAY_PIN, LOW);
		ofLog(OF_LOG_NOTICE, "Relay off");
		vroum.setLoop(true);
		vroum.play();
	}
	// reset the program
	if (shouldReset) {
		shouldReset = false;
		found = false;
		digitalWrite(RELAY_PIN, HIGH);
		ofLog(OF_LOG_NOTICE, "Relay on");
		attempts.clear();
		vroum.stop();
		stop.play();
	}
	currentButtonState = digitalRead(BUTTON_PIN);
	// button is on
	if (currentButtonState != previousButtonState && !currentButtonState) {
		shouldReset = true;
	}
	previousButtonState = currentButtonState;
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
	// if we didn't hit return, add the key to our string
	if (key != OF_KEY_RETURN) {
		// some trickery: ignore the backspace key
		if (key == OF_KEY_BACKSPACE && passwordBuffer.size() > 0) passwordBuffer.erase(passwordBuffer.end() - 1);
		else if (!isSpecialChar(key)) passwordBuffer += getLocaleKey(key);
	}
	// hit Return, check password
	else {
		attempt currentAttempt;
		currentAttempt.password = passwordBuffer;
		if (passwordBuffer == "resetTheMatrix") shouldReset = true;
		else if (passwordBuffer == "NNHLRCRB") {
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
		// disable some bad keys
		case 0:
		case 1:
		case 42:
		case 255:
			return true;
		default:
			return false;
	}
}

//--------------------------------------------------------------
int ofApp::getLocaleKey(int key) {
	switch (key) {
		// 1 to 0 with shift
		case 33: return '1'; break;
		case 64: return '2'; break;
		case 35: return '3'; break;
		case 36: return '4'; break;
		case 37: return '5'; break;
		case 94: return '6'; break;
		case 38: return '7'; break;
		case 42: return '8'; break;
		case 40: return '9'; break;
		case 41: return '0'; break;
		// 1 to 0 without shift
		case 49: return '&'; break;
		case 50: return 'e'; break;
		case 51: return '"'; break;
		case 52: return '\''; break;
		case 53: return '('; break;
		case 54: return '-'; break;
		case 55: return 'e'; break;
		case 56: return '_'; break;
		case 57: return 'c'; break;
		case 48: return 'a'; break;
		// )/° key
		case 45: return ')'; break;
		case 95: return '°'; break;
		// next to "return" key
		case 91: return '^'; break;
		case 123: return '^'; break;
		case 93: return '$'; break;
		case 125: return '$'; break;
		case 39: return 'u'; break;
		case 34: return '%'; break;
		case 92: return '*'; break;
		case 59: return 'm'; break;
		case 58: return 'M'; break;
		case 109: return ','; break;
		case 77: return '?'; break;
		case 44: return ';'; break;
		case 60: return '.'; break;
		case 46: return ':'; break;
		case 62: return '/'; break;
		case 47: return '!'; break;
		case 63: return '!'; break;
		// qwerty --> azerty
		case 113: return 'a'; break;
		case 81: return 'A'; break;
		case 119: return 'z'; break;
		case 87: return 'Z'; break;
		case 122: return 'w'; break;
		case 90: return 'W'; break;
		case 97: return 'q'; break;
		case 65: return 'Q'; break;
		// ² key
		case 10: return '²'; break;
		// defaults
		default: return key; break;
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
