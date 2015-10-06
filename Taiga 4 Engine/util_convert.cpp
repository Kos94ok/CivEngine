
#include "main.h"
#include "util.h"
#include "buff.h"

string cUtil::buffIdToType(int buff)
{
	if (buff == BUFF_HEAT) { return "heat"; }
	else if (buff == BUFF_FOOD) { return "food"; }
	return "missingno";
}

int cUtil::typeToBuffId(string str)
{
	if (str == "heat") { return BUFF_HEAT; }
	else if (str == "food") { return BUFF_FOOD; }
	return -1;
}

string cUtil::buildRefToType(int ref)
{
	if (ref == REF_ITEM_BUILD_CAMPFIRE) { return "campfire_full"; }
	else if (ref == REF_ITEM_BUILD_FURNACEBASIC) { return "furnace_basic"; }
	else if (ref == REF_ITEM_BUILD_FURNACEIND) { return "furnace_industrial"; }
	else if (ref == REF_ITEM_BUILD_TENT) { return "tent_basic"; }
	return "missingno";
}

int cUtil::typeToBuildRef(string str)
{
	if (str == "campfire_full") { return REF_ITEM_BUILD_CAMPFIRE; }
	else if (str == "furnace_basic") { return REF_ITEM_BUILD_FURNACEBASIC; }
	else if (str == "furnace_industrial") { return REF_ITEM_BUILD_FURNACEIND; }
	else if (str == "tent_basic") { return REF_ITEM_BUILD_TENT; }
	return -1;
}