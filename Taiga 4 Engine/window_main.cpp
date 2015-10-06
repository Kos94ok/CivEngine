
#include "main.h"
#include "window.h"
#include "settings.h"
#include "math.h"
#include "camera.h"
#include "visual.h"

void windowMain()
{
	int threadId = 0;
	console << "[WND] Window thread started" << " [ID: " << threadId << "]" << "\n";
	srand(time(0));
	sf::Uint32 screenMode;
	sf::ContextSettings context;
	context.antialiasingLevel = settings.antialiasingLevel;
	// Setting up window options
	if (settings.screenMode == 0) { screenMode = sf::Style::Default; }
	else if (settings.screenMode == 1) {
		screenMode = sf::Style::None;
		camera.res.x = window.getScreenSize().x;
		camera.res.y = window.getScreenSize().y;
	}
	else if (settings.screenMode == 2) { screenMode = sf::Style::Fullscreen; }
	// Checking the max texture size
	bool resChanged = false;
	if (visual.maxTextureSize < camera.res.x) { camera.res.x = visual.maxTextureSize; resChanged = true; }
	if (visual.maxTextureSize < camera.res.y) { camera.res.y = visual.maxTextureSize; resChanged = true; }
	if (resChanged) {
		window.showWarning(MSG_TEXTURESIZE_LIMIT);
		if (settings.sampleMod > 1.00f) { settings.sampleMod = 1.00f; }
	}
	// Creating the window
	console << "[WND] Creating " << camera.res.x << "x" << camera.res.y << " window rendering at " << math.round(camera.res.x * settings.sampleMod) << "x" << math.round(camera.res.y * settings.sampleMod) << endl;
	window.winHandle.create(sf::VideoMode(camera.res.x, camera.res.y), NAME_FULL + " " + NAME_VERSION_FULL + " [" + NAME_ENGINE + "]", screenMode, context);
	sf::Image icon;
	icon.loadFromFile("Data//Textures//icon32.png");
	window.winHandle.setIcon(32, 32, icon.getPixelsPtr());
	// Framerate limit
	window.winHandle.setFramerateLimit(settings.framerateLimit);
	// Server mode
	if (core.serverMode) {
		window.winHandle.setFramerateLimit(30);
		window.winHandle.setTitle(NAME_FULL + " Server " + NAME_VERSION_FULL + " [" + NAME_ENGINE + "]");
	}
	// Creating the textures
	window.texHandle.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleShadow.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleLight.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleLightMult.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleTop.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.winHandle.setVerticalSyncEnabled(math.intToBool(settings.enableVertSync));
	window.updateAspectRatio();
	// Set the pointer
	visual.setMousePointer(POINTER_DEFAULT);
	
	console << "[WND] Starting the main loop" << "\n";
	while (!core.thread_shutdown[threadId])
	{
		window.mainEvent();
		window.mainPaint();
		core.thread_antifreeze[threadId] = 0;
	}
	window.winHandle.close();
	console << "[WND] Cleaning up" << "\n";
}