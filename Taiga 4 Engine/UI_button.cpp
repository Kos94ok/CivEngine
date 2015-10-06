
#include "main.h"
#include "UI.h"
#include "game.h"
#include "craft.h"
#include "script.h"
#include "client.h"
#include "world.h"
#include "editor.h"
#include "target.h"
#include "chat.h"
#include "overworld.h"

void cUIButton::callbackLeft(int parent)
{
	int id, count;
	sf::Packet data;
	if (action == "noAction") {
		console.error << "[ERROR] Button has no action assigned!" << "\n";
	}
	else if (action == "contextMenu_close")
	{
		ui.clearContextMenu();
	}
	else if (action == "unit_addResource")
	{
		ui.clearContextMenu();
		stringstream(args[0]) >> id;
		stringstream(args[1]) >> count;
		data << MSG_CONTROLS_ADDRESOURCE << id << count;
		client.sendPacket(data);
		data.clear();
	}
	else if (action == "unit_pack")
	{
		ui.clearContextMenu();
		stringstream(args[0]) >> id;
		data << MSG_CONTROLS_PACK << id;
		client.sendPacket(data);
		data.clear();
	}
	else if (action == "unit_pickup")
	{
		ui.clearContextMenu();
		stringstream(args[0]) >> id;
		data << MSG_CONTROLS_PICKUP << id << false;
		client.sendPacket(data);
		data.clear();
	}
	else if (action == "unit_harvest")
	{
		ui.clearContextMenu();
		stringstream(args[0]) >> id;
		data << MSG_CONTROLS_HARVEST << id << false;
		client.sendPacket(data);
		data.clear();
	}
	else if (action == "unit_moveToUnit")
	{
		ui.clearContextMenu();
		stringstream(args[0]) >> id;
		cUnit* target = &game.getUnit(id);
		if (target->type != "missingno") {
			data << MSG_CONTROLS_MOVETO << target->pos.x << target->pos.y << false;
			client.sendPacket(data);
			data.clear();
		}
	}
	else if (action == "invItem_list")
	{
		stringstream(args[0]) >> id;
		craft.cont.add(game.unit[game.getUnitId(client.unit)].container.get(id));
		game.unit[game.getUnitId(client.unit)].container.remove(id, 1);
		craft.checkActiveRecipe();
		ui.clearContextMenu();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_craftRemove")
	{
		stringstream(args[0]) >> id;
		game.unit[game.getUnitId(client.unit)].container.add(craft.cont.get(id), 1);
		game.unit[game.getUnitId(client.unit)].container.sort(util.getInventorySortingType());
		craft.cont.remove(id, 1);
		craft.checkActiveRecipe();
		ui.clearContextMenu();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_craftRemoveAll")
	{
		stringstream(args[0]) >> id;
		game.unit[game.getUnitId(client.unit)].container.add(craft.cont.get(id), craft.cont.getAmount(craft.cont.get(id).type));
		game.unit[game.getUnitId(client.unit)].container.sort(util.getInventorySortingType());
		craft.cont.remove(id);
		craft.checkActiveRecipe();
		ui.clearContextMenu();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_craftResult")
	{
		ui.clearContextMenu();
		craft.getResult(false);
	}
	else if (action == "invItem_craftResultAll")
	{
		ui.clearContextMenu();
		craft.getResult(true);
	}
	else if (action == "invItem_craftClear")
	{
		for (int i = 0; i < craft.cont.itemCounter; i++)
		{
			game.unit[game.getUnitId(client.unit)].container.add(craft.cont.item[i], craft.cont.amount[i]);
		}
		game.unit[game.getUnitId(client.unit)].container.sort(util.getInventorySortingType());
		craft.cont.clear();
		craft.checkActiveRecipe();
		ui.clearContextMenu();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_equip")
	{
		stringstream(args[0]) >> id;
		id = game.unit[game.getUnitId(client.unit)].container.getId(id);
		//game.unit[game.getUnitId(client.unit)].container.item[id].equipped = !game.unit[game.getUnitId(client.unit)].container.item[id].equipped;
		game.unit[game.getUnitId(client.unit)].container.item[id].equip();
		ui.clearContextMenu();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_build")
	{
		stringstream(args[0]) >> id;
		cUnit* owner = &game.unit[game.getUnitId(client.unit)];
		target.enable_building(&owner->container.get(id));
		
		ui.clearContextMenu();
		ui.closeInventory();
	}
	else if (action == "invItem_consume")
	{
		stringstream(args[0]) >> id;
		game.useItem(id);
		ui.clearContextMenu();
		ui.updateInterfaceItemList();
	}
	else if (action == "invItem_dismantle")
	{
		stringstream(args[0]) >> id;
		data << MSG_CONTROLS_DISMANTLE << game.getUnit(client.unit).container.get(id).type << 1;
		client.sendPacket(data);
		data.clear();

		ui.clearContextMenu();
	}
	else if (action == "invItem_drop")
	{
		stringstream(args[0]) >> id;
		data << MSG_CONTROLS_DROPITEM << game.getUnit(client.unit).container.get(id).type << 1;
		client.sendPacket(data);
		data.clear();

		ui.clearContextMenu();
	}
	else if (action == "recipe_next")
	{
		craft.selectedRecipe += 1;
		craft.checkActiveRecipe();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "recipe_prev")
	{
		craft.selectedRecipe -= 1;
		craft.checkActiveRecipe();
		ui.updateInterfaceEquipment();
		ui.updateInterfaceItemList();
	}
	else if (action == "chat_scrollUp")
	{
		chat.scroll(1);
	}
	else if (action == "chat_scrollDown")
	{
		chat.scroll(-1);
	}
	else if (action == "start_taigaMaxi")
	{
		world.genNormalWorld();
		overworld.generateMap();
		ui.removeElementsByRef(REF_UI_MAINMENU, 0.50f);
		ui.updateFull();
		client.connect("localserver", 21045);
		script.execute(cScript::shooter_respawnSystem, 0);
		audio.playMusic(MUSIC_DAY);
	}
	else if (action == "start_genericShooter")
	{
		world.genNormalWorld();
		ui.removeElementsByRef(REF_UI_MAINMENU, 0.50f);
		ui.updateFull();
		client.connect("localserver", 21045);

		game.timeOfDay = 24.00f;
		game.ambientLight = 0.00f;
		script.execute(cScript::shooter_spawnItems, 0);
		script.execute(cScript::shooter_spawnEnemies, 0);
		script.execute(cScript::shooter_respawnSystem, 0);
	}
	else if (action == "start_editor")
	{
		editor.enable();
		client.connect("localserver", 21045);
		ui.removeElementsByRef(REF_UI_MAINMENU, 0.50f);
		ui.updateFull();
	}
	else if (action == "connect_temp")
	{
		client.connect("84.50.50.87", 21045);
		ui.removeElementsByRef(REF_UI_MAINMENU, 0.50f);
		ui.updateFull();
	}
	else if (action == "invToggle")
	{
		if (ui.invOpened) { ui.closeInventory(); }
		else { ui.openInventory(); }
	}
	else if (action == "overworldToggle")
	{
		if (ui.wndOverworld.isDisplayed) { ui.wndOverworld.close(); }
		else { ui.wndOverworld.open(); }
	}
	else if (action == "overworldMoveto")
	{
		overworld.moveTo(overworld.selectedMap);
	}
	else if (action.substr(0, 8) == "category")
	{
		if (action.substr(9) == "all") { ui.inventoryCategory = CATEGORY_ALL; }
		if (action.substr(9) == "equipped") { ui.inventoryCategory = CATEGORY_EQUIPPED; }
		if (action.substr(9) == "weapons") { ui.inventoryCategory = CATEGORY_WEAPONS; }
		if (action.substr(9) == "armor") { ui.inventoryCategory = CATEGORY_ARMOR; }
		if (action.substr(9) == "materials") { ui.inventoryCategory = CATEGORY_MATERIALS; }
		if (action.substr(9) == "consumables") { ui.inventoryCategory = CATEGORY_CONSUMABLES; }
		if (action.substr(9) == "other") { ui.inventoryCategory = CATEGORY_OTHER; }
		ui.updateInterfaceItemList();
	}
	else if (action == "activeItem")
	{
		stringstream(args[0]) >> id;
		game.useItem(id);
	}
	else if (action == "selectOverworldMap")
	{
		stringstream(args[0]) >> id;
		overworld.selectMap(id);
	}
	else if (action == "exit")
	{
		core.shutdown = true;
	}
	else
	{
		script.civ_callback(this, parent);
	}
}

void cUIButton::callbackRight(int parent)
{
	int id;
	if (action == "noAction") {
		console.error << "[ERROR] Button has no action assigned!" << "\n";
	}
	else if (action == "button_test") {
		ui.removeElement(ui.getElementId(parent));
	}
	// Context menu
	else if (ui.contextMenuTarget == CONTEXTMENU_NOTHING)
	{
		if (action == "invItem_list")
		{
			stringstream(args[0]) >> id;
			ui.createContextMenuForItem(id);
		}
		else if (action == "invItem_craftRemove")
		{
			stringstream(args[0]) >> id;
			ui.createContextMenuForCraftItem(id);
		}
		else if (action == "invItem_craftResult")
		{
			stringstream(args[0]) >> id;
			ui.createContextMenuForCraftResult(id);
		}
	}
}