
#include "main.h"
#include "logic.h"
#include "client.h"
#include "server.h"

void cGameLogic::updateStatsFor(int unit, int player, float timevar)
{
	cUnit* target = &game.getUnit(unit);
	if (target->type == "missingno") { return; }

	// Initial values
	float coldDelta = value.statCold_normal_day * timevar;
	if (game.isNight()) { coldDelta = value.statCold_normal_night * timevar; }
	float hungerDelta = value.statHunger_normal * timevar;

	// -2.0 cold per second on a single full campfire
	coldDelta -= (float)target->buff.getPower(BUFF_HEAT) * 0.02f * timevar;
	// -1.0 hunger per second for 10 food stacks
	hungerDelta -= (float)target->buff.getPower(BUFF_FOOD) * 0.10f * timevar;

	// Flush to client
	if (player == -1) {
		client.addCold(coldDelta);
		client.addHunger(hungerDelta);
	}
	// Flush to server
	else {
		server.player[player].addCold(coldDelta, true);
		server.player[player].addHunger(hungerDelta, true);
	}
}

void cGameLogic::updateSurvivalStats(int elapsedTime)
{
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	// Local update
	if (client.connected) {
		updateStatsFor(client.unit, -1, timevar);
	}
	// Server update
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (server.player[i].connected && server.player[i].unit != -1)
		{
			updateStatsFor(server.player[i].unit, i, timevar);
		}
	}
}