
#pragma once
#include "precomp.h"

class cItemEntry
{
public:
	string type;
	int count;
};

class cUnitEntry
{
public:
	int globalId;
	string type;
	vec2f pos;
	vector<cItemEntry> itemList;

	cUnitEntry() {
		globalId = -1;
	}
};

class cBlueprintHeader
{
	/*
	cBlueprintHeader is used to fetch the blueprint header data from the file.
	*/
public:
	int type;
	bool isValid;

	cBlueprintHeader() {
		type = -1;
		isValid = true;
	}

	cBlueprintHeader(int i) {
		type = i;
		isValid = true;
	}
};

class cSave
{
public:
	string savefileName;
	string worldName;

	string getChunkFilePath(vec2i pos);
	cBlueprintHeader getHeaderFromFile(string filename);
	vector<cUnitEntry> getListFromFile(string filename);
	void flushListToFile(cBlueprintHeader header, vector<cUnitEntry> list, string filename);

	void saveScreenshot(sf::Image img);

	cSave() {
		worldName = "_world";
		savefileName = "_temp";
	}
};

extern cSave save;