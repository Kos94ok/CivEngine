
#include "precomp.h"

class cPreRenderQueue
{
public:
	bool ready;
	std::vector<int> queue;
};

class cPreRender
{
public:
	cPreRenderQueue units;

	void updateUnits();
};

extern cPreRender preRender;
void preRenderMain();