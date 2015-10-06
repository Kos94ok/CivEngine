
#pragma once
#include "precomp.h"
#include "define.h"
#include "unit.h"
#include "item.h"
#include "particle.h"
#include "UI.h"
#include "util.h"
#include "audio.h"

#define LIMIT_DB_UNIT					64
#define LIMIT_DB_ITEM					64
#define LIMIT_DB_UIELEMENTS				32
#define LIMIT_DB_TEXTURES				512
#define LIMIT_DB_PARTICLES				32
#define LIMIT_DB_SOUNDS					32
#define LIMIT_DB_MUSIC					16

//extern cVisual visual;

class cDatabaseKey
{
public:
	std::string name;
	std::string value;
};

class cDatabaseExternal
{
public:
	std::vector<std::string> fileList;
	bool getFileList();

	bool parseFile(int index);
	bool getNextKey(std::ifstream *file, cDatabaseKey *key);
};

class cDatabase
{
public:
	cMutex access;

	cDatabaseExternal ext;
	cSound sound[LIMIT_DB_SOUNDS];
	cUnit unit[LIMIT_DB_UNIT];
	cItem item[LIMIT_DB_ITEM];
	cUIElement uiElement[LIMIT_DB_UIELEMENTS];
	cParticleUnit particle[LIMIT_DB_PARTICLES];
	cMusic music[LIMIT_DB_MUSIC];
	int texture[LIMIT_DB_TEXTURES];

	void init();
	void loadSounds();
	void loadUnits();
	void loadItems();
	void loadUI();
	void loadParticles();
	void loadTextures();
	void loadMusic();
	void loadExternal();
	void generateTextures();
	void clear();

	cSound& getSound(string type);
	cUnit& getUnit(string type);
	cItem& getItem(string type);
	cItem& findItem(int ref);
	cUIElement& getUIElement(string type);
	cParticleUnit& getParticle(string type);
	cMusic& getRandomMusic(int group);

	bool isItemGood(string type);
};

extern cDatabase database;