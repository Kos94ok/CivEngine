
#pragma once
#include "precomp.h"

#define BUFF_FLASHLIGHT					0
#define BUFF_TORCH						1
#define BUFF_HEAT						2
#define BUFF_FOOD						3

class cBuff
{
public:
	int type;
	int power;
	int ownerUnit;
	float duration;

	cBuff() {
		type = 0;
		power = 0;
		ownerUnit = -1;
		duration = 0.00f;
	}
};

class cBuffList
{
public:
	std::vector<cBuff> list;

	void add(int type, float duration = -1.00f, int power = 1, int ownerUnit = -1);
	void add(cBuff entry);
	void remove(int type, int owner = -1);
	void removeById(int id);
	int getPower(int type);
	float getDuration(int type);
};