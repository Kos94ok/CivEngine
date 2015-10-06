
#include "main.h"
#include "UI.h"
#include "game.h"
#include "craft.h"
#include "window.h"
#include "client.h"
#include "visual.h"
#include "camera.h"

void cUI::createContextMenuForItem(int targetItem)
{
	access.lock();
	contextMenuTarget = CONTEXTMENU_ITEM;
	contextMenuTargetId = targetItem;

	sf::Vector2f mousePos = window.getMousePos();

	int buttonCount = 0;
	cItem item = game.unit[game.getUnitId(client.unit)].container.get(targetItem);

	// Background
	int bgId = ui.addElement("image", mousePos);
	ui.element[ui.getElementId(bgId)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(bgId)].alpha = 200;
	ui.element[ui.getElementId(bgId)].size = sf::Vector2f(138.00f, 10.00f);
	ui.element[ui.getElementId(bgId)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(bgId)].ignoreOrigin = true;
	ui.element[ui.getElementId(bgId)].priority = 9;

	// Add to craft
	int id = ui.addElement("button", mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].setText("Add to craft");
	ui.element[ui.getElementId(id)].textOffset.x = 3.00f;
	ui.element[ui.getElementId(id)].textOffset.y = 5.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 255;
	ui.element[ui.getElementId(id)].textSize = 20;
	ui.element[ui.getElementId(id)].textColor = sf::Color(175, 175, 175);
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].button.action = "invItem_list";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	ui.element[ui.getElementId(id)].priority = 10;
	if (item.hasRef(REF_ITEM_NOCRAFT))
	{
		buttonCount -= 1;
		element[getElementId(id)].pos.x = -1000.00f;
	}
	// Equip
	if (item.hasRef(REF_ITEM_EQUIP))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Equip");
		if (item.equipped) { ui.element[ui.getElementId(id)].setText("Unequip"); }
		ui.element[ui.getElementId(id)].button.action = "invItem_equip";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	}
	// Build
	if (item.hasRef(REF_ITEM_BUILD))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Build");
		ui.element[ui.getElementId(id)].button.action = "invItem_build";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	}
	// Consume
	if (item.hasRef(REF_ITEM_CONSUME))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Consume");
		ui.element[ui.getElementId(id)].button.action = "invItem_consume";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	}
	// Dismantle / Unpack / Empty
	if (item.dismantle.itemCounter > 0)
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Dismantle");
		if (item.hasRef(REF_ITEM_EMPTIABLE)) { ui.element[ui.getElementId(id)].setText("Empty"); }
		if (item.hasRef(REF_ITEM_UNPACKABLE)) { ui.element[ui.getElementId(id)].setText("Unpack"); }
		ui.element[ui.getElementId(id)].button.action = "invItem_dismantle";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	}
	// Drop
	if (!item.hasRef(REF_ITEM_NODROP))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Drop");
		ui.element[ui.getElementId(id)].button.action = "invItem_drop";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	}

	// Size
	ui.element[ui.getElementId(bgId)].size.y += buttonCount * 32.00f;
	// Border
	ui.createBorder(ui.element[ui.getElementId(bgId)].pos, ui.element[ui.getElementId(bgId)].pos + ui.element[ui.getElementId(bgId)].size,
		REF_UI_CONTEXTMENU, 11);

	access.unlock();
}

void cUI::createContextMenuForCraftItem(int targetItem)
{
	access.lock();
	contextMenuTarget = CONTEXTMENU_ITEM;
	contextMenuTargetId = targetItem;

	sf::Vector2f mousePos = window.getMousePos();

	int buttonCount = 0;
	cItem item = craft.cont.get(targetItem);

	// Background
	int bgId = ui.addElement("image", mousePos);
	ui.element[ui.getElementId(bgId)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(bgId)].alpha = 200;
	ui.element[ui.getElementId(bgId)].size = sf::Vector2f(138.00f, 10.00f);
	ui.element[ui.getElementId(bgId)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(bgId)].ignoreOrigin = true;
	ui.element[ui.getElementId(bgId)].priority = 9;

	// Remove
	int id = ui.addElement("button", mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].setText("Remove");
	ui.element[ui.getElementId(id)].textOffset.x = 3.00f;
	ui.element[ui.getElementId(id)].textOffset.y = 5.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 255;
	ui.element[ui.getElementId(id)].textSize = 20;
	ui.element[ui.getElementId(id)].textColor = sf::Color(175, 175, 175);
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].button.action = "invItem_craftRemove";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	ui.element[ui.getElementId(id)].priority = 10;
	// Remove all
	id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].setText("Remove all");
	ui.element[ui.getElementId(id)].button.action = "invItem_craftRemoveAll";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	// Clear table
	id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].setText("Clear table");
	ui.element[ui.getElementId(id)].button.action = "invItem_craftClear";

	// Size
	ui.element[ui.getElementId(bgId)].size.y += buttonCount * 32.00f;
	// Border
	ui.createBorder(ui.element[ui.getElementId(bgId)].pos, ui.element[ui.getElementId(bgId)].pos + ui.element[ui.getElementId(bgId)].size,
		REF_UI_CONTEXTMENU, 11);

	access.unlock();
}

void cUI::createContextMenuForCraftResult(int targetItem)
{
	access.lock();
	contextMenuTarget = CONTEXTMENU_ITEM;
	contextMenuTargetId = targetItem;

	sf::Vector2f mousePos = window.getMousePos();

	int buttonCount = 0;
	cItem item = craft.resultCont.get(targetItem);

	float offsetY = 0.00f;
	if (mousePos.y > camera.res.y - 106) { offsetY = -106.00f; }

	// Background
	int bgId = ui.addElement("image", mousePos + sf::Vector2f(0.00f, offsetY));
	ui.element[ui.getElementId(bgId)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(bgId)].alpha = 200;
	ui.element[ui.getElementId(bgId)].size = sf::Vector2f(138.00f, 10.00f);
	ui.element[ui.getElementId(bgId)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(bgId)].ignoreOrigin = true;
	ui.element[ui.getElementId(bgId)].priority = 9;

	// Craft
	int id = ui.addElement("button", mousePos + sf::Vector2f(5.00f, 5.00f + offsetY + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].setText("Craft");
	ui.element[ui.getElementId(id)].textOffset.x = 3.00f;
	ui.element[ui.getElementId(id)].textOffset.y = 5.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 255;
	ui.element[ui.getElementId(id)].textSize = 20;
	ui.element[ui.getElementId(id)].textColor = sf::Color(175, 175, 175);
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].button.action = "invItem_craftResult";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	ui.element[ui.getElementId(id)].priority = 10;
	// Craft all
	id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + offsetY + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].setText("Craft all");
	ui.element[ui.getElementId(id)].button.action = "invItem_craftResultAll";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetItem);
	// Drop
	id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + offsetY + 32.00f * buttonCount++));
	ui.element[ui.getElementId(id)].setText("Clear table");
	ui.element[ui.getElementId(id)].button.action = "invItem_craftClear";

	// Size
	ui.element[ui.getElementId(bgId)].size.y += buttonCount * 32.00f;
	// Border
	ui.createBorder(ui.element[ui.getElementId(bgId)].pos, ui.element[ui.getElementId(bgId)].pos + ui.element[ui.getElementId(bgId)].size,
		REF_UI_CONTEXTMENU, 11);

	access.unlock();
}

void cUI::createContextMenuForGround()
{
	access.lock();
	contextMenuTarget = CONTEXTMENU_GROUND;

	sf::Vector2f mousePos = window.getMousePos();

	int buttonCount = 0;
	float buttonY = 20.00f;
	float buttonDist = 2.00f;

	// Background
	int bgId = ui.addElement("image", mousePos);
	ui.element[ui.getElementId(bgId)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(bgId)].alpha = 200;
	ui.element[ui.getElementId(bgId)].size = sf::Vector2f(138.00f, 10.00f);
	ui.element[ui.getElementId(bgId)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(bgId)].ignoreOrigin = true;
	ui.element[ui.getElementId(bgId)].priority = 9;

	// Close
	int id = ui.addElement("button", mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
	ui.element[ui.getElementId(id)].size.y = buttonY;
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].setText("No actions");
	ui.element[ui.getElementId(id)].textOffset.x = 3.00f;
	ui.element[ui.getElementId(id)].textOffset.y = 0.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 255;
	ui.element[ui.getElementId(id)].textSize = 16;
	ui.element[ui.getElementId(id)].textColor = sf::Color(175, 175, 175);
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].button.action = "contextMenu_close";
	ui.element[ui.getElementId(id)].priority = 10;

	// More buttons
	//id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));

	// Size
	ui.element[ui.getElementId(bgId)].size.y += (buttonY + buttonDist) * buttonCount;
	// Border
	ui.createBorder(ui.element[ui.getElementId(bgId)].pos, ui.element[ui.getElementId(bgId)].pos + ui.element[ui.getElementId(bgId)].size,
		REF_UI_CONTEXTMENU, 11);

	access.unlock();
}

void cUI::createContextMenuForUnit(int targetUnit)
{
	access.lock();

	contextMenuTarget = CONTEXTMENU_UNIT;
	contextMenuTargetId = targetUnit;

	sf::Vector2f mousePos = window.getMousePos();

	int buttonCount = 0;
	float buttonY = 20.00f;
	float buttonDist = 2.00f;
	cItem item = craft.cont.get(targetUnit);

	// Background
	int bgId = ui.addElement("image", mousePos);
	ui.element[ui.getElementId(bgId)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(bgId)].alpha = 200;
	ui.element[ui.getElementId(bgId)].size = sf::Vector2f(138.00f, 10.00f);
	ui.element[ui.getElementId(bgId)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(bgId)].ignoreOrigin = true;
	ui.element[ui.getElementId(bgId)].priority = 9;

	// Dummy button
	int id = ui.addElement("button", mousePos + sf::Vector2f(-5000.00f, -5000.00f));
	ui.element[ui.getElementId(id)].size.y = buttonY;
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].setText("missingno");
	ui.element[ui.getElementId(id)].textOffset.x = 3.00f;
	ui.element[ui.getElementId(id)].textOffset.y = 0.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_CONTEXTMENU);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 255;
	ui.element[ui.getElementId(id)].textSize = 16;
	ui.element[ui.getElementId(id)].textColor = sf::Color(175, 175, 175);
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].priority = 10;
	// Add resource
	if (game.getUnit(targetUnit).hasRef(REF_UNIT_ADDRESOURCE))
	{
		if (game.getUnit(client.unit).resource > 50)
		{
			id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
			ui.element[ui.getElementId(id)].setText("Add Resource");
			ui.element[ui.getElementId(id)].button.action = "unit_addResource";
			ui.element[ui.getElementId(id)].button.args[0] = to_string(targetUnit);
			ui.element[ui.getElementId(id)].button.args[1] = to_string(50);
		}
	}
	// Pack
	if (game.getUnit(targetUnit).hasRef(REF_UNIT_PACK))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Pack");
		ui.element[ui.getElementId(id)].button.action = "unit_pack";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetUnit);
	}
	// Pick up
	if (game.getUnit(targetUnit).hasRef(REF_UNIT_PICKUP))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Pick up");
		ui.element[ui.getElementId(id)].button.action = "unit_pickup";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetUnit);
	}
	// Harvest
	if (game.getUnit(targetUnit).hasRef(REF_UNIT_HARVESTABLE))
	{
		id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
		ui.element[ui.getElementId(id)].setText("Harvest");
		ui.element[ui.getElementId(id)].button.action = "unit_harvest";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(targetUnit);
	}
	// Move to
	id = ui.addElement(ui.element[ui.getElementId(id)], mousePos + sf::Vector2f(5.00f, 5.00f + (buttonY + buttonDist) * buttonCount++));
	ui.element[ui.getElementId(id)].setText("Move to");
	ui.element[ui.getElementId(id)].button.action = "unit_moveToUnit";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(targetUnit);

	// Size
	ui.element[ui.getElementId(bgId)].size.y += buttonCount * (buttonY + buttonDist);
	// Border
	ui.createBorder(ui.element[ui.getElementId(bgId)].pos, ui.element[ui.getElementId(bgId)].pos + ui.element[ui.getElementId(bgId)].size,
		REF_UI_CONTEXTMENU, 11);

	access.unlock();
}

void cUI::clearContextMenu()
{
	ui.removeElementsByRef(REF_UI_CONTEXTMENU);
	contextMenuTarget = CONTEXTMENU_NOTHING;
}