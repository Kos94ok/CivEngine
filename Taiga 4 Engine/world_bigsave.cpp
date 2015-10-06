
#include "main.h"
#include "world.h"
#include "saveload.h"

void cWorld::saveMapData()
{
	ofstream file;
	file.open("Savefiles//" + save.savefileName + "//" + save.worldName + "//" + "_map.dat");
	if (!file.good()) { console.error << "[cWorld::saveMapData] Unable to create the file!" << endl; return; }
	
	file << LIMIT_MAP << LIMIT_MAP;
	for (int y = 0; y < LIMIT_MAP; y++)
	{
		for (int x = 0; x < LIMIT_MAP; x++)
		{
			file << map[x][y].type << map[x][y].isInitialized << map[x][y].blueprint;
		}
	}
	file.close();
}

void cWorld::loadMapData()
{
	ifstream file;
	file.open("Savefiles//" + save.savefileName + "//" + save.worldName + "//" + "_map.dat");
	if (!file.good()) { console.error << "[cWorld::loadMapData] Unable to open the file!" << endl; return; }

	int limX, limY;
	file >> limX >> limY;
	for (int y = 0; y < limY; y++)
	{
		for (int x = 0; x < limX; x++)
		{
			file >> map[x][y].type >> map[x][y].isInitialized >> map[x][y].blueprint;
			map[x][y].isLoaded = false;
		}
	}
	file.close();
}

void cWorld::saveTo(string overworldName, string worldName)
{
	mutex.worldMain.lock();
	saveAll();
	saveMapData();
	string memOverworldName = save.savefileName;
	string memWorldName = save.worldName;

	save.savefileName = overworldName;
	save.worldName = worldName;

	string output = "Savefiles//" + overworldName;		CreateDirectoryA(output.c_str(), 0);
	output += "//" + save.worldName + "//";				CreateDirectoryA(output.c_str(), 0);
	string input = "Savefiles//" + memOverworldName + "//" + memWorldName + "//";
	util.copyFolder(input, output);

	save.savefileName = memOverworldName;
	save.worldName = memWorldName;
	mutex.worldMain.unlock();
}

void cWorld::loadFrom(string overworldName, string worldName)
{
	mutex.worldMain.lock();

	clearWorld();

	string memOverworldName = save.savefileName;
	string memWorldName = save.worldName;
	save.savefileName = overworldName;
	save.worldName = worldName;

	loadMapData();
	
	string input = "Savefiles//" + overworldName + "//" + save.worldName + "//";
	string output = "Savefiles//" + memOverworldName + "//" + memWorldName + "//";
	util.copyFolder(input, output);

	save.savefileName = memOverworldName;
	save.worldName = memWorldName;
	mutex.worldMain.unlock();
}