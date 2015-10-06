
#include "particle.h"
#include "database.h"
#include "math.h"

void cParticleUnit::setFade(int type, float time)
{
	fadeType = type;
	if (type == FADE_IN) {
		fadeVal = 0.00f;
		fadeMax = time;
	}
	else {
		fadeVal = time;
		fadeMax = time;
	}
}

void cParticleUnit::updateGenData()
{
	moveVector = vec2f(movementSpeed * cos(moveAngle * math.DEGTORAD), movementSpeed * sin(moveAngle * math.DEGTORAD));
}

void cParticleUnit::updateShadowPos(float shadowAngle, float shadowScale)
{
	shadowPos = math.polar(pos, flyingHeight * shadowScale * (lifetime / lifetimeMax), shadowAngle * flyingHeight / 10.00f - 90.00f);
}

int cParticle::addUnit(string type, vec2f pos, float moveAngle, float lifetime, float height)
{
	cParticleUnit newUnit = database.getParticle(type);
	newUnit.pos = pos;
	newUnit.lifetime = lifetime;
	newUnit.lifetimeMax = lifetime;
	newUnit.moveAngle = moveAngle;
	newUnit.flyingHeight = height;
	newUnit.flyingHeightMax = height;
	newUnit.updateGenData();

	unit.push_back(newUnit);
	unitCounter += 1;
	return unitCounter - 1;
}

void cParticle::removeUnit(int id)
{
	unit.erase(unit.begin() + id);
	unitCounter -= 1;
}