
#include "main.h"
#include "audio.h"
#include "database.h"
#include "settings.h"
#include "game.h"

int cAudio::getFreeSound()
{
	for (int i = 0; i < LIMIT_SOUND; i++)
	{
		if (sound[i].getStatus() == sf::Sound::Stopped) { return i; }
	}
	return -1;
}

void cAudio::registerIdleSound(cSound data)
{
	soundIdle[soundIdleCount].data = data;
	soundIdleCount += 1;
}

void cAudio::updateSoundEmitters()
{
	mutex.audioMain.lock();

	soundEmitters.clear();
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].sound.idle.length() > 0)
		{
			soundEmitters.push_back(game.unit[i].globalId);
		}
	}

	mutex.audioMain.unlock();
}