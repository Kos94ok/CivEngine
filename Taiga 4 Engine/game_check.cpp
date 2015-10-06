
#include "main.h"
#include "game.h"

// Time checks
bool cGame::isDay() { return (timeOfDay > 6.00f && timeOfDay < 18.00f); }
bool cGame::isNight() { return !isDay(); }
bool cGame::lightens() { return (timeOfDay > 4.00f && timeOfDay < 16.00f); }
bool cGame::darkens() { return !lightens(); }

// Is unit alive checks
bool cGame::isUnitAlive(int globalId)
{
	cUnit* foundUnit = &getUnit(globalId);
	if (foundUnit->type == "missingno") { return false; }
	if (foundUnit->health <= 0.00f && foundUnit->maxHealth > 0.00f) { return false; }
	if (foundUnit->order[0].type == ORDER_DEATH) { return false; }
	return true;
}

bool cGame::isUnitDead(int globalId) { return !isUnitAlive(globalId); }