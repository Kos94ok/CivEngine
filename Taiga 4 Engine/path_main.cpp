
#include "main.h"
#include "path.h"
#include "game.h"
#include "math.h"
#include "world.h"

bool cPath::calculate(vec2 start, vec2 end, float collision, int unitId)
{
	access.lock();
	clear();

	vec2 iter = start, step, oldIter;
	bool objFound = true;
	float localAngle;
	// Validating the point
	end = validatePoint(end, collision, unitId);
	// Calculating the precision
	float precision = 0.00f;
	float dist = math.getDistance(start, end);
	if (dist > 500.00f) { precision = 7.00f; }
	else if (dist > 200.00f) { precision = 6.00f; }
	else if (dist > 100.00f) { precision = 5.00f; }
	else { precision = 4.00f; }

	int i = 0;
	for (i = 0; i < 100; i++)
	{
		float angle = math.getAngle(iter, end);
		localAngle = angle;
		// Looking for path
		objFound = true;
		for (int u = 0; u < 18 && objFound; u++)
		{
			step = vec2(cos(localAngle * math.DEGTORAD) * precision, sin(localAngle * math.DEGTORAD) * precision);
			objFound = false;
			// Checking the path for given angle
			for (int y = 0; y < 20; y++)
			{
				if (!isPointFree(iter + step * (float)y, collision, unitId))
				{				
					objFound = true;
					y = 20;
				}
			}
			// Adjust angle and try again
			if (objFound)
			{
				if (u % 2 == 0) { localAngle = angle + u * (10.00f - precision); }
				else { localAngle = angle - u * (10.00f - precision); }
			}
		}
		// If good point found
		if (!objFound)
		{
			if (math.getDistance(iter, end) < precision * 7.50f) { addWaypoint(end); access.unlock(); return true; }
			else {
				iter = iter + step * 5.00f;
				addWaypoint(iter);
			}
		}
		else { access.unlock(); return false; }
	}
	access.unlock();
	return false;
}

vec2 cPath::validatePoint(vec2 loc, float dist, int unitId)
{
	if (isPointFree(loc, dist, unitId)) { return loc; }
	vec2 temp;
	for (int y = 5; y < 100; y += 5)
	{
		for (int i = 0; i < 360; i+= 5)
		{
			temp = math.polar(loc, y, i);
			if (isPointFree(temp, dist, unitId)) { return temp; }
		}
	}
	return loc;
}

bool cPath::isPointFree(vec2 loc, float dist, int unitId)
{
	vec2i chunk = world.getChunkInPos(loc);
	// Checking for bad chunks
	if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	// Checking for unit collision
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (!game.unit[i].hasRef(REF_UNIT_NOCOLLISION) && math.getDistance(game.unit[i].pos, loc) <= game.unit[i].collisionDistance + dist && game.unit[i].globalId != unitId)
		{
			return false;
		}
	}
	return true;
}

void cPath::addWaypoint(vec2 loc)
{
	waypoint[waypointCounter] = loc;
	waypointCounter += 1;
}

void cPath::clear()
{
	waypointCounter = 0;
}