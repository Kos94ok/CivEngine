
#include "main.h"
#include "world.h"
#include "game.h"
#include "camera.h"
#include "client.h"
#include "server.h"
#include "math.h"

void cWorld::applyBlueprint(vec2i position, int type)
{
	string name;
	vector<int> correctBlueprints;
	ifstream in;
	ofstream out;

	// Getting all correct blueprints
	for (int i = 0; i < (int)blueprint.size(); i++)
	{
		if (blueprint[i].type == type) { correctBlueprints.push_back(i); }
	}
	if (correctBlueprints.size() > 0)
	{
		// Getting random correct blueprint
		int bpIndex = correctBlueprints[math.rand(0, (int)correctBlueprints.size() - 1)];
		map[position.x][position.y].blueprint = bpIndex;

		// Opening source file
		name = "Data//Blueprints//" + blueprint[bpIndex].name;
		in.open(name);
		if (!in.good()) {
			console.error << "[ERROR] Can't open source chunk file [" + name + "]";
			return;
		}
		// Opening target file
		name = "Savefiles//" + save.savefileName + "//" + save.worldName + "//" + to_string(position.x) + "-" + to_string(position.y) + ".chunk";
		out.open(name);
		if (!out.good()) {
			console.error << "[ERROR] Can't open target chunk file [" + name + "]" << endl;
			return;
		}
		// Copying the content
		out << in.rdbuf();
	}
	else { console.error << "[ERROR] No valid blueprints for type " << type << " at (" << position.x << "; " << position.y << ")!" << "\n"; }
	map[position.x][position.y].isInitialized = true;
}

bool cWorld::isChunkLoaded(vec2i pos)
{
	return map[pos.x][pos.y].isLoaded;
}

bool cWorld::isChunkViable(vec2i pos)
{
	int unitId;
	float bufferArea = 500.00f;
	sf::FloatRect camRect(vec2f(camera.pos.x - bufferArea, camera.pos.y - bufferArea), vec2f(camera.res.x + bufferArea * 2, camera.res.y + bufferArea * 2));
	sf::FloatRect unitRect;
	sf::FloatRect chunkRect(vec2f(pos.x * LIMIT_CHUNKSIZE, pos.y * LIMIT_CHUNKSIZE), vec2f(LIMIT_CHUNKSIZE, LIMIT_CHUNKSIZE));
	if (client.unit != -1){
		unitId = game.getUnitId(client.unit);
		if (unitId != -1) {
			unitRect = sf::FloatRect(vec2f(game.unit[unitId].pos.x - camera.res.x / 2, game.unit[unitId].pos.y - camera.res.y / 2), vec2f(camera.res.x, camera.res.y));
		}
	}
	// Local player
	if (client.connected && (camRect.intersects(chunkRect) || unitRect.intersects(chunkRect)))
	{
		return true;
	}
	// Remote players
	if (core.localServer || core.serverMode)
	{
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (server.player[i].connected)
			{
				camRect = sf::FloatRect(vec2f(server.player[i].camPos.x - bufferArea, server.player[i].camPos.y - bufferArea),
					vec2f(server.player[i].camRes.x + bufferArea * 2, server.player[i].camRes.y + bufferArea * 2));
				if (server.player[i].unit != -1){
					unitId = game.getUnitId(server.player[i].unit);
					if (unitId != -1)
					{
						unitRect = sf::FloatRect(vec2f(game.unit[unitId].pos.x - server.player[i].camRes.x / 2,
							game.unit[unitId].pos.y - server.player[i].camRes.y / 2), vec2f(server.player[i].camRes.x, server.player[i].camRes.y));
					}
				}
				if (camRect.intersects(chunkRect) || unitRect.intersects(chunkRect))
				{
					return true;
				}
			}
		}
	}
	return false;
}

vec2i cWorld::getChunkInPos(vec2 pos)
{
	return vec2i(floor(pos.x / LIMIT_CHUNKSIZE), floor(pos.y / LIMIT_CHUNKSIZE));
}

vec2 cWorld::getChunkCenter(vec2i pos)
{
	return vec2((float)pos.x * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2, (float)pos.y * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2);
}

void cWorld::clearWorld()
{
	mutex.worldMain.lock();
	util.clearFolder("Savefiles//" + save.savefileName + "//" + save.worldName + "//");
	unloadAll();
	applyUnload();
	mutex.worldMain.unlock();
}

void cWorld::applyUnload()
{
	int active = -1;
	bool detected;

	while ((int)unloadVector.size() > 0)
	{
		// Getting the first value
		active = unloadVector[0];
		unloadVector.erase(unloadVector.begin());
		// Searching for the replacement
		for (int i = game.unitCounter - 1; i > 0; i--)
		{
			detected = false;
			// Making sure that the unit should not be removed
			for (int y = 0; y < (int)unloadVector.size(); y++)
			{
				if (unloadVector[y] == i) { detected = true; y = (int)unloadVector.size(); }
			}
			// Not detected, applying
			if (!detected)
			{
				game.unit[active] = game.unit[i];
				game.unitCounter -= 1;
				i = 0;
			}
		}
	}
}