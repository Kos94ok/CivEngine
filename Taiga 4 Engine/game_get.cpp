
#include "main.h"
#include "game.h"
#include "database.h"
#include "math.h"
#include "client.h"

// Convert global id to local id
int cGame::getUnitId(int globalId)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].globalId == globalId) { return i; }
	}
	return -1;
}

// Convert world location to local id
int cGame::getUnitId(sf::Vector2f pos, int refFilter)
{
	cUnit unit = getUnit(pos, refFilter);
	if (unit.type == "missingno") { return -1; }
	return unit.globalId;
}

// Get player
cUnit& cGame::getPlayer()
{
	return getUnit(client.unit);
}

// Get unit
cUnit& cGame::getUnit(int id)
{
	id = getUnitId(id);
	if (id != -1) {
		return unit[id];
	}
	//console << "[cGame::getUnit] Can't find the unit " << id << "!" << "\n";
	return database.unit[0];
}

// Convert world location to unit
cUnit& cGame::getUnit(vec2f pos, int refFilter)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (math.getDistance(unit[i].pos, pos) < unit[i].interactDistance && unit[i].hasRef(refFilter)) {
			return unit[i];
		}
	}
	return database.unit[0];
}

// Convert world location to unit
cUnit& cGame::getUnitPhys(vec2f &pos, int refFilter)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (math.getDistance(unit[i].pos, pos) < unit[i].collisionDistance && unit[i].hasRef(refFilter)) {
			return unit[i];
		}
	}
	return database.unit[0];
}

// Find unit by ref
cUnit& cGame::getUnitByRef(int refFilter)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].hasRef(refFilter)) {
			return unit[i];
		}
	}
	return database.unit[0];
}

// Get local unit
cUnit& cGame::getLocalUnit()
{
	cUnit* retVal = &database.unit[0];
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].globalId == ID_LOCAL) {
			retVal = &unit[i];
		}
	}
	return *retVal;
}

// Get the distance required for interact
float cGame::getUnitInteractDistance(cUnit unitA, cUnit unitB) {
	return (unitA.interactDistance + unitB.interactDistance);
}

// Get the distance required for collision
float cGame::getUnitCollisionDistance(cUnit *unitA, cUnit *unitB) {
	return (unitA->collisionDistance + unitB->collisionDistance);
}

// Get closest interact point
vec2f cGame::getUnitInteractPoint(cUnit unitA, cUnit unitB) {
	return math.polar(unitB.pos, unitA.interactDistance + unitB.interactDistance, math.getAngle(&unitB, &unitA));
}

// Get amount of units by type
int cGame::getUnitCount(std::string type)
{
	int count = 0;
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].type == type) { count += 1; }
	}
	return count;
}

// Get amount of units by ref
int cGame::getUnitCount(int refId)
{
	int count = 0;
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].hasRef(refId)) { count += 1; }
	}
	return count;
}