
#include "main.h"

#include "math.h"
#include "util.h"
#include "chat.h"
#include "console.h"
#include "prerender.h"
#include "window.h"
#include "visual.h"
#include "icon.h"
#include "refSystem.h"
#include "audio.h"
#include "item.h"
#include "UI.h"
#include "target.h"
#include "path.h"
#include "ai.h"
#include "animation.h"
#include "unit.h"
#include "game.h"
#include "database.h"
#include "craft.h"
#include "client.h"
#include "server.h"
#include "camera.h"
#include "settings.h"
#include "saveload.h"
#include "world.h"
#include "overworld.h"
#include "editor.h"
#include "logic.h"
#include "script.h"
#include "api.h"
#include "weather.h"
#include "particle.h"

cCore core;
cValue value;
cMath math;
cChat chat;
cConsole console;
cPreRender preRender;
cWindow window;
cVisual visual;
cAudio audio;
cUI ui;
cTarget target;
cPath path;
cGame game;
cDatabase database;
cCraft craft;
cServer server;
cClient client;
cCamera camera;
cSettings settings;
cSave save;
cWorld world;
cOverworld overworld;
cEditor editor;
cUtil util;
cScript script;
cGameLogic gamelogic;
cCodeStats codeStats;
cAI ai;
cAPI api;
cMutexGlobal mutex;
cWeather weather;
cParticle particle;
cUtilTimer utilTimer;

/*
TODO:
- Script message (event) system?
- Multiple levels of enemies based on the Slender-style system

- Add order confirmation sfx
- Add bigger item icon to description
- Add enabled/disabled campfire states
- Add context menu based campfire heal with items
- Campfire should be built in disabled state
- Fix tree death rotation
- Add proper death animations for trees and stones
- Fix item bag shadow
- Fix interact bug (when character moves past the object before interacting)
- Add berries and mushrooms
- Finish overworld
- Add camera scrolling speed settings for all types
- Finish with rabbit AI!
- Add wolf
- Add animal spawn
*/

int main(int argc, char* argv[])
{
	game.access.lock();
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-server") == 0) { core.serverMode = true; }
	}
	util.checkLogFiles();
	console << "[MAIN] Main thread started" << "\n";
	console << "[MAIN] Parsed [" << argc << "] arguments" << "\n";
	// Random initialization
	console << "[MAIN] Initializing" << "\n";
	srand(time(0));
	timeBeginPeriod(5);
	if (!core.serverMode) { core.localServer = true; }
	util.detectVideoCard();
	util.checkLaunchStatus();

	// Loading databases
	console << "[MAIN] Parsing settings.ini" << "\n";
	settings.setDefault();
	settings.load();
	console << "[MAIN] Initializing main database" << "\n";
	database.init();
	console << "[MAIN] Loading craft recipes" << "\n";
	craft.loadRecipes();
	console << "[MAIN] Initializing visual data" << "\n";
	visual.init();
	database.generateTextures();
	console << "[MAIN] Analyzing world blueprints" << "\n";
	world.analyzeBlueprints();

	// Starting the threads
	console << "[MAIN] Starting the threads\n";
	thread threadWindow(windowMain);					Sleep(10);
	thread threadGameLogic(gameLogicMain);				Sleep(10);
	thread threadServerConnect(serverConnectMain);		Sleep(10);
	thread threadServerReceive(serverReceiveMain);		Sleep(10);
	thread threadServerSend(serverSendMain);			Sleep(10);
	thread threadClientReceive(clientReceiveMain);		Sleep(10);
	thread threadClientSend(clientSendMain);			Sleep(10);
	thread threadWorldLoader(worldLoaderMain);			Sleep(10);
	thread threadAICore(AICoreMain);					Sleep(10);
	thread threadAudio(audioMain);						Sleep(10);
	thread threadPreRender(preRenderMain);				Sleep(10);
	thread threadAnimation(animationMain);				Sleep(10);
	thread threadMusic(musicMain);						Sleep(10);
	
	// Initializing the server
	if (core.serverMode) {
		server.initialize();
	}
	// Creating the menu
	else {
		script.ui_initialMenu(NULL);
		audio.playMusic(MUSIC_MENU);
	}
	game.access.unlock();

	ai.enable();

	console << "[MAIN] Overlooking the threads..." << "\n";
	int globalTime = timeGetTime();
	while (!core.shutdown)
	{
		// Counting ticks
		if (timeGetTime() - globalTime >= 1000)
		{
			globalTime = timeGetTime();
			core.thread_windowTicksPerSec = core.thread_windowTicks;
			core.thread_serverWorldTicksPerSec = core.thread_serverWorldTicks;
			core.thread_animWorldTicksPerSec = core.thread_animWorldTicks;
			core.thread_windowTicks = 0;
			core.thread_serverWorldTicks = 0;
			core.thread_animWorldTicks = 0;
		}
		// Adding some antifreeze
		for (int i = 0; i < 12; i++) {
			if (!(core.serverMode && (i == 5 || i == 6 || i == 9 || i == 10 || i == 11 || i == 12))) { core.thread_antifreeze[i] += 1; }
			if (core.thread_antifreeze[i] > 1000) {
				core.thread_antifreeze[i] = 0;
				console.error << "[MAIN] Thread " << i << " is not responding..." << endl;
			}
		}
		Sleep(1);
	}

	// Waiting for shutdown
	console << "[MAIN] Waiting for script threads to finish..." << "\n";
	for (int i = 0; i < (int)script.threadVector.size(); i++)
	{
		script.threadVector[i].join();
	}
	console << "[MAIN] Waiting for core threads to finish..." << "\n";
	core.thread_shutdown[12] = true;	threadMusic.join();
	core.thread_shutdown[11] = true;	threadAnimation.join();
	core.thread_shutdown[10] = true;	threadPreRender.join();
	core.thread_shutdown[9] = true;		threadAudio.join();
	core.thread_shutdown[8] = true;		threadAICore.join();
	core.thread_shutdown[7] = true;		threadWorldLoader.join();
	core.thread_shutdown[6] = true;		threadClientSend.join();
	core.thread_shutdown[5] = true;		threadClientReceive.join();
	core.thread_shutdown[4] = true;		threadServerSend.join();
	core.thread_shutdown[3] = true;		threadServerReceive.join();
	core.thread_shutdown[2] = true;		threadServerConnect.join();
	core.thread_shutdown[1] = true;		threadGameLogic.join();
	core.thread_shutdown[0] = true;		threadWindow.join();

	chat << endl << endl;
	console << "[MAIN] Cleaning up" << endl << endl << endl;
	// Cleaning up
	return 0;
}