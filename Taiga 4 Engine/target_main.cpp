
#include "target.h"
#include "UI.h"
#include "game.h"
#include "camera.h"
#include "script.h"
#include "client.h"
#include "window.h"
#include "path.h"
#include "math.h"
#include "console.h"
#include "visual.h"

void cTarget::enable_forButton(int elementId)
{
	reset();
	mode = MODE_TARGET_BUTTON;
	// Checking the argument
	int id = ui.getElementId(elementId);
	string type;
	if (id == -1) { console.error << "[cTarget::enable_forButton] Incorrect element id!" << endl; return; }

	// Activating
	if (!active) {
		active = true;
	}
	visual.setMousePointer(POINTER_TARGET);
	forButton = elementId;
}

void cTarget::enable_building(int ref)
{
	reset();
	string type = util.buildRefToType(ref);
	if (type == "missingno") { console.error << "[cTarget::enable_building] Incorrect item reference!" << endl; return; }
	mode = MODE_TARGET_BUILDING;
	buildRef = ref;
	// Adding the new unit
	game.access.lock();
	game.addUnit(type, vec2f(0.00f, 0.00f), -1, -1, false, ID_LOCAL);
	game.getLocalUnit().addRef(REF_UNIT_TARGET);
	game.getLocalUnit().addRef(REF_UNIT_NOSHADOW);
	game.getLocalUnit().addRef(REF_UNIT_NOSELECTION);
	game.getLocalUnit().addRef(REF_UNIT_PLACEHOLDER);
	game.getLocalUnit().addRef(REF_UNIT_NOCOLLISION);
	game.getLocalUnit().removeRef(REF_UNIT_BURNOUT_CAMPFIRE);
	game.getLocalUnit().light.power = 0.00f;
	game.access.unlock();

	// Activating the script
	if (!activeBuild) {
		script.execute(cScript::unit_bindToMouse, to_string(REF_UNIT_TARGET));
	}
	active = true;
	activeBuild = true;
}

void cTarget::enable_building(cItem* item)
{
	if (item->hasRef(REF_ITEM_BUILD_CAMPFIRE)) {
		enable_building(REF_ITEM_BUILD_CAMPFIRE);
	}
	else if (item->hasRef(REF_ITEM_BUILD_FURNACEBASIC)) {
		enable_building(REF_ITEM_BUILD_FURNACEBASIC);
	}
	else if (item->hasRef(REF_ITEM_BUILD_FURNACEIND)) {
		enable_building(REF_ITEM_BUILD_FURNACEIND);
	}
	else if (item->hasRef(REF_ITEM_BUILD_TENT)) {
		enable_building(REF_ITEM_BUILD_TENT);
	}
}

void cTarget::apply()
{
	sf::Packet data;
	vec2f mousePos = window.getMousePos(true);
	if (mode == MODE_TARGET_BUTTON)
	{
		int id = ui.getElementId(forButton);
		if (id != -1) {
			ui.element[id].button.callbackLeft(forButton);
		}
		reset();
	}
	else if (mode == MODE_TARGET_BUILDING && isBuildGood)
	{
		data << MSG_CONTROLS_BUILD << buildRef << mousePos.x << mousePos.y;
		client.sendPacket(data);
		data.clear();
		reset();
	}
}

void cTarget::reset()
{
	mode = MODE_NONE;
	active = false;
	activeBuild = false;

	cUnit* oldUnit = &game.getUnitByRef(REF_UNIT_TARGET);
	if (oldUnit->type != "missingno") {
		game.removeUnit(oldUnit->globalId, false);
	}
	visual.setMousePointer(POINTER_DEFAULT);
}

void cTarget::updateBuildStatus(int unitId)
{
	cUnit* targetUnit = &game.getUnit(unitId);
	if (targetUnit->type == "missingno") {
		console.error << "[cTarget::updateBuildStatus] Incorrect unit id! [" << unitId << "]" << endl;
		return;
	}
	isBuildGood = (math.getDistance(targetUnit->pos, game.getUnit(client.unit).pos) < value.maxBuildDist
		&& path.isPointFree(targetUnit->pos, targetUnit->collisionDistance, targetUnit->globalId));
}