
#pragma once
#include "precomp.h"
#include "define.h"

class cReference
{
public:
	int refCounter;
	bool ref[LIMIT_REFERENCE];
	void addRef(int id);
	bool hasRef(int id);
	void removeRef(int id);
};