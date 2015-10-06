
#pragma once
#include "precomp.h"
#include "util.h"

class cPath
{
public:
	cMutex access;

	int waypointCounter;
	vec2f waypoint[LIMIT_ORDERS];

	void addWaypoint(vec2f loc);
	void clear();
	bool isPointFree(vec2f loc, float dist, int unitId);
	vec2f validatePoint(vec2f loc, float dist, int unitId);
	bool calculate(vec2f start, vec2f end, float collision, int unitId);
};

extern cPath path;