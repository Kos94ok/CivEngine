
#pragma once
#include "precomp.h"
#include "item.h"

class cTarget
{
public:
	bool active;
	bool activeBuild;
	bool isBuildGood;
	int mode;
	int buildRef;
	int forButton;

	void enable_forButton(int elementId);
	void enable_building(int ref);
	void enable_building(cItem* item);
	void apply();
	void reset();
	void updateBuildStatus(int unitId);
	
	cTarget() {
		active = false;
		activeBuild = false;
	}
};

extern cTarget target;