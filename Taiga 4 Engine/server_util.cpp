
#include "main.h"
#include "server.h"
#include "client.h"
#include "world.h"
#include "editor.h"
#include "UI.h"

void cServer::sendPacket(int target, sf::Packet data, int overrideId)
{
	if (!core.localServer && !core.serverMode) { return; }

	mutex.serverPacketQueue.lock();

	if (overrideId != -1) { dataQueue[dataQueueCounter].id = overrideId; }
	else {
		dataQueue[dataQueueCounter].id = packetIdCounter;
		packetIdCounter += 1;
		if (packetIdCounter > 50000) { packetIdCounter = 0; }
	}
	dataQueue[dataQueueCounter].target = target;
	dataQueue[dataQueueCounter].data = data;
	dataQueueCounter += 1;

	mutex.serverPacketQueue.unlock();
}

void cServer::removePacket(int localIndex)
{
	mutex.serverPacketQueue.lock();

	for (int i = localIndex; i < server.dataQueueCounter - 1; i++) {
		server.dataQueue[i] = server.dataQueue[i + 1];
	}
	server.dataQueueCounter -= 1;

	mutex.serverPacketQueue.unlock();
}

void cServer::initialize()
{
	console.displayed = true;
	world.genNormalWorld();
}

void cServer::introduce(int playerId)
{
	// Initializing
	player[playerId].unit = -1;
	// Sending unit data
	sf::Packet data;
	// Ignore local player
	if (!server.isLocalPlayer(playerId))
	{
		for (int i = 0; i < game.unitCounter; i++)
		{
			//data << MSG_UNIT_ADD << game.unit[i].globalId << game.unit[i].type << game.unit[i].pos.x << game.unit[i].pos.y << -1 << -1;
			data = server.packUnitData(game.unit[i].globalId);
			sendPacket(playerId, data);
			data.clear();
			if (server.dataQueueCounter > 100) { Sleep(1); }
			for (int y = 0; y < game.unit[i].orderCounter; y++)
			{
				if (game.unit[i].order[y].type == ORDER_MOVETO)
				{
					data << MSG_ORDER_MOVETO << game.unit[i].globalId << game.unit[i].order[y].targetPos.x << game.unit[i].order[y].targetPos.y << true;
					sendPacket(playerId, data);
				}
				data.clear();
				if (server.dataQueueCounter > 100) { Sleep(1); }
			}
		}
	}
	// Send gamedata
	if (!server.isLocalPlayer(playerId))
	{
		data << MSG_GAME_TIME << game.timeOfDay;
		server.sendPacket(playerId, data);
		data.clear();
		data << MSG_GAME_LIGHT << game.ambientLight;
		server.sendPacket(playerId, data);
		data.clear();
	}
	// Create new unit
	string playerType = "player";
	if (core.editorMode) { playerType = "editor"; }

	server.assignUnit(playerId, game.addUnit(playerType, world.spawnPoint));
	server.player[playerId].setHealth(100.00f);
	server.player[playerId].setMaxHealth(100.00f);
	server.player[playerId].moveCamera(world.spawnPoint);
	if (!core.editorMode)
	{
		//game.unit[game.getUnitId(server.player[playerId].unit)].addItem("human_ear", 100);
		//game.unit[game.getUnitId(server.player[playerId].unit)].addItem("human_arm", 50);
		//game.unit[game.getUnitId(server.player[playerId].unit)].addItem("human_leg", 70);
		//game.unit[game.getUnitId(server.player[playerId].unit)].addItem("test_resourceManipulator", 1);
		game.unit[game.getUnitId(server.player[playerId].unit)].addItem("weapon_rifle", 1);
		game.unit[game.getUnitId(server.player[playerId].unit)].addItem("survival_kit_basic", 1);
		server.sendWorldData(playerId);
	}
	else { editor.initialize(); }
	ui.updateInterface();
}

void cServer::sendChunkData(int playerId, int x, int y)
{
	sf::Packet data;
	if (!world.map[x][y].isLoaded)
	{
		world.loadChunk(vec2i(x, y));
	}

	game.access.lock();
	world.access.lock();

	data << MSG_BIG_WORLD_ONECHUNK;
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == vec2i(x, y))
		{
			addUnitData(&data, &game.unit[i]);
		}
	}
	server.sendPacket(playerId, data);

	world.access.unlock();
	game.access.unlock();
}

void cServer::sendWorldData(int playerId)
{
	sf::Packet data;
	data << MSG_BIG_WORLD_ALLCHUNKS << LIMIT_MAP;

	for (int y = 0; y < LIMIT_MAP; y++)
	{
		for (int x = 0; x < LIMIT_MAP; x++)
		{
			data << world.map[x][y].type;
		}
	}
	sendPacket(playerId, data);
	data.clear();
}

void cServer::assignUnit(int playerId, int unitId)
{
	sf::Packet data;
	player[playerId].unit = unitId;
	if (!isLocalPlayer(playerId))
	{
		data << MSG_UNIT_HERO << player[playerId].unit;
		sendPacket(playerId, data);
		data.clear();
	}
	else {
		client.unit = player[playerId].unit;
	}
}

bool cServer::isLocalPlayer(int player)
{
	return (!core.serverMode && player == 0 && client.connected);
}

int cServer::getController(int target)
{
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (player[i].connected && player[i].unit == target) { return i; }
	}
	return -1;
}

void cServer::sendEcho(int source, string str)
{
	sf::Packet data;
	data << MSG_ECHO << source << str;
	sendPacket(PLAYERS_REMOTE, data);
	data.clear();
}

void cServer::pingPlayers()
{
	sf::Packet data;
	data << MSG_PING;
	sendPacket(PLAYERS_ALL, data);
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (player[i].connected)
		{
			player[i].lastPingTime = timeGetTime();
		}
	}
	data.clear();
}

int cServer::getPlayerCount()
{
	int retVal = 0;
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (player[i].connected) {
			retVal += 1;
		}
	}
	return retVal;
}