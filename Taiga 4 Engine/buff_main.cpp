
#include "buff.h"

void cBuffList::add(int type, float duration, int power, int ownerUnit)
{
	cBuff entry;
	entry.type = type;
	entry.power = power;
	entry.duration = duration;
	entry.ownerUnit = ownerUnit;

	add(entry);
}

void cBuffList::add(cBuff entry)
{
	list.push_back(entry);
}

void cBuffList::remove(int type, int owner)
{
	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type && (owner == -1 || list[i].ownerUnit == owner)) {
			list.erase(list.begin() + i);
			i -= 1;
		}
	}
}

void cBuffList::removeById(int id)
{
	list.erase(list.begin() + id);
}

int cBuffList::getPower(int type)
{
	int retVal = 0;

	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type) {
			retVal += list[i].power;
		}
	}

	return retVal;
}

float cBuffList::getDuration(int type)
{
	float retVal = 0.00f;

	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type) {
			retVal += list[i].duration;
		}
	}

	return retVal;
}