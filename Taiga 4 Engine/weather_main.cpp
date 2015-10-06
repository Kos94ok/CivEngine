
#include "main.h"
#include "weather.h"
#include "math.h"
#include "camera.h"
#include "console.h"
#include "visual.h"
#include "database.h"
#include "server.h"

void cWeatherCloud::updateGenData()
{
	moveVector = vec2f(moveSpeed * cos(moveAngle * math.DEGTORAD), moveSpeed * sin(moveAngle * math.DEGTORAD));
}

void cWeather::set(int id, float power)
{
	this->power[id] = power;
}

void cWeather::setClouds(float power)
{
	cloudDensity = power;
}

void cWeather::setWind(float power)
{
	windPower = power;
}

void cWeather::changeTo(int id, float power)
{
	targetPower[id] = power;
	// Server side
	if (core.serverSide())
	{
		sf::Packet data;
		data << MSG_GAME_WEATHER << id << power;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cWeather::changeCloudsTo(float power)
{
	targetCloud = power;
	// Server side
	if (core.serverSide())
	{
		sf::Packet data;
		data << MSG_GAME_CLOUD << power;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cWeather::changeWindTo(float power)
{
	targetWind = power;
	// Server side
	if (core.serverSide())
	{
		sf::Packet data;
		data << MSG_GAME_WIND << power;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

cWeatherCloud newCloud;
void cWeather::createCloud(vec2f pos, vec2f size)
{
	newCloud.tex = database.texture[TEX_GEN_CLOUD + math.rand(0, LIMIT_GENTEXCOUNT_CLOUD - 1)];
	newCloud.pos = pos;
	newCloud.size = size;
	newCloud.moveSpeed = math.randf(10.00f, 50.00f);
	newCloud.moveAngle = math.randf(180.00f, 180.00f);
	//sf::Texture gen = visual.genCloudTexture(size, pieceSizeMin, pieceSizeMax, pieceCount);
	newCloud.updateGenData();

	mutex.renderClouds.lock();
	cloud.push_back(newCloud);
	//cloudTexture.push_back(gen);
	mutex.renderClouds.unlock();
}

void cWeather::removeCloud(int id)
{
	cloud[id] = cloud[(int)cloud.size() - 1];
	cloud.pop_back();
}