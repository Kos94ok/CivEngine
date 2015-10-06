
#include "main.h"
#include "editor.h"
#include "game.h"
#include "client.h"
#include "UI.h"

void cEditor::enable()
{
	console.echo << "[CMD] Enabled editor mode." << "\n";
	core.editorMode = true;
}

void cEditor::initialize()
{
	cUnit* clientUnit = &game.getUnit(client.unit);
	cItem* targetItem;
	if (clientUnit->type != "missingno")
	{
		clientUnit->container.add("tree_basic_");
		targetItem = &clientUnit->container.get("tree_basic_");
		targetItem->equipped = true;
		clientUnit->container.add("tree_fir_");
		targetItem = &clientUnit->container.get("tree_fir_");
		targetItem->equipped = true;
		clientUnit->container.add("stone_basic_");
		targetItem = &clientUnit->container.get("stone_basic_");
		targetItem->equipped = true;
		clientUnit->container.add("campfire_full");
		targetItem = &clientUnit->container.get("campfire_full");
		targetItem->equipped = true;
	}
	ui.updateInterfaceEquipment();
	ui.updateInterfaceItemList();
	ui.updateInterfaceItemDescr();
}