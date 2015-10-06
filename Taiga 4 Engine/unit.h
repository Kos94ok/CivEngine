
#pragma once
#include "precomp.h"
#include "define.h"
#include "audio.h"
#include "refSystem.h"
#include "ai.h"
#include "item.h"
#include "animation.h"
#include "buff.h"
#include "aura.h"

class cUnitSoundbank
{
public:
	string idle;
	cSoundFootsteps footsteps;
};

class cOrder
{
public:
	int type;
	int targetObject;
	int paramA;
	sf::Vector2f targetPos;
};

class cLightSource
{
public:
	float power;
	int texture;

	int priority;
	bool directional;
	float flickerMod;
	float flickerTime;
	float flickerCurTime;

	cLightSource() {
		priority = 0;
		power = 0.00f;
		texture = -1;
		flickerMod = 0.00f;
		flickerCurTime = 0.00f;
		directional = false;
	}

	cLightSource* operator=(int textureIndex)
	{
		texture = textureIndex;
		return this;
	}
};

class cShader
{
public:
	int id;
	int bloomSampleCount;
	float bloomSampleOffset;

	cShader() {
		id = -1;
		bloomSampleCount = 1;
		bloomSampleOffset = 0.005f;
	}
};

class cLifeTimer
{
public:
	bool enabled;
	float time;

	cLifeTimer()
	{
		enabled = false;
	}
};

class cSelectionOffset
{
public:
	float top;
	float bot;
	float left;
	float right;

	cSelectionOffset() {
		top = 0.00f;
		bot = 0.00f;
		left = 0.00f;
		right = 0.00f;
	}
	cSelectionOffset(vec4f input) {
		top = input.a;
		bot = input.b;
		left = input.c;
		right = input.d;
	}
};

class cCharacter : public cReference
{
public:
	int owner;
	int globalId;
	string type;
	vec2f pos;
	vec2f center;
	vec2f size;
	vec2f shadowOffset;
	vec2f lastGoodPos;
	float interactDistance;
	float collisionDistance;
	float flyingHeight;
	int selectionPriority;
	cSelectionOffset selectionOffset;

	bool animAvailable(int animType);

	cCurrentAnim anim;
	cAnimDisplay animDisplay;
	cAnimation animData[LIMIT_ANIMATIONS];
	cLightSource light;
	cShader shader;
};

class cUnit : public cCharacter
{
public:
	bool toRemove;
	int orderCounter;
	cOrder order[LIMIT_ORDERS];
	float actionTimer;
	sf::Vector2i chunkPos;

	int addOrder_moveto(vec2f target, bool overwrite = true);
	int addOrder_moveto_path(vec2f target, bool overwrite = true);
	int addOrder_pickup(int target, bool overwrite = true);
	int addOrder_harvest(int target, bool overwrite = true, int powerLevel = POWER_HAND);
	int addOrder_packunit(int target, bool overwrite = true);
	int addOrder_death(bool overwrite = true);
	void removeOrder(int id);
	void clearOrders();
	void removeAllOrders();

	float health;
	float displayHealth;
	float maxHealth;
	float facingAngle;
	float targetFacingAngle;
	float rotateSpeed;
	float movementSpeed;
	cAura aura;
	cAIBrain ai;
	cBuffList buff;
	cLifeTimer lifeTimer;
	cItemContainer container;
	cDropContainer drop;
	cUnitSoundbank sound;

	float resource;
	float resourceLimit;
	void addResource(float d, bool sendMessage = true);
	void setResource(float value, bool sendMessage = true);

	void moveTo(vec2f newPos);
	void rotateTo(float newAngle);
	void addHealth(float d);
	void addMaxHealth(float d);
	void setHealth(float hp);
	void setMaxHealth(float hp);
	void addItem(string type, int count = 1, bool toLog = true);
	void removeItem(string type, int count = -1, bool toLog = true);
	void removeItem(int id, int count = -1, bool toLog = true);
	void addBuff(int type, float duration = -1.00f, int power = 1, int owner = -1);
	void addBuff(cBuff entry);
	void removeBuff(int type);
	bool hasBuff(int type);
	void setLifeTimer(float time);
	void resetLifeTimer();
	void updateFacing();
	void updateAction();
	void updateAnimation();
	void updateDisplayAnim();

	cUnit() {
		toRemove = false;
		flyingHeight = 0.00f;
		actionTimer = 0.00f;
		resource = 0.00f;
		resourceLimit = 1000000.00f;
		interactDistance = 0.00f;
		collisionDistance = 0.00f;
		orderCounter = 0;
		facingAngle = 0.00f;
		targetFacingAngle = facingAngle;
		health = 50.00f;
		displayHealth = health;
		maxHealth = 100.00f;
		owner = -1;
		selectionPriority = 0;
		rotateSpeed = 360.00f;
	}
};