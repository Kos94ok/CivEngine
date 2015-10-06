
#include "main.h"
#include "ai.h"
#include "game.h"

void cAI::tick(int elapsedTime)
{
	localTime += elapsedTime;
	if (!enabled) { return; }

	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].ai.enabled && game.unit[i].ai.thinkTimer > 1000)
		{
			game.unit[i].ai.thinkTimer = 0;
			game.unit[i].ai.thinkFunc(game.unit[i].globalId);
		}
		else if (game.unit[i].ai.enabled) { game.unit[i].ai.thinkTimer += elapsedTime; }
	}
}

void AICoreMain()
{
	int threadId = 8;
	console << "[AI_CORE] Starting the AI core thread" << " [ID: " << threadId << "]" << "\n";
	srand(time(0));
	int elapsedTime, globalTime = 0;
	while (!core.thread_shutdown[threadId])
	{
		elapsedTime = timeGetTime() - globalTime;
		globalTime = timeGetTime();
		// Ignore system time change or huge lags
		if (elapsedTime > 0 && elapsedTime < 100)
		{
			// Only performing on the server-side
			if (core.localServer || core.serverMode) {
				ai.tick(elapsedTime);
			}
			core.thread_antifreeze[threadId] = 0;
		}
		Sleep(1);
	}
	console << "[AI_CORE] Cleaning up" << "\n";
}