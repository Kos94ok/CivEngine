
#include "main.h"
#include "unit.h"
#include "audio.h"
#include "settings.h"
#include "camera.h"
#include "game.h"
#include "client.h"
#include "math.h"
#include "database.h"

void cAudio::playSound(string name)
{
	if (core.serverMode) { return; }
	soundQueue.push_back(cSoundQueue(name));
}

void cAudio::playSound(string name, vec2f pos)
{
	if (core.serverMode) { return; }
	soundQueue.push_back(cSoundQueue(name, pos));
}

void cAudio::playMusic(int group)
{
	if (core.serverMode) { return; }
	musicActiveGroup = group;
	musicRestartRequired = false;
}

void cAudio::playMusicFromStart(int group)
{
	if (core.serverMode) { return; }
	musicActiveGroup = group;
	musicRestartRequired = true;
}

void audioMain()
{
	int threadId = 9;
	if (core.serverMode) { return; }
	console << "[AUDIO] Starting the audio thread" << " [ID: " << threadId << "]" << "\n";
	int slot;
	cSound data;
	vec2f listener;
	cSoundQueue dataQueue;
	float dist, vol, volMod;
	while (!core.thread_shutdown[threadId])
	{
		// Adjusting listener's position
		if (settings.enableCameraListener) {
			listener = camera.pos + vec2f(camera.res.x, camera.res.y) / 2.00f;
		}
		else if (client.unit != -1) {
			listener = game.getUnit(client.unit).pos;
		}
		// Playing the first sound from the queue
		if (audio.soundQueue.size() > 0 && audio.getFreeSound() != -1)
		{
			slot = audio.getFreeSound();
			// Getting the next sound in the queue
			data = database.getSound(audio.soundQueue[0].name);
			// Executing
			if (ifstream("Data//Sounds//" + data.file).good()) {
				// Saving the information
				audio.soundData[slot] = audio.soundQueue[0];
				// Determining the volume modifier
					// Classification
				if (data.classification == AUDIO_FOOTSTEPS) { volMod = settings.volFootsteps; }
				else if (data.classification == AUDIO_EFFECT) { volMod = settings.volEffects; }
				else { volMod = settings.volEffects; }
					// Master volume
				volMod *= settings.volMaster;
				// Playing the sound
				audio.soundBuffer[slot].loadFromFile("Data//Sounds//" + data.file);
				audio.sound[slot].setBuffer(audio.soundBuffer[slot]);
				// Checking the volume
				if (audio.soundData[slot].positional) {
					dist = math.getDistance(listener, audio.soundData[slot].pos);
					vol = data.volume * volMod * min(1.00f, max(0.00f, (data.maxDist - dist + data.minDist) / data.maxDist));
					audio.sound[slot].setVolume(vol);
				}
				else {
					vol = data.volume * volMod;
					audio.sound[slot].setVolume(vol);
				}
				// Don't play the sound if the volume is zero
				if (vol > 1.00f) {
					audio.sound[slot].play();
				}
			}
			// Sound not found
			else {
				console.error << "[AUDIO] Can't open sound \"Data//Sounds//" + data.file + "\"" << endl;
			}
			// Erasing the used data
			audio.soundQueue.erase(audio.soundQueue.begin());
		}
		// No free sound slots
		else if (audio.soundQueue.size() > 0) {
			console.error << "[AUDIO] No free sound queue slots!" << endl;
		}
		// Thread routine
		core.thread_antifreeze[threadId] = 0;
		Sleep(1);
	}
	console << "[AUDIO] Cleaning up" << "\n";
	// Stopping all the sounds
	for (int i = 0; i < LIMIT_SOUND; i++)
	{
		audio.sound[i].stop();
		audio.sound[i].resetBuffer();
	}
}

void musicMain()
{
	int threadId = 12;
	if (core.serverMode) { return; }
	console << "[MUSIC] Starting the music thread" << " [ID: " << threadId << "]" << "\n";
	cMusic* data;
	vec2f listener;
	int elapsedTime, globalTime = 0;
	while (!core.thread_shutdown[threadId])
	{
		// Adjusting listener's position
		if (settings.enableCameraListener) {
			listener = camera.pos + vec2f(camera.res.x, camera.res.y) / 2.00f;
		}
		else if (client.unit != -1) {
			listener = game.getUnit(client.unit).pos;
		}
		// Get elapsed time
		elapsedTime = utilTimer.getElapsedTimeForThread(threadId);
		float timevar = (float)elapsedTime / 1000.00f * core.timeModifier;
		// Check if playing
		int nowPlaying = -1;
		for (int i = 0; i < LIMIT_MUSIC; i++)
		{
			if (audio.music[i].getVolume() > 1.00f) { nowPlaying = i; break; }
		}
		// Check each music group
		for (int i = 0; i < LIMIT_MUSIC; i++)
		{
			// Restarting the music
			if (audio.music[i].getStatus() == sf::Music::Stopped) {
				data = &database.getRandomMusic(i);
				audio.music[i].openFromFile("Data//Music//" + data->file);
				audio.musicData[i] = *data;
				audio.music[i].play();
			}
			// Handling the music change
				// Mute all the wrong groups
			if (i != audio.musicActiveGroup && audio.music[i].getVolume() > 0.01f) {
				audio.music[i].setVolume(audio.music[i].getVolume() - timevar * value.musicFadeOutSpeed);
				if (audio.music[i].getVolume() <= 0.01f) { audio.music[i].setVolume(0.00f); }
			}
				// Add volume to the correct group
			float maxVolume = audio.musicData[i].volume * settings.volMusic * settings.volMaster;
			if (i == audio.musicActiveGroup && (nowPlaying == -1 || nowPlaying == i)
				&& audio.music[i].getVolume() < maxVolume)
			{
				audio.music[i].setVolume(audio.music[i].getVolume() + timevar * value.musicFadeInSpeed);
				if (audio.music[i].getVolume() > maxVolume) {
					audio.music[i].setVolume(maxVolume);
				}
			}
		}

		// Check the idle sound
		audio.updateSoundEmitters();
		for (int i = 0; i < LIMIT_SOUNDIDLE; i++)
		{
			float vol = 0.00f, bestVol = 0.00f;
			// Calculating the volume
			for (int y = 0; y < (int)audio.soundEmitters.size(); y++)
			{
				cUnit* unit = &game.getUnit(audio.soundEmitters[y]);
				if (unit->sound.idle == audio.soundIdle[i].data.name)
				{
					float dist = math.getDistance(unit->pos, listener);
					vol = audio.soundIdle[i].data.volume * min(1.00f, max(0.00f, (audio.soundIdle[i].data.maxDist - dist + audio.soundIdle[i].data.minDist) / audio.soundIdle[i].data.maxDist));
					// Apply the burnout effect
					if (unit->hasRef(REF_UNIT_BURNOUT_CAMPFIRE)) { vol *= unit->resource / unit->resourceLimit; }
					// Check the best volume
					bestVol = max(vol, bestVol);
				}
			}
			// Applying the settings
			bestVol *= settings.volMaster * settings.volAmbient;
			// Starting the sound
			if (bestVol > 1.00f)
			{
				audio.soundIdle[i].handle.setVolume(bestVol);
				if (audio.soundIdle[i].handle.getStatus() == sf::Music::Stopped)
				{
					audio.soundIdle[i].handle.openFromFile("Data//Sounds//" + audio.soundIdle[i].data.file);
					audio.soundIdle[i].handle.setLoop(true);
					audio.soundIdle[i].handle.play();
				}
			}
			else {
				audio.soundIdle[i].handle.stop();
			}
		}


		// Thread routine
		core.thread_antifreeze[threadId] = 0;
		Sleep(50);
	}
	console << "[MUSIC] Cleaning up" << "\n";
	// Stopping all the music
	for (int i = 0; i < LIMIT_MUSIC; i++)
	{
		audio.music[i].stop();
	}
}