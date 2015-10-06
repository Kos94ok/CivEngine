
#include "main.h"
#include "overworld.h"
#include "math.h"
#include "saveload.h"
#include "world.h"

void cOverworld::generateMap()
{
	cOverworldMap entry;
	// Test maps
	entry.name = "Spawn";
	entry.id = mapCounter++;
	entry.pos = vec2f(0.00f, 0.00f);
	entry.level = 1;
	entry.dirpath = "map000";
	map.push_back(entry);

	entry.name = "Forest";
	entry.id = mapCounter++;
	entry.pos = vec2f(50.00f, -250.00f);
	entry.level = 1;
	entry.dirpath = "map001";
	map.push_back(entry);

	entry.name = "Forest";
	entry.id = mapCounter++;
	entry.pos = vec2f(100.00f, -200.00f);
	entry.level = 2;
	entry.dirpath = "map002";
	map.push_back(entry);

	// Test links
	linkMaps(0, 1);
	linkMaps(0, 2);

	activeMap = 0;
}

void cOverworld::moveTo(int target)
{
	world.saveTo(save.savefileName, map[activeMap].dirpath);
	ifstream file;
	file.open("Savefiles//" + save.savefileName + "//" + map[target].dirpath + "//_map.dat");
	if (file.good())
	{
		console.debug << "Savefile found" << endl;
		file.close();
		world.loadFrom(save.savefileName, map[target].dirpath);
	}
	else
	{
		console.debug << "Savefile not found" << endl;
		world.clearWorld();
		world.genNormalWorld();
	}
	activeMap = target;
}