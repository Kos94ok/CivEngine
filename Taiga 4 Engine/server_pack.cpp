
#include "main.h"
#include "server.h"

void cServer::addUnitData(sf::Packet* data, cUnit* unit, int variation)
{
	data->operator<<(unit->globalId) << unit->type << unit->pos.x << unit->pos.y << unit->owner << variation;
}

sf::Packet cServer::packUnitData(int id, int variation)
{
	sf::Packet data;
	cUnit* unit = &game.getUnit(id);
	if (unit->type == "missingno") { console.error << "[cServer::packUnitData] Can't find the unit!" << endl; return data; }
	data << MSG_UNIT_ADD;
	addUnitData(&data, unit);
	
	return data;
}