
#pragma once
#include "precomp.h"

class cOverworldMap
{
public:
	int id;

	vec2f pos;
	string name;
	string descr;
	string dirpath;
	int level;
};

class cOverworldLink
{
public:
	int a;
	int b;
};

class cOverworld
{
public:
	int activeMap;
	int selectedMap;
	int mapCounter;
	vector<cOverworldMap> map;
	vector<cOverworldLink> links;

	void generateMap();
	void linkMaps(int a, int b);

	bool isMapAvailable(int index);
	int getActiveMapIndex();
	int getSelectedMapIndex();

	void selectMap(int id);
	void moveTo(int target);
	//void save(string saveName);
	//void load(string saveName);
};

extern cOverworld overworld;