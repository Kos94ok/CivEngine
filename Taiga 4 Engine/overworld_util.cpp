
#include "main.h"
#include "overworld.h"
#include "UI.h"

void cOverworld::linkMaps(int a, int b)
{
	cOverworldLink link;
	link.a = a;
	link.b = b;
	links.push_back(link);
}

bool cOverworld::isMapAvailable(int index)
{
	for (cOverworldLink link : links)
	{
		if (link.a == index && link.b == activeMap) { return true; }
		if (link.b == index && link.a == activeMap) { return true; }
	}
	return false;
}

int cOverworld::getActiveMapIndex()
{
	for (int i = 0; i < (int)map.size(); i++)
	{
		if (map[i].id == activeMap) { return i; }
	}
	return 0;
}

int cOverworld::getSelectedMapIndex()
{
	for (int i = 0; i < (int)map.size(); i++)
	{
		if (map[i].id == selectedMap) { return i; }
	}
	return 0;
}

void cOverworld::selectMap(int id)
{
	selectedMap = id;
	if (ui.wndOverworld.isDisplayed) {
		ui.wndOverworld.update();
	}
}