
#include "prerender.h"
#include "main.h"
#include "console.h"
#include "settings.h"
#include "camera.h"
#include "game.h"

void cPreRender::updateUnits()
{
	game.access.lock();
	mutex.renderUnits.lock();
	preRender.units.ready = true;
	preRender.units.queue.clear();

	sf::RenderStates renderState;

	float cameraLeft = camera.pos.x;
	float cameraRight = camera.res.x + camera.pos.x;
	float cameraTop = camera.pos.y;
	float cameraBot = camera.res.y + camera.pos.y;
	// Render distance
	if (core.advancedDebug)
	{
		cameraLeft -= 5000;	cameraRight += 5000;
		cameraTop -= 5000;	cameraBot += 5000;
	}
	else
	{
		cameraLeft -= 200;	cameraRight += 200;
		cameraTop -= 300;	cameraBot += 550;
	}

	// Random initialization
	float unitLeft, unitRight, unitTop, unitBot;
	sf::FloatRect camRect(camera.pos.x, camera.pos.y, camera.res.x, camera.res.y);
	sf::FloatRect objRect;
	vector<int> miniQueue;
	vector<int> sortedQueue;

	for (int i = 0; i < game.unitCounter; i++)
	{
		unitTop = game.unit[i].pos.y - game.unit[i].center.y;
		unitBot = unitTop + game.unit[i].size.y;
		unitLeft = game.unit[i].pos.x - game.unit[i].center.x;
		unitRight = unitLeft + game.unit[i].size.x;

		if (!game.unit[i].hasRef(REF_UNIT_NORENDER)
			&& (game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE) || (unitRight >= cameraLeft && unitLeft <= cameraRight && unitTop >= cameraTop && unitBot <= cameraBot)
			/*&& !game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE_BOT) && !game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE_TOP))
			|| (game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE_BOT) && y == cameraTop)
			|| (game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE_TOP) && y == cameraBot - step*/))
		{
			// Check if unit is already in the queue
			bool found = false;
			for (int checkVal : miniQueue)
			{
				if (checkVal == i)
				{
					found = true;
					break;
				}
			}
			// Don't allow repeating
			if (!found) { miniQueue.push_back(i); }
		}
	}
	// Sorting
	bool renderFirst = false;
	bool renderLast = false;
	int bestUnit = -1;
	float bestPos = -1.00f;
	while ((int)miniQueue.size() > 0)
	{
		// Search
		for (int i = 0; i < (int)miniQueue.size(); i++)
		{
			if (bestUnit == -1 || game.unit[miniQueue[i]].pos.y < bestPos || game.unit[miniQueue[i]].hasRef(REF_UNIT_RENDERFIRST))
			{
				bestUnit = i;
				bestPos = game.unit[miniQueue[i]].pos.y;
				if (game.unit[miniQueue[i]].hasRef(REF_UNIT_RENDERFIRST)) { break; }
			}
		}
		// Add
		preRender.units.queue.push_back(miniQueue[bestUnit]);
		// Erase
		miniQueue[bestUnit] = miniQueue[miniQueue.size() - 1];
		miniQueue.pop_back();
		// Clear
		bestUnit = -1;
		bestPos = -1.00f;
	}
	mutex.renderUnits.unlock();
	game.access.unlock();
}

void preRenderMain()
{
	if (core.serverMode) { return; }

	int threadId = 10;
	console << "[RENDER] Pre-render thread started" << " [ID: " << threadId << "]" << "\n";
	while (!core.thread_shutdown[threadId])
	{
		if (!preRender.units.ready) { preRender.updateUnits(); }
		core.thread_antifreeze[threadId] = 0;
		Sleep(5);
	}
	console << "[RENDER] Cleaning up" << "\n";
}