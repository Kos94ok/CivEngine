
#pragma once
#include "precomp.h"

class cUnit;

class cMath
{
public:
	int round(float num);
	float getDistance(float ax, float ay, float bx, float by);
	float getDistance(vec2f &a, vec2f &b);
	float getDistance(vec2i &a, vec2i &b);
	float getDistance(cUnit* unitA, cUnit* unitB);
	float getAngle(float ax, float ay, float bx, float by);
	float getAngle(vec2f a, vec2f b);
	float getAngle(vec2i a, vec2i b);
	float getAngle(cUnit* unitA, cUnit* unitB);
	float polarX(float x, float dist, float angle);
	float polarY(float y, float dist, float angle);
	vec2f polar(vec2f point, float dist, float angle);
	float randf(float min, float max);
	int rand(int min, int max);
	float convertAngle(float angle);
	bool intToBool(int a);
	bool stringToBool(std::string str);
	int boolToInt(bool a);
	int stringToInt(std::string str);
	bool isPointInRect(float x, float y, float rectX, float rectY, float rectSizeX, float rectSizeY);

	float PI;
	float RADTODEG;
	float DEGTORAD;

	cMath()
	{
		PI = 3.14159f;
		RADTODEG = 180.00f / PI;
		DEGTORAD = PI / 180.00f;
	}
};

extern cMath math;