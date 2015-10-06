
#include "main.h"
#include "math.h"
#include "game.h"

int cMath::round(float num) {
	return num < 0 ? num - 0.5 : num + 0.5;
}

float cMath::getDistance(float ax, float ay, float bx, float by) {
	return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

float cMath::getDistance(vec2f &a, vec2f &b) {
	return getDistance(a.x, a.y, b.x, b.y);
}

float cMath::getDistance(vec2i &a, vec2i &b) {
	return getDistance(math.round(a.x), math.round(a.y), math.round(b.x), math.round(b.y));
}

float cMath::getDistance(cUnit* unitA, cUnit* unitB) {
	return getDistance(unitA->pos, unitB->pos);
}

float cMath::getAngle(float ax, float ay, float bx, float by) {
	return math.RADTODEG * atan2((float)by - ay, (float)bx - ax);
}

float cMath::getAngle(vec2f a, vec2f b) {
	return getAngle(a.x, a.y, b.x, b.y);
}

float cMath::getAngle(vec2i a, vec2i b) {
	return getAngle(math.round(a.x), math.round(a.y), math.round(b.x), math.round(b.y));
}

float cMath::getAngle(cUnit* unitA, cUnit* unitB) {
	return getAngle(unitA->pos.x, unitA->pos.y, unitB->pos.x, unitB->pos.y);
}

float cMath::polarX(float x, float dist, float angle) {
	return x + dist * cos(angle * math.DEGTORAD);
}

float cMath::polarY(float y, float dist, float angle) {
	return y + dist * sin(angle * math.DEGTORAD);
}

vec2f cMath::polar(vec2f point, float dist, float angle) {
	return vec2f(polarX(point.x, dist, angle), polarY(point.y, dist, angle));
}

float cMath::randf(float min, float max) {
	return min + ((float)std::rand() / RAND_MAX) * (max - min);
}

int cMath::rand(int min, int max)
{
	if (min == max) { return min; }
	if (min < max) { return cMath::round(cMath::randf(min, max)); }
	if (min > max) { return cMath::round(cMath::randf(max, min)); }
	return 0;
}

// Convert the angle from [-180; 180] to [0; 360]
float cMath::convertAngle(float angle)
{
	if (angle < 0) { angle += 360; }
	angle = 360 - angle;
	return angle;
}

bool cMath::intToBool(int a) {
	if (a <= 0) { return false; }
	return true;
}

bool cMath::stringToBool(string str) {
	return intToBool(stringToInt(str));
}

int cMath::boolToInt(bool a) {
	if (a) { return 1; }
	return 0;
}

int cMath::stringToInt(string str) {
	int tmp;
	stringstream(str) >> tmp;
	return tmp;
}

bool cMath::isPointInRect(float x, float y, float rectX, float rectY, float rectSizeX, float rectSizeY)
{
	if (x < rectX) { return false; }
	if (y < rectY) { return false; }
	if (x > rectX + rectSizeX) { return false; }
	if (y > rectY + rectSizeY) { return false; }
	return true;
}