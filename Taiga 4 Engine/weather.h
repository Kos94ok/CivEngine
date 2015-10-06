
#include "precomp.h"
#include "math.h"

#define WEATHER_SNOW					0
#define WEATHER_RAIN					1

#define LIMIT_WEATHERTYPE				8

class cWeatherCloud
{
public:
	int tex;
	float moveSpeed;
	vec2f pos;
	vec2f size;
	vec2f moveVector;
	float moveAngle;

	void updateGenData();
};

class cWeather
{
public:
	float power[LIMIT_WEATHERTYPE];
	float cloudDensity;
	float windPower;
	float targetPower[LIMIT_WEATHERTYPE];
	float targetCloud;
	float targetWind;

	vector<cWeatherCloud> cloud;
	vector<sf::Texture> cloudTexture;
	void createCloud(vec2f pos, vec2f size);
	void removeCloud(int id);

	//int changeType;
	//float changeTimer;
	//float changeTimerMax;

	void set(int id, float power);
	void setClouds(float power);
	void setWind(float power);
	void changeTo(int id, float power);
	void changeCloudsTo(float power);
	void changeWindTo(float power);

	cWeather() {
		power[WEATHER_SNOW] = math.randf(0.00f, 20000.00f);
		targetPower[WEATHER_SNOW] = math.randf(0.00f, 20000.00f);

		windPower = math.randf(1000.00f, 5000.00f);
		targetWind = math.randf(1000.00f, 5000.00f);

		cloudDensity = math.randf(500.00f, 20000.00f);
		targetCloud = math.randf(500.00f, 20000.00f);
	}
};

extern cWeather weather;