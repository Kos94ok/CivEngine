
#include "main.h"
#include "database.h"

void cDatabase::loadSounds()
{
	int i = 0;
	sound[i].name = "missingno";
	sound[i].volume = 0.00f;
	i += 1;

	// Effects
	sound[i].name = "rifle_shot";
	sound[i].file = "rifle.ogg";
	sound[i].volume = 50.00f;
	sound[i].minDist = 100.00f;
	sound[i].maxDist = 700.00f;
	sound[i].classification = AUDIO_EFFECT;
	i += 1;

	// Idle
		// Campfire
	sound[i].name = "campfire";
	sound[i].file = "fire2.ogg";
	sound[i].volume = 50.00f;
	sound[i].minDist = 100.00f;
	sound[i].maxDist = 700.00f;
	sound[i].classification = AUDIO_AMBIENT;
	audio.registerIdleSound(sound[i]);
	i += 1;

		// Wind
	sound[i].name = "wind_tree";
	sound[i].file = "wind_light.wav";
	sound[i].volume = 20.00f;
	sound[i].minDist = 50.00f;
	sound[i].maxDist = 1000.00f;
	sound[i].classification = AUDIO_AMBIENT;
	audio.registerIdleSound(sound[i]);
	i += 1;

	// Footsteps
		// Human
	sound[i].name = "footsteps_human_snow001";
	sound[i].file = "footsteps_snow001.wav";
	sound[i].volume = 50.00f;
	sound[i].minDist = 0.00f;
	sound[i].maxDist = 500.00f;
	sound[i].classification = AUDIO_FOOTSTEPS;
	i += 1;

	sound[i] = sound[i - 1];
	sound[i].name = "footsteps_human_snow002";
	sound[i].file = "footsteps_snow002.wav";
	sound[i].volume = 30.00f;
	i += 1;

	sound[i] = sound[i - 1];
	sound[i].name = "footsteps_human_snow003";
	sound[i].file = "footsteps_snow003.wav";
	sound[i].volume = 50.00f;
	i += 1;

		// Animals
	sound[i].name = "footsteps_animal_snow001";
	sound[i].file = "footsteps_snow001.wav";
	sound[i].volume = 10.00f;
	sound[i].minDist = 0.00f;
	sound[i].maxDist = 200.00f;
	sound[i].classification = AUDIO_FOOTSTEPS;
	i += 1;

	sound[i] = sound[i - 1];
	sound[i].name = "footsteps_animal_snow002";
	sound[i].file = "footsteps_snow002.wav";
	sound[i].volume = 7.00f;
	i += 1;

	sound[i] = sound[i - 1];
	sound[i].name = "footsteps_animal_snow003";
	sound[i].file = "footsteps_snow003.wav";
	sound[i].volume = 10.00f;
	i += 1;
}

void cDatabase::loadMusic()
{
	int i = 0;
	music[i].file = "missingno";
	i += 1;

	music[i].file = "main.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_MENU;
	i += 1;

	music[i].file = "ambient_day01.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_DAY;
	i += 1;

	music[i].file = "ambient_day02.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_DAY;
	i += 1;

	music[i].file = "ambient_night01.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_NIGHT;
	i += 1;

	music[i].file = "ambient_night02.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_NIGHT;
	i += 1;

	music[i].file = "ambient_campfire01.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_CAMPFIRE;
	i += 1;

	music[i].file = "ambient_campfire02.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_CAMPFIRE;
	i += 1;

	music[i].file = "credits.ogg";
	music[i].volume = 100.00f;
	music[i].group = MUSIC_CREDITS;
	i += 1;
}