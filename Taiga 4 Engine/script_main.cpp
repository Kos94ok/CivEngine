
#include "main.h"
#include "script.h"

void cScript::execute(std::function<void(cArg)> scriptFunc, cArg args)
{
	access.lock();

	functionQueue.push_back(scriptFunc);
	functionArgs.push_back(args);
	/*int freeThread = getFreeThread();
	if (freeThread == -1)
	{
		threadReady.push_back(false);
		threadVector.push_back(std::thread(cScript::threadMain, threadReady.size() - 1));
	}*/
	access.unlock();
}

int cScript::getFreeThread()
{
	for (int i = 0; i < (int)threadReady.size(); i++)
	{
		if (threadReady[i]) { return i; }
	}
	return -1;
}

void cScript::threadMain(int index)
{
	function<void(cArg)> reservedFunction;
	cArg args;

	while (!core.shutdown)
	{
		script.access.lock();
		if (script.functionQueue.size() > 0)
		{
			script.threadReady[index] = false;
			// Copying the data
			reservedFunction = script.functionQueue[0];
			args = script.functionArgs[0];
			// Removing the data from global array
			script.functionQueue.erase(script.functionQueue.begin());
			script.functionArgs.erase(script.functionArgs.begin());
			script.access.unlock();
			// Starting the function
			reservedFunction(args);
			// Function completed
			script.threadReady[index] = true;
		}
		else { script.access.unlock(); }
		Sleep(10);
	}
}

void cScript::wait(int time)
{
	int timeSlept = 0;
	while (timeSlept < time && !core.shutdown)
	{
		Sleep(10);
		timeSlept += 10;
	}
}