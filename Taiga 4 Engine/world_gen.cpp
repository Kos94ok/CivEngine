
#include "main.h"
#include "world.h"
#include "game.h"
#include "database.h"
#include "settings.h"
#include "math.h"
#include "camera.h"
#include "visual.h"

void cWorld::genTaigaMini()
{
	string type;
	int random;
	// Creating some trees
	for (int i = 0; i < math.rand(500, 600); i++)
	{
		random = math.rand(0, 3);
		if (random == 0) { type = "tree_basic_a"; }
		else if (random == 1) { type = "tree_basic_b"; }
		else if (random == 2) { type = "tree_basic_c"; }
		else { type = "tree_basic_d"; }
		game.addUnit(type, vec2(math.rand(-camera.res.x * 2, camera.res.x * 2), math.rand(-camera.res.y * 2, camera.res.y * 2)));
	}
	// Creating some units
	for (int i = 0; i < 10; i++)
	{
		int id = game.addUnit("campfire_full",
			sf::Vector2f(math.rand(-camera.res.x * 2, camera.res.x * 2), math.rand(-camera.res.y * 2, camera.res.y * 2)));
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].light.flickerCurTime = math.randf(0.00f, game.unit[game.getUnitId(id)].light.flickerTime);
		}
	}

	// Moving the camera
	camera.moveto(sf::Vector2f(0.00f, 0.00f));
}

void cWorld::genArena()
{
	database.texture[TEX_WORLD_GROUND] = visual.addTexture("bg_neon.jpg");

	float arenaSizeX = 2000.00f;
	float arenaSizeY = 1200.00f;
	// Left
	int id = game.addUnit("arena_wall", vec2(0.00f, 0.00f));
	game.getUnit(id).size.y = arenaSizeY;
	// Top
	id = game.addUnit("arena_wall", vec2(20.00f, 0.00f));
	game.getUnit(id).size.x = arenaSizeX - 20.00f;
	// Right
	id = game.addUnit("arena_wall", vec2(arenaSizeX, 0.00f));
	game.getUnit(id).size.y = arenaSizeY;
	// Bottom
	id = game.addUnit("arena_wall", vec2(0.00f, arenaSizeY));
	game.getUnit(id).size.x = arenaSizeX + 20.00f;
}

void cWorld::genNormalWorld()
{
	console << "[cWorld::genNormalWorld] Starting the generation" << endl;
	// Clearing the saved world data;
	clearWorld();
	// Settings initial state
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			map[i][j].type = CHUNK_UNDEFINED;
			map[i][j].isLoaded = false;
			map[i][j].isInitialized = false;
		}
	}

	// Generating spawn point
	vec2i spawnPointI = vec2i(LIMIT_MAP / 2, LIMIT_MAP / 2);

	// Generating paths
	endPoints.clear();
	for (int i = 0; i < settings.wgStartingPath; i++)
	{
		genChunkPath(1, spawnPointI);
	}
	// Searching for escape points
	float dist;
	float d1 = 0, d2 = 0, d3 = 0;
	vec2i p1, p2, p3;
	for (int i = 0; i < (int)endPoints.size(); i++)
	{
		dist = math.getDistance(endPoints[i], spawnPointI);
		if (dist > d1) {
			d3 = d2;		p3 = p2;
			d2 = d1;		p2 = p1;
			d1 = dist;		p1 = endPoints[i];
		}
		else if (dist > d2) {
			d3 = d2;		p3 = p2;
			d2 = dist;		p2 = endPoints[i];
		}
		else if (dist > d3) {
			d3 = dist;		p3 = endPoints[i];
		}
	}
	// Determining the escape points
	float randVal = math.randf(0.00f, 1.00f);
	map[p1.x][p1.y].type = CHUNK_ESCAPE;
	if (randVal <= 0.70f && d2 > 10) {
		map[p2.x][p2.y].type = CHUNK_ESCAPE;
	}
	if (randVal <= 0.30f && d3 > 10) {
		map[p3.x][p3.y].type = CHUNK_ESCAPE;
	}

	// Writing global spawn point
	spawnPoint = vec2(spawnPointI.x * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2, spawnPointI.y * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2);

	// Applying spawn point
	map[spawnPointI.x][spawnPointI.y].type = CHUNK_SPAWN;

	// Creating a wall around the normal chunks
	for (int y = 0; y < 2; y++)
	{
		for (int j = 1; j < LIMIT_MAP - 1; j++)
		{
			for (int i = 1; i < LIMIT_MAP - 1; i++)
			{
				if (map[i][j].type == CHUNK_UNDEFINED &&
					((map[i + 1][j].type != CHUNK_UNDEFINED && (map[i + 1][j].type != CHUNK_TEMP || y == 1) && map[i + 1][j].type != CHUNK_TEMP + 1)
					|| (map[i + 1][j + 1].type != CHUNK_UNDEFINED && (map[i + 1][j + 1].type != CHUNK_TEMP || y == 1) && map[i + 1][j + 1].type != CHUNK_TEMP + 1)
					|| (map[i][j + 1].type != CHUNK_UNDEFINED && (map[i][j + 1].type != CHUNK_TEMP || y == 1) && map[i][j + 1].type != CHUNK_TEMP + 1)
					|| (map[i - 1][j + 1].type != CHUNK_UNDEFINED && (map[i - 1][j + 1].type != CHUNK_TEMP || y == 1) && map[i - 1][j + 1].type != CHUNK_TEMP + 1)
					|| (map[i - 1][j].type != CHUNK_UNDEFINED && (map[i - 1][j].type != CHUNK_TEMP || y == 1) && map[i - 1][j].type != CHUNK_TEMP + 1)
					|| (map[i - 1][j - 1].type != CHUNK_UNDEFINED && (map[i - 1][j - 1].type != CHUNK_TEMP || y == 1) && map[i - 1][j - 1].type != CHUNK_TEMP + 1)
					|| (map[i][j - 1].type != CHUNK_UNDEFINED && (map[i][j - 1].type != CHUNK_TEMP || y == 1) && map[i][j - 1].type != CHUNK_TEMP + 1)
					|| (map[i + 1][j - 1].type != CHUNK_UNDEFINED && (map[i + 1][j - 1].type != CHUNK_TEMP || y == 1) && map[i + 1][j - 1].type != CHUNK_TEMP + 1)))
				{
					map[i][j].type = CHUNK_TEMP;
					if (y == 1) { map[i][j].type = CHUNK_TEMP + 1; }
				}
			}
		}
	}
	// Swap temp to normal
	for (int j = 1; j < LIMIT_MAP - 1; j++) {
		for (int i = 1; i < LIMIT_MAP - 1; i++) {
			if (map[i][j].type == CHUNK_TEMP || map[i][j].type == CHUNK_TEMP + 1) {
				map[i][j].type = CHUNK_BLOCKED;
			}
		}
	}

	// Blocking the edges
	/*for (int i = 0; i < LIMIT_MAP; i++)
	{
		map[i][0].type = CHUNK_BLOCKED;
		map[i][63].type = CHUNK_BLOCKED;
	}
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		map[0][j].type = CHUNK_BLOCKED;
		map[63][j].type = CHUNK_BLOCKED;
	}*/

	ofstream file;
	file.open("Logs//map.txt");
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			if (map[i][j].type == CHUNK_UNDEFINED) { file << "? "; }
			else if (map[i][j].type == CHUNK_NORMAL) { file << "~ "; }
			else if (map[i][j].type == CHUNK_BLOCKED) { file << "X "; }
			else if (map[i][j].type == CHUNK_SPAWN) { file << "@ "; }
			else if (map[i][j].type == CHUNK_ESCAPE) { file << "& "; }
		}
		file << "\n";
	}
	file.close();

	// Creating directory
	string name = "Savefiles//" + save.savefileName + "//" + save.worldName;
	string dirName = "Savefiles//" + save.savefileName;
	CreateDirectoryA("Savefiles", 0);
	CreateDirectoryA(dirName.c_str() , 0);
	CreateDirectoryA(name.c_str(), 0);
	// Copying blueprints to real positions
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			if (map[i][j].type != CHUNK_UNDEFINED)
			{
				//applyBlueprint(vec2i(i, j), map[i][j].type);
			}
		}
	}
	console << "[cWorld::genNormalWorld] World generated" << endl;
}

void cWorld::genChunkPath(int val, vec2i pos)
{
	vec2i newPos;
	int random = math.rand(0, 3), saver = 0;

	// Looking for a non-claimed path
	do
	{
		newPos = pos;
		random = math.rand(0, 3);
		if (random == 0) { newPos.x += 1; }
		else if (random == 1) { newPos.y += 1; }
		else if (random == 2) { newPos.x -= 1; }
		else if (random == 3) { newPos.y -= 1; }
		saver += 1;
	}
	while (saver < 10 && map[newPos.x][newPos.y].type != CHUNK_UNDEFINED);

	// If the path has found
	if (saver < 10)
	{
		pos = newPos;
		map[pos.x][pos.y].type = CHUNK_NORMAL;

		// Termination chance
		if (math.rand(settings.wgMinimalPathLength, settings.wgMaximalPathLength) < val) {
			endPoints.push_back(pos);
			return;
		}
			// Not terminated - continue
		genChunkPath(val + 1, pos);
			// Chance to fork
		if (math.rand(0, 100) < settings.wgPathForkChance) { genChunkPath(val + 1, pos); }
	}
	else { return; }
}