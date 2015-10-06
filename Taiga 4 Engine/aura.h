
#pragma once
#include "precomp.h"
#include "buff.h"

class cAuraBuff
{
public:
	int type;
	int power;
	float range;
	bool fadeout;
	bool burnoutEffect;
};

class cAura
{
public:
	bool enabled;
	vector<cAuraBuff> buffList;

	void add(int type, int power, float range, bool fadeout = false, bool burnoutEffect = false);
	void clear();
};