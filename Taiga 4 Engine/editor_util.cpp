
#include "main.h"
#include "editor.h"
#include "game.h"

void cEditorSelection::add(int target)
{
	bool isFound = false;
	for (int i = 0; i < count; i++) { if (i == target) { isFound = true; } }

	if (!isFound) { unit[count++] = target; }
}

void cEditorSelection::remove(int target)
{
	int id = -1;
	for (int i = 0; i < count; i++)
	{
		if (target == unit[i]) { id = i; }
	}
	if (id != -1)
	{
		for (int i = id; i < count - 1; i++)
		{
			unit[i] = unit[i + 1];
		}
		count -= 1;
	}
}

void cEditorSelection::clear()
{
	count = 0;
}

bool cEditorSelection::isSelected(int target)
{
	for (int i = 0; i < count; i++)
	{
		if (target == unit[i]) { return true; }
	}
	return false;
}

void cEditorSelection::destroySelected()
{
	for (int i = 0; i < count; i++)
	{
		game.removeUnit(unit[i]);
	}
	clear();
}