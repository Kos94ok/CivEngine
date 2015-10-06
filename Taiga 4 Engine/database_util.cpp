
#include "main.h"
#include "database.h"
#include "math.h"

cSound& cDatabase::getSound(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_SOUNDS; i++)
	{
		if (sound[i].name == type) { access.unlock(); return sound[i]; }
	}
	// Return error-unit
	console.error << "[ERROR] Can't find the sound [" << type << "] in the database!" << "\n";
	access.unlock();
	return sound[0];
}

cUnit& cDatabase::getUnit(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		if (unit[i].type == type) { access.unlock(); return unit[i]; }
	}
	// Return error-unit
	console.error << "[ERROR] Can't find the unit [" << type << "] in the database!" << "\n";
	access.unlock();
	return unit[0];
}

cItem& cDatabase::getItem(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].type == type) { access.unlock(); return item[i]; }
	}
	// Return error-unit
	console.error << "[ERROR] Can't find the item [" << type << "] in the database!" << "\n";
	access.unlock();
	return item[0];
}

cItem& cDatabase::findItem(int ref)
{
	access.lock();
	int retVal = 0;
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].hasRef(ref) && retVal == 0) { retVal = i; }
		else if (item[i].hasRef(ref) && retVal != 0) { console.error << "[WARNING] Item with ref [" << ref << "] not unique in the database [" << item[i].type << "]!" << endl; }
	}
	// Return
	if (retVal == 0) { console.error << "[ERROR] Can't find any items with ref [" << ref << "] in the database!" << "\n"; }
	access.unlock();
	return item[retVal];
}

cUIElement& cDatabase::getUIElement(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_UIELEMENTS; i++)
	{
		if (uiElement[i].type == type) { access.unlock(); return uiElement[i]; }
	}
	// Return error-unit
	console.error << "[ERROR] Can't find the UI element [" << type << "] in the database!" << "\n";
	access.unlock();
	return uiElement[0];
}

cParticleUnit& cDatabase::getParticle(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_PARTICLES; i++)
	{
		if (particle[i].type == type) { access.unlock(); return particle[i]; }
	}
	// Return error-unit
	console.error << "[ERROR] Can't find the particle [" << type << "] in the database!" << "\n";
	access.unlock();
	return particle[0];
}

cMusic& cDatabase::getRandomMusic(int group)
{
	vector<int> indexes;
	access.lock();
	for (int i = 0; i < LIMIT_DB_MUSIC; i++)
	{
		if (music[i].group == group) { indexes.push_back(i); }
	}
	// Select the random one
	if (indexes.size() > 0) {
		access.unlock();
		return music[indexes[math.rand(0, indexes.size() - 1)]];
	}
	// Return error-music
	console.error << "[ERROR] Requested incorrect music group [" << group << "]!" << endl;
	access.unlock();
	return music[0];
}

bool cDatabase::isItemGood(string type)
{
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].type == type) { return true; }
	}
	return false;
}

void cDatabase::clear()
{
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		item[i].descrLinesUsed = 0;
		item[i].dismantle.clear();
	}
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		unit[i].container.clear();
	}
}