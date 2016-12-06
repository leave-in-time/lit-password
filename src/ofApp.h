#pragma once

#include "ofMain.h"
#include "wiringPi.h"
#include <signal.h>

#define FONT_HEIGHT 12
#define LINE_OFFSET 6
#define LINE_HEIGHT 18
#define INTERVAL 750

struct attempt {
	string password;
	bool found;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void setupGPIOs();
		void update();
		void draw();

		void keyPressed(int key);
		bool isSpecialChar(int key);
		int getLocaleKey(int key);
		static void sighandler(int signal);

		void drawHeader();
		void drawAttempts();
		void drawBuffer();
		void drawCursor();

		ofRectangle getBitmapStringBoundingBox(string text);

		ofShader crtShader;
		ofFbo fbo;

		vector<attempt> attempts;
		string passwordBuffer;
		bool found;
		uint64_t timeElapsed;
		bool shouldDrawCursor;
		bool shouldReset;

		const static int RELAY_PIN = 2;
		const static int BUTTON_PIN = 0;
		bool previousButtonState;
		bool currentButtonState;

		ofSoundPlayer vroum;
		ofSoundPlayer stop;
};
