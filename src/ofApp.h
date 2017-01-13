#pragma once

#include "ofMain.h"
#ifdef TARGET_OPENGLES
#include "wiringPi.h"
#endif
#include <signal.h>

#define FONT_HEIGHT 12
#define LINE_OFFSET 6
#define LINE_HEIGHT 18
#define INTERVAL 750
#define STARS "*********************"
#define EMPTY_STARS "*                   *"
#define DANGER "*      DANGER       *"
#define ALERT "* ALERTE COLLISION  *"
#define INCENTIVE "SAISISSEZ MOT DE PASSE:"
#define OK "ACCES SALLE DES MACHINES AUTORISE, ARRETEZ LES MOTEURS POUR EVITER LA COLLISION!"
#define KO "MOT DE PASSE INVALIDE"

struct attempt {
	string password;
	bool found;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
	#ifdef TARGET_OPENGLES
		void setupGPIOs();
	#endif
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
		int headerOffset;
		int promptOffset;
		bool found;
		uint64_t cursorElapsed;
		bool shouldDrawCursor;
		uint64_t headerElapsed;
		bool shouldDrawHeader;
		bool shouldReset;

		const static int RELAY_PIN = 2;
		const static int BUTTON_PIN = 0;
		bool previousButtonState;
		bool currentButtonState;

		ofSoundPlayer vroum;
		ofSoundPlayer stop;
};
