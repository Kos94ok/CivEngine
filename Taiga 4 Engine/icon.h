
#pragma once
#include "precomp.h"

class cIcon
{
public:
	int tex;

	cIcon() {
		tex = -1;
	}

	cIcon(int texValue)
	{
		tex = texValue;
	}

	cIcon* operator=(int textureIndex)
	{
		tex = textureIndex;
		return this;
	}

	bool operator==(int textureIndex)
	{
		return tex == textureIndex;
	}

	bool operator!=(int textureIndex)
	{
		return tex != textureIndex;
	}
};