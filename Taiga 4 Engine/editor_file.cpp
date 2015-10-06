
#include "main.h"
#include "editor.h"
#include "game.h"
#include "saveload.h"

void cEditor::saveBlueprint(string name)
{
	cUnitEntry unitEntry;
	vector<cUnitEntry> unitList;
	// Looking for the path
	string path = "Data//Blueprints//" + name;
	if (path.substr(path.length() - 10) != ".blueprint") { path += ".blueprint"; }
	// Creating unit list
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (!game.unit[i].hasRef(REF_UNIT_NOSAVE))
		{
			unitEntry.globalId = -1;
			unitEntry.pos = game.unit[i].pos;
			unitEntry.type = game.unit[i].type;
			unitList.push_back(unitEntry);
		}
	}
	save.flushListToFile(editor.autogenType, unitList, path);
}

void cEditor::loadBlueprint(string name)
{
	//char buffer[256];
	string buf, type;
	vec2 position;
	// Looking for the path
	string path = "Data//Blueprints//" + name;
	if (path.substr(path.length() - 10) != ".blueprint") { path += ".blueprint"; }
	vector<cUnitEntry> unitList = save.getListFromFile(path);
	
	game.clearUnits();
	for (int i = 0; i < (int)unitList.size(); i++)
	{
		game.addUnit(unitList[i].type, unitList[i].pos, -1, -1, false);
	}
}