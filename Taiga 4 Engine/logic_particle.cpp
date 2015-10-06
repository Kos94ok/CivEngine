
#include "main.h"
#include "logic.h"
#include "game.h"
#include "camera.h"
#include "weather.h"
#include "math.h"
#include "particle.h"
#include "settings.h"

float cloudTimer = 0.00f;
float weatherTimer = 0.00f;
float particleTimer = 0.00f;
void cGameLogic::updateParticles(int elapsedTime)
{
	int id;
	float randVal;
	vec2f bufferArea = vec2f(1000.00f, 1000.00f), cloudPos, cloudSizeVec;
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	sf::FloatRect camRect(camera.pos.x, camera.pos.y, camera.res.x, camera.res.y);
	sf::FloatRect cloudRect;

	// Updating global weather data
		// Determine the new values
	weatherTimer += timevar;
	if (weatherTimer > 15.00f)
	{
		weatherTimer = 0.00f;
		// Only change weather on server side
		if (core.serverSide() && math.randf(0.00f, 1.00f) < 0.15f)
		{
			randVal = math.randf(0.00f, 1.00f);
			// Sunny ( 10% )
			if (randVal <= 0.10f) {
				//console.debug << "[DEBUG] Weather: Sunny" << endl;
				weather.changeTo(WEATHER_SNOW, 0.00f);
				weather.changeWindTo(0.00f);
				weather.changeCloudsTo(math.randf(500.00f, 2500.00f));
			}
			// Light snow ( 15% )
			else if (randVal <= 0.25f) {
				//console.debug << "[DEBUG] Weather: Light snow" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(2500.00f, 7500.00f));
				weather.changeWindTo(math.randf(0.00f, 3000.00f));
				weather.changeCloudsTo(math.randf(2500.00f, 10000.00f));
			}
			// Tons of snow ( 15% )
			else if (randVal <= 0.40f) {
				//console.debug << "[DEBUG] Weather: Tons of snow" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(15000.00, 25000.00f));
				weather.changeWindTo(math.randf(0.00f, 3000.00f));
				weather.changeCloudsTo(math.randf(30000.00f, 50000.00f));
			}
			// Really windy ( 10% )
			else if (randVal <= 0.50f) {
				//console.debug << "[DEBUG] Weather: Really windy" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(0.00f, 5000.00f));
				weather.changeWindTo(math.randf(15000.00f, 20000.00f));
				weather.changeCloudsTo(math.randf(500.00f, 10000.00f));
			}
			// Clouds everywhere ( 20% )
			else if (randVal <= 0.70f) {
				//console.debug << "[DEBUG] Weather: Clouds everywhere" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(0.00f, 1000.00f));
				weather.changeWindTo(math.randf(0.00f, 5000.00f));
				weather.changeCloudsTo(math.randf(30000.00f, 50000.00f));
			}
			// Blizzard ( 5% )
			else if (randVal <= 0.75f) {
				//console.debug << "[DEBUG] Weather: Blizzard" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(15000.00f, 25000.00f));
				weather.changeWindTo(math.randf(15000.00f, 20000.00f));
				weather.changeCloudsTo(math.randf(30000.00f, 50000.00f));
			}
			// Everything medium ( 15% )
			else if (randVal <= 0.90f) {
				//console.debug << "[DEBUG] Weather: Everything medium" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(7500.00f, 15000.00f));
				weather.changeWindTo(math.randf(5000.00f, 15000.00f));
				weather.changeCloudsTo(math.randf(15000.00f, 25000.00f));
			}
			// Completely random ( 10% )
			else {
				//console.debug << "[DEBUG] Weather: Random" << endl;
				weather.changeTo(WEATHER_SNOW, math.randf(0.00f, 25000.00f));
				weather.changeWindTo(math.randf(0.00f, 20000.00f));
				weather.changeCloudsTo(math.randf(500.00f, 50000.00f));
			}
		}
	}
		// Updating current values
	float snowPowerMod = 250.00f;
	float windPowerMod = 300.00f;
	float cloudPowerMod = 750.00f;
		// Snow power
	if (weather.power[WEATHER_SNOW] < weather.targetPower[WEATHER_SNOW]) {
		weather.power[WEATHER_SNOW] += snowPowerMod * timevar;
		if (weather.power[WEATHER_SNOW] > weather.targetPower[WEATHER_SNOW]) { weather.power[WEATHER_SNOW] = weather.targetPower[WEATHER_SNOW]; }
	}
	else if (weather.power[WEATHER_SNOW] > weather.targetPower[WEATHER_SNOW]) {
		weather.power[WEATHER_SNOW] -= snowPowerMod * timevar;
		if (weather.power[WEATHER_SNOW] < weather.targetPower[WEATHER_SNOW]) { weather.power[WEATHER_SNOW] = weather.targetPower[WEATHER_SNOW]; }
	}
		// Wind power
	if (weather.windPower < weather.targetWind) {
		weather.windPower += windPowerMod * timevar;
		if (weather.windPower > weather.targetWind) { weather.windPower = weather.targetWind; }
	}
	else if (weather.windPower > weather.targetWind) {
		weather.windPower -= windPowerMod * timevar;
		if (weather.windPower < weather.targetWind) { weather.windPower = weather.targetWind; }
	}
		// Cloud power
	if (weather.cloudDensity < weather.targetCloud) {
		weather.cloudDensity += cloudPowerMod * timevar;
		if (weather.cloudDensity > weather.targetCloud) { weather.cloudDensity = weather.targetCloud; }
	}
	else if (weather.cloudDensity > weather.targetCloud) {
		weather.cloudDensity -= cloudPowerMod * timevar;
		if (weather.cloudDensity < weather.targetCloud) { weather.cloudDensity = weather.targetCloud; }
	}
	

	// Spawning clouds
	cloudTimer += timevar;
	if (weather.cloudDensity > 0.00f)
	{
		int cloudCount = (int)weather.cloud.size();
		int targetCount = weather.cloudDensity * settings.cloudDensity / 100.00f;
		while (cloudCount < targetCount)
		{
			cloudCount = (int)weather.cloud.size();
			cloudTimer = 0.00f;
			if (cloudCount < targetCount)
			{
				int saver = 0;
				float cloudSize = math.randf(300.00f, 500.00f);
				do
				{
					//cloudPos.x = camera.pos.x + camera.res.x + cloudSize;
					//cloudPos.y = camera.pos.y + math.randf(0.00f, camera.res.y);
					cloudPos.x = math.randf(camera.pos.x - bufferArea.x, camera.pos.x + camera.res.x + bufferArea.x);
					cloudPos.y = math.randf(camera.pos.y - bufferArea.y, camera.pos.y + camera.res.y + bufferArea.y);
					cloudSizeVec.x = cloudSize;
					cloudSizeVec.y = cloudSize;
					cloudRect = sf::FloatRect(cloudPos - cloudSizeVec / 2.00f, cloudSizeVec);
					saver += 1;
				} while (cloudRect.intersects(camRect) && saver < 200);
				if (saver < 200) { weather.createCloud(cloudPos, cloudSizeVec); }
			}
		}
	}

	// Updating the cloud data
	cWeatherCloud* cloud;
	float moveMod = timevar * max(0.20f, weather.windPower / 10000.00f);
	mutex.renderClouds.lock();
	for (int i = 0; i < (int)weather.cloud.size(); i++)
	{
		cloud = &weather.cloud[i];
		cloud->pos += cloud->moveVector * moveMod;
		if (weather.cloud[i].pos.x < camera.pos.x - bufferArea.x) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.x > camera.pos.x + camera.res.x + bufferArea.x) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.y < camera.pos.y - bufferArea.y) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.y > camera.pos.y + camera.res.y + bufferArea.y) { weather.removeCloud(i); }
	}
	mutex.renderClouds.unlock();

	// Spawning snow
	particleTimer += timevar;
	mutex.renderParticles.lock();
	if (particleTimer >= 0.05f)
	{
		particleTimer = 0;
		if (weather.power[WEATHER_SNOW] > 0.00f)
		{
			int count = math.round(weather.power[WEATHER_SNOW] * settings.particleDensity / 1000.00f);
			for (int i = 0; i < math.rand(max(0, count - 5), count); i++)
			{
				vec2f spawnPoint = vec2f(camera.pos.x + math.randf(0.00, camera.res.x), camera.pos.y + math.randf(0.00f, camera.res.y));
				id = particle.addUnit("weather_snow", spawnPoint, math.randf(265.00f, 275.00f) + (weather.windPower / 10000.00f) * 30.00f, math.randf(1.00f, 2.00f), 45.00f);
				particle.unit[id].movementSpeed = math.rand(50, 100) * max(1.00f, weather.windPower / 2500.00f);
				particle.unit[id].updateGenData();
				particle.unit[id].setFade(FADE_IN, 0.50f);
			}
		}
	}

	// Updating the particle data
	float shadowAngleMod = 4.00f;
	float shadowScaleMod = 1.00f;
	float shadowScaleMin = 1.20f;
	float shadowAngle = 0.00f, shadowScale = 1.20f;

	float timeLocal = game.timeOfDay;
	shadowScale = abs(timeLocal - 12.00f) / 6.00f * shadowScaleMod + shadowScaleMin;
	shadowAngle = (timeLocal - 12.00f) * shadowAngleMod;

	cParticleUnit* unit;
	for (int i = 0; i < (int)particle.unit.size(); i++)
	{
		unit = &particle.unit[i];
		unit->pos -= unit->moveVector * timevar;
		// Always on screen
		if (unit->hasRef(REF_PARTICLE_ONSCREEN))
		{
			if (unit->pos.x < camera.pos.x) { unit->pos.x += camera.res.x; }
			else if (unit->pos.x > camera.pos.x + camera.res.x) { unit->pos.x -= camera.res.x; }
			else if (unit->pos.y < camera.pos.y) { unit->pos.y += camera.res.y; }
			else if (unit->pos.y > camera.pos.y + camera.res.y) { unit->pos.y -= camera.res.y; }
		}
		unit->updateShadowPos(shadowAngle, shadowScale);
		// Life timer
		if (unit->fadeType != FADE_OUT)
		{
			unit->lifetime -= timevar;
			if (unit->lifetime <= 0.00f) {
				unit->moveVector = vec2f(0.00f, 0.00f);
				unit->lifetime = 0.00f;
				unit->setFade(FADE_OUT, 0.50f);
			}
		}
		// Fade
		if (unit->fadeType != FADE_STOP)
		{
			if (unit->fadeType == FADE_IN) {
				unit->fadeVal += timevar;
				if (unit->fadeVal >= unit->fadeMax) { unit->setFade(FADE_STOP, 1.00f); }
			}
			else if (unit->fadeType == FADE_OUT) {
				unit->fadeVal -= timevar;
				if (unit->fadeVal <= 0.00f) {
					particle.removeUnit(i);
					i -= 1;
				}
			}
		}
	}
	mutex.renderParticles.unlock();
}