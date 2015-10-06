
#pragma once
#include "precomp.h"

class cPacketQueue
{
public:
	int id;
	int target;
	sf::Packet data;
};

class cColor
{
public:
	int r;
	int g;
	int b;
	int a;

	cColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	};

	cColor(int R, int G, int B, int A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
};

class cCodeStats
{
public:
	int linesOfCode;
	int headerFiles;
	int sourceFiles;

	void calculate();
};

extern cCodeStats codeStats;

class cUtil
{
public:
	void detectVideoCard();
	int getInventorySortingType();
	void checkLog(string filename);
	void checkLogFiles();
	string getCurrentTimeString(bool removeSemi = false, bool includeDay = false);
	
	sf::Color convertUnitHighlightColor(int code);
	sf::Color parseOldschoolColorCode(string code);
	string buildRefToType(int ref);
	int typeToBuildRef(string str);
	string buffIdToType(int buff);
	int typeToBuffId(string str);

	void copyFile(string input, string output);
	void copyFolder(string input, string output);
	void clearFolder(string input);

	bool screenshotRequested;
	void makeScreenshot();

	void checkLaunchStatus();
	
	bool intersects(vec2f pos, vec2f rectPos, vec2f rectSize);
};

extern cUtil util;

class cValue
{
public:
	const float rifleDamage = 10.00f;
	const float rifleMaxDistance = 1750.00f;

	const float maxBuildDist = 200.00f;

	const float enemyDamage = 10.00f;

	const float chatNoFocusTimer = 30.00f;
	const float chatRecentMessageTimer = 30.00f;
	const float progressBarFadeInTimer = 0.10f;
	const float progressBarFadeOutTimer = 0.30f;

	const float statCold_normal_day = 0.50f;
	const float statCold_normal_night = 0.80f;
	const float statHunger_normal = 0.20f;

	const float mouseDoubleClickTimer = 0.20f;

	const float scrollSpeed_mouse_normal = 1000.00f;
	const float scrollSpeed_keyboard_normal = 1000.00f;
	const float scrollSpeed_mouseWheel_normal = 5.00f;

	const float musicFadeInSpeed = 5.00f;
	const float musicFadeOutSpeed = 5.00f;

	float getMinColdLocal() { return -5.00f; }
	float getMaxColdLocal() { return 100.00f; }
	float getMinHungerLocal() { return -5.00f; }
	float getMaxHungerLocal() { return 100.00f; }
};

extern cValue value;

class cMutex
{
public:
	sf::Mutex mutex;

	void lock() {
		mutex.lock();
	}
	void unlock() {
		mutex.unlock();
	}
};

class cMutexGlobal
{
public:
	cMutex animation;
	cMutex renderMain;
	cMutex renderUnits;
	cMutex renderClouds;
	cMutex renderParticles;
	cMutex mainMatrix;

	cMutex serverHistory;
	cMutex serverPacketQueue;

	cMutex audioMain;
	cMutex worldMain;
};

extern cMutexGlobal mutex;

class cArg
{
public:
	string data[4];

	cArg(string a = "0", string b = "0", string c = "0", string d = "0") {
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
	}

	cArg(int i) {
		cArg();
	}

	string operator [](int i) const { return data[i]; }
	string & operator [](int i) { return data[i]; }
};

class cUtilTimer
{
public:
	int threadTimer[LIMIT_THREAD];

	int getElapsedTimeForThread(int threadId);
};

extern cUtilTimer utilTimer;

template <typename type> class Vector4
{
public:
	type a, b, c, d;

	Vector4(type a, type b, type c, type d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
};

typedef Vector4<int> vec4i;
typedef Vector4<float> vec4;
typedef Vector4<float> vec4f;