
#include "main.h"
#include "item.h"
#include "client.h"
#include "window.h"
#include "game.h"
#include "visual.h"
#include "database.h"
#include "client.h"
#include "math.h"

void cItem::use()
{
	sf::Packet data;
	if (!hasRef(REF_ITEM_ACTIVE) && !(hasRef(REF_ITEM_CONSUME))) { return; }
	// Remove all orders
	data << MSG_CONTROLS_STOP;
	client.sendPacket(data);
	data.clear();

	// Blink
	if (hasRef(REF_ITEM_BLINK))
	{
		data << MSG_CONTROLS_BLINK << window.getMousePos(true).x << window.getMousePos(true).y;
		client.sendPacket(data);
		data.clear();
	}
	// Rifle
	if (hasRef(REF_ITEM_RIFLE))
	{
		data << MSG_CONTROLS_RIFLE << window.getMousePos(true).x << window.getMousePos(true).y;
		client.sendPacket(data);
		data.clear();
		audio.playSound("rifle_shot", game.getPlayer().pos);
	}
	// Axe
	if (hasRef(REF_ITEM_AXE))
	{
		if (visual.hoveredUnit != -1 && game.getUnit(visual.hoveredUnit).hasRef(REF_UNIT_TREE))
		{
			data << MSG_CONTROLS_AXE << visual.hoveredUnit << powerLevel;
			client.sendPacket(data);
			data.clear();
		}
	}
	// Pickaxe
	if (hasRef(REF_ITEM_PICKAXE))
	{
		if (visual.hoveredUnit != -1 && game.getUnit(visual.hoveredUnit).hasRef(REF_UNIT_STONE))
		{
			data << MSG_CONTROLS_PICKAXE << visual.hoveredUnit << powerLevel;
			client.sendPacket(data);
			data.clear();
		}
	}
	// Consumable
	if (hasRef(REF_ITEM_CONSUME))
	{
		data << MSG_CONTROLS_CONSUME << type << 1;
		client.sendPacket(data);
		data.clear();
	}
	// Editor items
	if (hasRef(REF_ITEM_EDITOR))
	{
		int variation = -1;
		if (hasRef(REF_ITEM_EDITOR_VAR2)) { variation = math.rand(0, 1); }
		if (hasRef(REF_ITEM_EDITOR_VAR3)) { variation = math.rand(0, 2); }
		if (hasRef(REF_ITEM_EDITOR_VAR4)) { variation = math.rand(0, 3); }
		if (hasRef(REF_ITEM_EDITOR_VAR5)) { variation = math.rand(0, 4); }
		if (hasRef(REF_ITEM_EDITOR_VAR6)) { variation = math.rand(0, 5); }
		if (hasRef(REF_ITEM_EDITOR_VAR7)) { variation = math.rand(0, 6); }
		if (hasRef(REF_ITEM_EDITOR_VAR8)) { variation = math.rand(0, 7); }
		game.addUnit(type, window.getMousePos(true), -1, variation);
	}
}

void cItem::equip()
{
	sf::Packet data;
	if (!hasRef(REF_ITEM_EQUIP)) { return; }

	equipped = !equipped;
	// Flashlight
	if (hasRef(REF_ITEM_FLASHLIGHT) && equipped)
	{
		data << MSG_CONTROLS_FLASHLIGHT_ON;
		client.sendPacket(data);
		data.clear();
	}
	else if (hasRef(REF_ITEM_FLASHLIGHT) && !equipped)
	{
		data << MSG_CONTROLS_FLASHLIGHT_OFF;
		client.sendPacket(data);
		data.clear();
	}
	// Torch
	if (hasRef(REF_ITEM_TORCH) && equipped)
	{
		data << MSG_CONTROLS_TORCH_ON;
		client.sendPacket(data);
		data.clear();
	}
	else if (hasRef(REF_ITEM_TORCH) && !equipped)
	{
		data << MSG_CONTROLS_TORCH_OFF;
		client.sendPacket(data);
		data.clear();
	}
}