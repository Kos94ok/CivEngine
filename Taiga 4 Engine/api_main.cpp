
#include "main.h"
#include "api.h"
#include "database.h"
#include "client.h"

cUnit& cAPI::unitHandle(int id, bool isGlobal)
{
	// Global id
	if (isGlobal)
	{
		for (int i = 0; i < game.unitCounter; i++)
		{
			if (game.unit[i].globalId == id) { return game.unit[i]; }
		}
	}
	// Local id
	else
	{
		if (game.unitCounter < id) { return game.unit[id]; }
	}
	// Nothing found
	return database.unit[0];
}
cUnit cAPI::unitCopy(int id, bool isGlobal) { return unitHandle(id, isGlobal); }
cUnit& cAPI::lastCreatedUnit() { return game.unit[game.unitCounter - 1]; }
cUnit& cAPI::controlledUnit() { return game.getUnit(client.unit); }
void cAPI::issueMoveOrder(sf::Vector2f pos, int id, bool overwrite, bool isGlobal) { unitHandle(id, isGlobal).addOrder_moveto_path(pos, overwrite); }
void cAPI::issueStopOrder(int id, bool isGlobal) { unitHandle(id, isGlobal).removeAllOrders(); }

cUIElement& cAPI::elementHandle(int id)
{
	if (ui.element[id].isValid) { return ui.element[id]; }
	return database.uiElement[0];
}
cUIElement cAPI::elementCopy(int id) { return elementHandle(id); }
cUIElement& cAPI::lastCreatedElement() { return ui.element[ui.lastCreatedElement]; }