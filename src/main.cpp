#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	int width = 800;
	int height = 600;
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.width = width;
	settings.height = height;
	settings.setGLESVersion(2);
	ofCreateWindow(settings);
#else
	ofGLWindowSettings settings;
	settings.width = width;
	settings.height = height;
	settings.setGLVersion(3,2);
	ofCreateWindow(settings);
#endif
	ofRunApp(new ofApp());

}
