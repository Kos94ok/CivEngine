
#pragma once
#include "util.h"

class cScript
{
public:
	cMutex access;

	std::vector<bool> threadReady;
	std::vector<std::thread> threadVector;
	std::vector<std::function<void(cArg)>> functionQueue;
	std::vector<cArg> functionArgs;

	void execute(std::function<void(cArg)> scriptFunc, cArg args = 0);
	int getFreeThread();

	static void threadMain(int index);
	void wait(int time);

	cScript() 
	{
		for (int i = 0; i < LIMIT_SCRIPTCOUNT; i++)
		{
			threadReady.push_back(false);
			threadVector.push_back(std::thread(cScript::threadMain, i));
		}
	}

	// Script data
	static void shooter_spawnEnemies(cArg args);
	static void shooter_respawnSystem(cArg args);
	static void shooter_spawnItems(cArg args);
	static void test_unitAddSystem(cArg args);
	static void test_consoleSystem(cArg args);

	static void ui_initialMenu(cArg args);
	static void server_sendChunkData(cArg args);
	static void unit_torch(cArg args);
	static void unit_flashlight(cArg args);
	static void unit_bindToMouse(cArg args);
};

extern cScript script;