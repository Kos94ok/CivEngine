
#pragma once
#include "define.h"
#include "util.h"
#include "refSystem.h"

#define LIMIT_MUSIC						5
#define LIMIT_SOUND						128
#define LIMIT_SOUNDIDLE					8

#define AUDIO_MUSIC						0
#define AUDIO_EFFECT					1
#define AUDIO_AMBIENT					2
#define AUDIO_WEATHER					3
#define AUDIO_FOOTSTEPS					4

#define MUSIC_MENU						0
#define MUSIC_DAY						1
#define MUSIC_NIGHT						2
#define MUSIC_CAMPFIRE					3
#define MUSIC_CREDITS					4

class cSound
{
public:
	string name;
	string file;
	float volume;
	float minDist;
	float maxDist;
	int classification;

	cSound() {
		name = "";
		file = "";
		minDist = 0.00f;
		maxDist = 300.00f;
	}
};

class cSoundIdle
{
public:
	cSound data;
	sf::Music handle;
};

class cSoundFootsteps
{
public:
	vector<int> frames;
	vector<string> data;

	void setFrames(int a = -1, int b = -1, int c = -1, int d = -1, int e = -1, int f = -1) {
		frames.clear();
		if (a != -1) { frames.push_back(a); }
		if (b != -1) { frames.push_back(b); }
		if (c != -1) { frames.push_back(c); }
		if (d != -1) { frames.push_back(d); }
		if (e != -1) { frames.push_back(e); }
		if (f != -1) { frames.push_back(f); }
	}
	void setData(string a = "", string b = "", string c = "", string d = "", string e = "", string f = "") {
		data.clear();
		if (a != "") { data.push_back(a); }
		if (b != "") { data.push_back(b); }
		if (c != "") { data.push_back(c); }
		if (d != "") { data.push_back(d); }
		if (e != "") { data.push_back(e); }
		if (f != "") { data.push_back(f); }
	}
};

class cSoundQueue
{
public:
	string name;
	vec2f pos;

	bool positional;

	cSoundQueue() {}
	cSoundQueue(string name, vec2f pos = vec2f(0.00f, 0.00f)) {
		this->name = name;
		if (pos == vec2f(0.00f, 0.00f)) {
			positional = false;
		}
		else {
			positional = true;
			this->pos = pos;
		}
	}
};

class cSoundMapEntry
{
public:
	vec2f pos;
	string sound;
};

class cMusic
{
public:
	int group;
	string file;
	float volume;

	cMusic() {
		group = MUSIC_MENU;
		file = "";
		volume = 100.00f;
	}
};

class cAudio
{
public:
	cMutex access;

	sf::Sound sound[LIMIT_SOUND];
	sf::SoundBuffer soundBuffer[LIMIT_SOUND];
	cSoundQueue soundData[LIMIT_SOUND];
	vector<cSoundQueue> soundQueue;

	int musicActiveGroup;
	bool musicRestartRequired;
	sf::Music music[LIMIT_MUSIC];
	cMusic musicData[LIMIT_MUSIC];

	int soundIdleCount;
	cSoundIdle soundIdle[LIMIT_SOUNDIDLE];
	vector<int> soundEmitters;
	// Calling this function will add the sound from the database to a local db
	void registerIdleSound(cSound data);
	// Calling this function will rebuild the internal sound emitter unit list
	void updateSoundEmitters();

	void playSound(string name);
	void playSound(string name, vec2f pos);
	void playMusic(int group);
	void playMusicFromStart(int group);

	int getFreeSound();

	cAudio() {
		soundIdleCount = 0;
		musicActiveGroup = -1;
		musicRestartRequired = false;
		for (int i = 0; i < LIMIT_MUSIC; i++) {
			music[i].setVolume(0.00f);
		}
	}
};

extern cAudio audio;
void audioMain();
void musicMain();