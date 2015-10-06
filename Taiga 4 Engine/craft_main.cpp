
#include "main.h"
#include "craft.h"
#include "database.h"
#include "game.h"
#include "client.h"

typedef cComponent item;

void cCraft::addRecipe(cComponent result, int resBalance, cComponent ingrA,
				cComponent ingrB, cComponent ingrC, cComponent ingrD, cComponent ingrE,
				cComponent ingrF, cComponent ingrG, cComponent ingrH, cComponent ingrI)
{
	int ingrCount = 1;
	recipe[recipeCounter].result = result;
	if (database.isItemGood(ingrA.type)) recipe[recipeCounter].ingr[0] = ingrA;
	if (ingrB.type != "") { if (database.isItemGood(ingrB.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrB; }
	if (ingrC.type != "") { if (database.isItemGood(ingrC.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrC; }
	if (ingrD.type != "") { if (database.isItemGood(ingrD.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrD; }
	if (ingrE.type != "") { if (database.isItemGood(ingrE.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrE; }
	if (ingrF.type != "") { if (database.isItemGood(ingrF.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrF; }
	if (ingrG.type != "") { if (database.isItemGood(ingrG.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrG; }
	if (ingrH.type != "") { if (database.isItemGood(ingrH.type)) recipe[recipeCounter].ingr[ingrCount++] = ingrH; }
	recipe[recipeCounter].ingrCount = ingrCount;
	recipe[recipeCounter].resourceBalance = resBalance;
	recipeCounter += 1;
}

int cCraft::getActiveRecipeId()
{
	int matchFound = 0;
	int id = 0, amount = 0;
	bool match = true;
	if (cont.itemCounter == 0) { return -1; }
	for (int i = 0; i < recipeCounter; i++)
	{
		match = true;
		for (int y = 0; y < cont.itemCounter; y++)
		{
			if (!isItemInRecipe(i, cont.item[y].type))
			{
				match = false;
				y = cont.itemCounter;
			}
		}
		if (match && selectedRecipe == matchFound) { return i; }
		else if (match) { matchFound += 1; }
	}
	return -1;
}

int cCraft::getActiveRecipeRepeats(bool ignoreResource)
{
	bool match = true;
	int i = getActiveRecipeId();
	if (i == -1) { return 0; }
	int amount = 100;
	// Amount of ingridients
	for (int y = 0; y < recipe[i].ingrCount; y++)
	{
		amount = min(amount, craft.cont.getAmount(recipe[i].ingr[y].type) / max(recipe[i].ingr[y].count, 1));
	}
	// Amount of resource
	if (recipe[i].resourceBalance < 0.00f && !ignoreResource)
	{
		amount = min((float)amount, floor(game.getUnit(client.unit).resource / -recipe[i].resourceBalance));
	}
	return amount;
}

int cCraft::getActiveRecipeCount()
{
	int matchFound = 0;
	int id = 0, amount = 0;
	bool match = true;
	if (cont.itemCounter == 0) { return 0; }
	for (int i = 0; i < recipeCounter; i++)
	{
		match = true;
		for (int y = 0; y < cont.itemCounter; y++)
		{
			if (!isItemInRecipe(i, cont.item[y].type))
			{
				match = false;
				y = cont.itemCounter;
			}
		}
		if (match) { matchFound += 1; }
	}
	return matchFound;
}

void cCraft::checkActiveRecipe()
{
	bool match = true;
	int matchId = -1, id = -1;
	if (cont.itemCounter == 0) {
		resultCont.clear();
		return;
	}

	// Checking recipe index
	int recCount = getActiveRecipeCount();
	if (selectedRecipe < 0) { selectedRecipe = recCount - 1; }
	if (selectedRecipe >= recCount) { selectedRecipe = 0; }

	// Searching for the active recipe
	matchId = getActiveRecipeId();
	// If recipe found
	if (matchId != -1)
	{
		int rpt = getActiveRecipeRepeats();
		// Adding the recipe result
		resultCont.clear();
		resultCont.add(recipe[matchId].result.type, recipe[matchId].result.count * rpt);
	}
	// No recipe found
	else
	{
		resultCont.clear();
	}
}

void cCraft::getResult(bool craftAll)
{
	int rec = getActiveRecipeId();
	if (rec == -1 || getActiveRecipeRepeats() <= 0) { return; }
	int rpt = 1;
	if (craftAll) { rpt = getActiveRecipeRepeats(); }

	// Add crafted item
	//game.unit[game.getUnitId(client.unit)].container.add(craft.resultCont.item[0], craft.recipe[rec].result.count * rpt);

	// Remove stuff
	for (int i = 0; i < recipe[rec].ingrCount; i++)
	{
		// Only if item should be consumed
		if (recipe[rec].ingr[i].consume) { cont.remove(recipe[rec].ingr[i].type, craft.recipe[rec].ingr[i].count * rpt); }
	}
	// Temporary remove the resource
	game.unit[game.getUnitId(client.unit)].resource -= -craft.recipe[rec].resourceBalance * rpt;

	// Update amounts
	checkActiveRecipe();

	// Update UI
	//ui.updateInterfaceItemList();

	// Add ingridients back to inventory (will be removed by server command)
	game.unit[game.getUnitId(client.unit)].resource += -craft.recipe[rec].resourceBalance * rpt;
	for (int i = 0; i < recipe[rec].ingrCount; i++)
	{
		game.unit[game.getUnitId(client.unit)].container.add(recipe[rec].ingr[i].type, craft.recipe[rec].ingr[i].count * rpt);
	}

	// Data to server
	sf::Packet data;
	data << MSG_CONTROLS_CRAFTITEM << rec << rpt;
	client.sendPacket(data);
}

void cCraft::updateCraftAmounts()
{
	return;
}

bool cCraft::isItemInRecipe(int recId, string itemType)
{
	for (int i = 0; i < recipe[recId].ingrCount; i++)
	{
		if (recipe[recId].ingr[i].type == itemType) {
			return true;
		}
	}
	return false;
}

void cCraft::clearGhostItems()
{
	for (int i = 0; i < cont.itemCounter; i++)
	{
		if (cont.item[i].hasRef(REF_ITEM_GHOST))
		{
			cont.remove(i);
			i -= 1;
		}
	}
}

void cCraft::loadRecipes()
{
	addRecipe("voodoo_doll", 0, item("human_leg", 1), item("human_arm", 1), item("human_ear", 1), item("human_eye"));
	addRecipe("test_catAll", 0, "test_catWeapons", "test_catArmor", "test_catMaterials", "test_catConsumables", "test_catOther");
	addRecipe("test_resourceManipulator", 100, "test_resourceManipulator");
	addRecipe("campfire_basic", -100, item("weapon_knife_steel", true), "material_flint");

	// Materials
	addRecipe("material_charcoal", 0, item("material_wood", 2), item("furnace_basic", true));
	addRecipe("material_charcoal", 0, "material_wood", item("furnace_industrial", true));
	addRecipe("material_iron_cast", 0, item("material_iron_raw", 2), "material_charcoal", item("furnace_basic", true));
	addRecipe("material_iron_cast", 0, item("material_iron_raw", 2), "material_charcoal", item("furnace_industrial", true));
	addRecipe("material_steel", 0, item("material_iron_raw", 3), item("material_charcoal", 4), item("furnace_industrial", true));
	addRecipe("material_stone_construction", 50, item("material_stone", 1), item("tool_pickaxe_flint", true));
	addRecipe("material_stone_construction", 50, item("material_stone", 1), item("tool_pickaxe_iron", true));

	// Tools
	addRecipe("tool_axe_stone", -100, item("weapon_knife_steel", true));
	addRecipe("tool_pickaxe_stone", -100, item("weapon_knife_steel", true));

	addRecipe("tool_axe_flint", -200, item("material_flint", 3), item("material_wood", 1));
	addRecipe("tool_pickaxe_flint", -200, item("material_flint", 3), item("material_wood", 1));

	addRecipe("tool_axe_iron", -300, item("material_iron_cast", 3), item("material_wood", 1), "material_charcoal", item("furnace_basic", true));
	addRecipe("tool_axe_iron", -300, item("material_iron_cast", 3), item("material_wood", 1), "material_charcoal", item("furnace_industrial", true));
	addRecipe("tool_pickaxe_iron", -300, item("material_iron_cast", 3), item("material_wood", 1), "material_charcoal", item("furnace_basic", true));
	addRecipe("tool_pickaxe_iron", -300, item("material_iron_cast", 3), item("material_wood", 1), "material_charcoal", item("furnace_industrial", true));

	addRecipe("tool_axe_steel", -400, item("material_steel", 3), item("material_wood", 1), "material_charcoal", item("furnace_industrial", true));
	addRecipe("tool_pickaxe_steel", -400, item("material_steel", 3), item("material_wood", 1), "material_charcoal", item("furnace_industrial", true));

	// Furnace
	addRecipe("furnace_basic", -1000, item("material_stone_construction", 10));
	addRecipe("furnace_industrial", -1000, item("material_stone_construction", 10), item("material_iron_cast", 10));
}