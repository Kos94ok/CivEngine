
#pragma once
#include "precomp.h"
#include "define.h"
#include "refSystem.h"

class cParticleUnit : public cReference
{
public:
	int myId;
	std::string type;
	sf::Vector2f size;
	float lifetime;
	float lifetimeMax;

	int texture;
	float movementSpeed;

	float moveAngle;
	float flyingHeight;
	float flyingHeightMax;
	sf::Vector2f pos;
	sf::Vector2f shadowPos;

	sf::Vector2f moveVector;

	int fadeType;
	float fadeVal;
	float fadeMax;

	void setFade(int type, float time);
	void updateGenData();
	void updateShadowPos(float shadowAngle, float shadowScale);

	cParticleUnit() {
		fadeType = FADE_STOP;
	}
};

class cParticle
{
public:
	int unitCounter;
	std::vector<cParticleUnit> unit;

	int addUnit(std::string type, sf::Vector2f pos, float moveAngle, float lifetime, float height);
	void removeUnit(int id);

	cParticle() {
		unitCounter = 0;
	}
};

extern cParticle particle;