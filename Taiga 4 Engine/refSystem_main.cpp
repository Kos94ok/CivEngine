
#include "main.h"
#include "refSystem.h"

void cReference::addRef(int id) {
	if (id < 0 || id > LIMIT_REFERENCE) { return; }
	ref[id] = true;
}

bool cReference::hasRef(int id) {
	if (id == -1) { return true; }
	return ref[id];
}

void cReference::removeRef(int id) {
	if (id < 0 || id > LIMIT_REFERENCE) { return; }
	ref[id] = false;
}