#include "ofMain.h"
#include "application.h"


//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
	settings.resizable = false;
	settings.setSize(1440,900);
	ofCreateWindow(settings);
	return ofRunApp(new Application);

}
