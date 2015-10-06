
#include "main.h"
#include "database.h"
#include "visual.h"

void cDatabase::loadItems()
{
	int i = 0;
	item[i].type = "missingno";
	item[i].setName("missingno");
	item[i].addRef(REF_ITEM_NOCRAFT);
	i += 1;

	// Weapons
	// Hunting rifle
	item[i].type = "weapon_rifle";
	item[i].setName("Hunting Rifle");
	item[i].addDescrLine("Этот текст все равно потом поменяется.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_RIFLE);
	item[i].icon = visual.createIcon("icons_a.png", "icon_rifle", vec2i(7, 0), vec2f(0.125f, 0.125f));
	i += 1;

	// Steel knife
	item[i].type = "weapon_knife_steel";
	item[i].setName("Steel Knife");
	item[i].addDescrLine("Лень что-то писать :(");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	i += 1;

	// Tools
	// Flashlight
	item[i].type = "tool_flashlight";
	item[i].setName("Flashlight");
	item[i].addDescrLine("Just a cool flashlight from Starbound");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_FLASHLIGHT);
	i += 1;

	// Torch
	item[i].type = "tool_torch";
	item[i].setName("Torch");
	item[i].addDescrLine("Some torch action?");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_TORCH);
	i += 1;

	// Axes
	// Stone
	item[i].type = "tool_axe_stone";
	item[i].setName("Stone Axe");
	item[i].addDescrLine("Used to chop some wood.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].powerLevel = POWER_STONE;
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_AXE);
	item[i].icon = visual.createIcon("icons_b.png", "icon_axe_stone", vec2i(1, 0), vec2f(0.125f, 0.125f));
	i += 1;
	// Flint
	item[i] = item[i - 1];
	item[i].type = "tool_axe_flint";
	item[i].setName("Flint Axe");
	item[i].powerLevel = POWER_FLINT;
	item[i].icon = visual.createIcon("icons_b.png", "icon_axe_flint", vec2i(1, 1), vec2f(0.125f, 0.125f));
	i += 1;
	// Iron
	item[i] = item[i - 1];
	item[i].type = "tool_axe_iron";
	item[i].setName("Iron Axe");
	item[i].powerLevel = POWER_IRON;
	item[i].icon = visual.createIcon("icons_b.png", "icon_axe_iron", vec2i(1, 2), vec2f(0.125f, 0.125f));
	i += 1;
	// Steel
	item[i] = item[i - 1];
	item[i].type = "tool_axe_steel";
	item[i].setName("Steel Axe");
	item[i].powerLevel = POWER_STEEL;
	item[i].icon = visual.createIcon("icons_b.png", "icon_axe_iron", vec2i(1, 2), vec2f(0.125f, 0.125f));
	i += 1;

	// Pickaxes
	// Stone
	item[i].type = "tool_pickaxe_stone";
	item[i].setName("Stone Pickaxe");
	item[i].addDescrLine("Used to pick some stone.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].powerLevel = POWER_STONE;
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_PICKAXE);
	item[i].icon = visual.createIcon("icons_b.png", "icon_pickaxe_stone", vec2i(0, 0), vec2f(0.125f, 0.125f));
	i += 1;
	// Flint
	item[i] = item[i - 1];
	item[i].type = "tool_pickaxe_flint";
	item[i].setName("Flint Pickaxe");
	item[i].powerLevel = POWER_FLINT;
	item[i].icon = visual.createIcon("icons_b.png", "icon_pickaxe_flint", vec2i(0, 1), vec2f(0.125f, 0.125f));
	i += 1;
	// Iron
	item[i] = item[i - 1];
	item[i].type = "tool_pickaxe_iron";
	item[i].setName("Iron Pickaxe");
	item[i].powerLevel = POWER_IRON;
	item[i].icon = visual.createIcon("icons_b.png", "icon_pickaxe_iron", vec2i(0, 2), vec2f(0.125f, 0.125f));
	i += 1;
	// Steel
	item[i] = item[i - 1];
	item[i].type = "tool_pickaxe_steel";
	item[i].setName("Steel Pickaxe");
	item[i].powerLevel = POWER_STEEL;
	item[i].icon = visual.createIcon("icons_b.png", "icon_pickaxe_iron", vec2i(0, 2), vec2f(0.125f, 0.125f));
	i += 1;

	// Food
	// Canned meat
	item[i].type = "food_meat_canned";
	item[i].setName("Canned Meat");
	item[i].addDescrLine("Some meat in the can. Good to win some time");
	item[i].addDescrLine("against hunger.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Restores 25 health");
	item[i].addDescrLine("- Restores 42 hunger");
	item[i].addDescrLine("- Consumable");
	item[i].dismantle.add("material_tin", 1);
	item[i].consume.healthBalance = 25;
	item[i].consume.buff.add(BUFF_FOOD, 30.00f, 7);
	item[i].consume.buff.add(BUFF_FOOD, 20.00f, 7);
	item[i].consume.buff.add(BUFF_FOOD, 10.00f, 7);
	item[i].addRef(REF_ITEM_CONSUME);
	item[i].addRef(REF_ITEM_EMPTIABLE);
	item[i].addRef(REF_ITEM_DISMANTLE_ON_CONSUME);
	item[i].category = CATEGORY_CONSUMABLES;
	i += 1;

	// Bottle of water
	item[i].type = "food_water_bottle";
	item[i].setName("Bottle of Water");
	item[i].addDescrLine("A bottle full of fresh water. Restores some");
	item[i].addDescrLine("energy and gives more strength to survive.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Restores 50 health");
	item[i].addDescrLine("- Consumable");
	item[i].consume.healthBalance = 50;
	item[i].addRef(REF_ITEM_CONSUME);
	item[i].category = CATEGORY_CONSUMABLES;
	i += 1;

	// Raw meat
	item[i].type = "food_meat_raw";
	item[i].setName("Raw Meat");
	item[i].addDescrLine("Some raw meat. Can be cooked in a few ways to");
	item[i].addDescrLine("make it better in all ways.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Restores 10 health");
	item[i].addDescrLine("- Restores 20 hunger");
	item[i].addDescrLine("- Consumable");
	item[i].consume.healthBalance = 10;
	item[i].consume.buff.add(BUFF_FOOD, 10.00f, 20);
	item[i].addRef(REF_ITEM_CONSUME);
	item[i].category = CATEGORY_CONSUMABLES;
	i += 1;

	// Materials
	// Flint
	item[i].type = "material_flint";
	item[i].setName("Flint");
	item[i].addDescrLine("A simple yet powerful tool to start a fire in");
	item[i].addDescrLine("the wilderness. Combine with the dagger to get");
	item[i].addDescrLine("some sparks.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Tin
	item[i].type = "material_tin";
	item[i].setName("Tin");
	item[i].addDescrLine("A small piece of tin. Can be useful.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Charcoal
	item[i].type = "material_charcoal";
	item[i].setName("Charcoal");
	item[i].addDescrLine("Some charcoal. Use it to light up the furnace.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Wood
	item[i].type = "material_wood";
	item[i].setName("Wood");
	item[i].addDescrLine("A wooden log.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Stone
	item[i].type = "material_stone";
	item[i].setName("Stone");
	item[i].addDescrLine("A big stone.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Construction stone
	item[i].type = "material_stone_construction";
	item[i].setName("Construction stone");
	item[i].addDescrLine("A high quality finished stone. Can be used");
	item[i].addDescrLine("in construction.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Raw iron
	item[i].type = "material_iron_raw";
	item[i].setName("Raw Iron");
	item[i].addDescrLine("Some raw iron.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Cast iron
	item[i].type = "material_iron_cast";
	item[i].setName("Cast Iron");
	item[i].addDescrLine("Some cast iron.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Animal bone
	item[i].type = "material_bone";
	item[i].setName("Animal Bone");
	item[i].addDescrLine("Durable animal bone.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Steel
	item[i].type = "material_steel";
	item[i].setName("Steel");
	item[i].addDescrLine("Some steel.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Other
	// Basic survival kit
	item[i].type = "survival_kit_basic";
	item[i].setName("Emergency Kit");
	item[i].addDescrLine("Contains some essential survival equipment.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Unpackable");
	item[i].dismantle.add("weapon_knife_steel", 1);
	item[i].dismantle.add("food_meat_canned", 5);
	item[i].dismantle.add("food_water_bottle", 1);
	item[i].dismantle.add("tool_flashlight", 1);
	item[i].dismantle.add("material_flint", 1);
	item[i].addRef(REF_ITEM_UNPACKABLE);
	item[i].category = CATEGORY_OTHER;
	i += 1;

	// Campfire
	item[i].type = "campfire_basic";
	item[i].setName("Campfire");
	item[i].addDescrLine("Incredibly valuable item during the long and cold");
	item[i].addDescrLine("night in the wilds.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Buildable");
	item[i].addRef(REF_ITEM_BUILD);
	item[i].addRef(REF_ITEM_BUILD_CAMPFIRE);
	item[i].category = CATEGORY_OTHER;
	i += 1;

	// Furnace
	item[i].type = "furnace_basic";
	item[i].setName("Simple Furnace");
	item[i].addDescrLine("Primitive furnace that can be used to create basic");
	item[i].addDescrLine("iron equipment.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Buildable");
	item[i].addRef(REF_ITEM_BUILD);
	item[i].addRef(REF_ITEM_BUILD_FURNACEBASIC);
	item[i].category = CATEGORY_OTHER;
	i += 1;

	// Industrial furnace
	item[i].type = "furnace_industrial";
	item[i].setName("Industrial Furnace");
	item[i].addDescrLine("Better furnace that can be used to create advanced");
	item[i].addDescrLine("iron and steel equipment.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Buildable");
	item[i].addRef(REF_ITEM_BUILD);
	item[i].addRef(REF_ITEM_BUILD_FURNACEIND);
	item[i].category = CATEGORY_OTHER;
	i += 1;

	// Tent
	item[i].type = "tent_basic";
	item[i].setName("Camping Tent");
	item[i].addDescrLine("Just a tent. Nothing special.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Buildable");
	item[i].category = CATEGORY_OTHER;
	item[i].addRef(REF_ITEM_BUILD);
	item[i].addRef(REF_ITEM_BUILD_TENT);
	i += 1;

	// Regular Shooter - Story Mode
	item[i].type = "human_eye";
	item[i].setName("Human eye");
	item[i].addDescrLine("It looks inside you...");
	item[i].addDescrLine("Оно смотрит прямо в тебя...");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "human_ear";
	item[i].setName("Human ear");
	item[i].addDescrLine("Why do you even have this thing?!");
	item[i].addDescrLine("Почему ЭТО у тебя в руках?!");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "human_arm";
	item[i].setName("Human arm");
	item[i].addDescrLine("Drop it! Drop it right now!!");
	item[i].addDescrLine("Брось! Брось бяку быстро!");
	item[i].weight = 5.00f;
	i += 1;

	item[i].type = "human_leg";
	item[i].setName("Human leg");
	item[i].addDescrLine("You are a terrible person...");
	item[i].addDescrLine("Ты - ужасный человек...");
	item[i].weight = 5.00f;
	i += 1;

	item[i].type = "voodoo_recipe";
	item[i].setName("Voodoo Recipe");
	item[i].addDescrLine("It says \"Use eyes, ears, arms and legs to");
	item[i].addDescrLine("create a really powerful vooo-dooo doll\".");
	item[i].addDescrLine("Тут написано \"Используй глаза, уши, руки");
	item[i].addDescrLine("и ноги, чтобы сделать действительно крутую");
	item[i].addDescrLine("куклу вуу-дуу.\".");
	i += 1;

	item[i].type = "voodoo_doll";
	item[i].setName("Voodoo doll");
	item[i].addDescrLine("You can use it to summon some slender guys...");
	item[i].addDescrLine("Используй эту штуку, чтобы вызвать что-нибудь");
	item[i].addDescrLine("слендероподобное...");
	item[i].weight = 1.50f;
	i += 1;

	// Editor items
	item[i].type = "tree_basic_";
	item[i].setName("Tree Basic");
	item[i].addRef(REF_ITEM_EDITOR);
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_EDITOR_VAR4);
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "tree_fir_";
	item[i].setName("Tree Fir");
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "stone_basic_";
	item[i].setName("Stone Basic");
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "campfire_full";
	item[i].setName("Campfire");
	item[i].removeRef(REF_ITEM_EDITOR_VAR4);
	i += 1;

	// More test items
	item[i].type = "test_resourceManipulator";
	item[i].setName("Resource Manipulator");
	item[i].category = CATEGORY_OTHER;
	item[i].addDescrLine("Can create the resource from nowhere!");
	item[i].addRef(REF_ITEM_NODROP);
	i += 1;
}