#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.width = 1920;
	settings.height = 1080;
	settings.windowMode = OF_WINDOW;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
#else
	ofGLWindowSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.setGLVersion(3,2);
	ofCreateWindow(settings);
#endif
	ofRunApp(new ofApp());

}
