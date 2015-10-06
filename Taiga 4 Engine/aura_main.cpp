
#include "aura.h"

cAuraBuff newBuff;
void cAura::add(int type, int power, float range, bool fadeout, bool burnoutEffect)
{
	enabled = true;
	newBuff.type = type;
	newBuff.power = power;
	newBuff.range = range;
	newBuff.fadeout = fadeout;
	newBuff.burnoutEffect = burnoutEffect;
	buffList.push_back(newBuff);
}

void cAura::clear()
{
	enabled = false;
	buffList.clear();
}