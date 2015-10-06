
#include "main.h"
#include "database.h"
#include "visual.h"

void cDatabase::loadUnits()
{
	int i = 0;
	unit[i].type = "missingno";
	unit[i].size = vec2(20, 20);
	unit[i].addRef(REF_UNIT_NORENDER);
	i += 1;

	// Actual units
	unit[i].type = "player";
	unit[i].size = vec2(64, 64);
	unit[i].center = vec2(32, 52);
	unit[i].health = 100.00f;
	unit[i].maxHealth = 100.00f;
	unit[i].movementSpeed = 75;
	unit[i].rotateSpeed = 360.00f;
	unit[i].animData[ANIM_IDLE].up.tex = visual.addTexture("player_idle_up.png");
	unit[i].animData[ANIM_IDLE].upDiag.tex = visual.addTexture("player_idle_diagup.png");
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("player_idle_side.png");
	unit[i].animData[ANIM_IDLE].downDiag.tex = visual.addTexture("player_idle_diagdown.png");
	unit[i].animData[ANIM_IDLE].down.tex = visual.addTexture("player_idle_down.png");
	unit[i].animData[ANIM_MOVE].up = cAnimDirection(visual.addTexture("player_move_up.png"), -1, 4, 0.25f);
	unit[i].animData[ANIM_MOVE].upDiag = cAnimDirection(visual.addTexture("player_move_diagup.png"), -1, 4, 0.25f);
	unit[i].animData[ANIM_MOVE].side = cAnimDirection(visual.addTexture("player_move_side.png"), -1, 4, 0.25f);
	unit[i].animData[ANIM_MOVE].downDiag = cAnimDirection(visual.addTexture("player_move_diagdown.png"), -1, 4, 0.25f);
	unit[i].animData[ANIM_MOVE].down = cAnimDirection(visual.addTexture("player_move_down.png"), -1, 4, 0.25f);
	unit[i].animData[ANIM_WORK] = unit[i].animData[ANIM_MOVE];
	unit[i].light.power = 320.00f;
	unit[i].light.texture = visual.addTexture("light_whiteWeak.png");
	unit[i].container.itemLimit = LIMIT_ITEMS;
	unit[i].interactDistance = 30.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].sound.footsteps.setFrames(0, 2);
	unit[i].sound.footsteps.setData("footsteps_human_snow001", "footsteps_human_snow002", "footsteps_human_snow003");
	unit[i].addRef(REF_UNIT_NOUNLOAD);
	unit[i].addRef(REF_UNIT_BESTPATHING);
	unit[i].addRef(REF_UNIT_SCARY);
	i += 1;

	unit[i].type = "rabbit";
	unit[i].size = vec2(30, 30);
	unit[i].center = vec2(15, 20);
	unit[i].health = 10.00f;
	unit[i].maxHealth = 10.00f;
	unit[i].movementSpeed = 120;
	unit[i].rotateSpeed = 360.00f;
	unit[i].animData[ANIM_IDLE].up.tex = visual.addTexture("rabbit_idle_up.png");
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("rabbit_idle_side.png");
	unit[i].animData[ANIM_IDLE].down.tex = visual.addTexture("rabbit_idle_down.png");
	unit[i].animData[ANIM_MOVE].up = cAnimDirection(visual.addTexture("rabbit_move_up.png"), -1, 4, 0.15f);
	unit[i].animData[ANIM_MOVE].side = cAnimDirection(visual.addTexture("rabbit_move_side.png"), -1, 4, 0.15f);
	unit[i].animData[ANIM_MOVE].down = cAnimDirection(visual.addTexture("rabbit_move_down.png"), -1, 4, 0.15f);
	unit[i].animData[ANIM_DEATH].side = cAnimDirection(visual.addTexture("rabbit_death_side.png"), -1, 4, 1.00f);
	unit[i].drop.add("food_meat_raw", 1, 0.90f);
	unit[i].drop.add("food_meat_raw", 1, 0.35f);
	unit[i].drop.add("food_meat_raw", 1, 0.10f);
	unit[i].drop.add("material_bone", 1, 0.75f);
	unit[i].drop.add("material_bone", 1, 0.50f);
	unit[i].drop.add("material_bone", 1, 0.25f);
	unit[i].interactDistance = 10.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].ai = cAI::think_rabbit;
	unit[i].sound.footsteps.setFrames(0, 2);
	unit[i].sound.footsteps.setData("footsteps_animal_snow001", "footsteps_animal_snow002", "footsteps_animal_snow003");
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_BESTPATHING);
	i += 1;

	unit[i].type = "enemy";
	unit[i].size = vec2(64, 64);
	unit[i].center = vec2(32, 32);
	unit[i].health = 10.00f;
	unit[i].maxHealth = 10.00f;
	unit[i].movementSpeed = 80;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("enemy_idle_side.png");
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("enemy_idle_side.png");
	unit[i].light.power = 100.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].interactDistance = 20.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].ai = cAI::think_enemy;
	unit[i].addRef(REF_UNIT_NOUNLOAD);
	unit[i].addRef(REF_UNIT_BESTPATHING);
	i += 1;

	// Trees
	unit[i].type = "tree_basic_a";
	unit[i].size = vec2(150, 300);
	unit[i].center = vec2(75, 280);
	unit[i].health = 500.00f;
	unit[i].maxHealth = 500.00f;
	unit[i].resource = 275.00f;
	unit[i].rotateSpeed = 5760.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_a_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("tree_basic_a_idle_shadow.png");
	unit[i].interactDistance = 15.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].selectionOffset = vec4f(0.00f, 0.00f, 0.25f, 0.25f);
	//unit[i].sound.idle = "wind_tree";
	unit[i].drop.add("material_wood", 1, 1.00f);
	unit[i].drop.add("material_wood", 1, 0.75f);
	unit[i].drop.add("material_wood", 1, 0.50f);
	unit[i].drop.add("material_wood", 1, 0.25f);
	unit[i].addRef(REF_UNIT_TREE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = database.getUnit("tree_basic_a");
	unit[i].type = "tree_basic_b";
	unit[i].size = vec2f(175, 175);
	unit[i].center = vec2f(95, 155);
	unit[i].shadowOffset = vec2f(0, -5);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_b_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("tree_basic_b_idle_shadow.png");
	unit[i].selectionOffset = vec4f(0.00f, 0.00f, 0.25f, 0.25f);
	i += 1;

	unit[i] = database.getUnit("tree_basic_a");
	unit[i].type = "tree_basic_c";
	unit[i].size = vec2f(220, 220);
	unit[i].center = vec2f(100, 190);
	unit[i].shadowOffset = vec2f(0, -5);
	unit[i].collisionDistance = 15.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_c_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("tree_basic_c_idle_shadow.png");
	unit[i].selectionOffset = vec4f(0.10f, 0.00f, 0.25f, 0.25f);
	i += 1;

	unit[i] = database.getUnit("tree_basic_a");
	unit[i].type = "tree_basic_d";
	unit[i].size = vec2f(230, 230);
	unit[i].center = vec2f(160, 205);
	unit[i].shadowOffset = vec2f(0, -5);
	unit[i].collisionDistance = 15.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_d_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("tree_basic_d_idle_shadow.png");
	unit[i].selectionOffset = vec4f(0.00f, 0.00f, 0.50f, 0.20f);
	i += 1;

	// Fir trees
	unit[i].type = "tree_fir_a";
	unit[i].size = vec2(100, 170);
	unit[i].center = vec2(50, 167);
	unit[i].health = 500.00f;
	unit[i].maxHealth = 500.00f;
	unit[i].resource = 275.00f;
	unit[i].rotateSpeed = 5760.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_fir_a_idle.png");
	unit[i].animData[ANIM_DEATH].side.frameCount = 4;
	unit[i].animData[ANIM_DEATH].side.frameDelay = 0.75f;
	unit[i].animData[ANIM_DEATH].side.tex = visual.addTexture("tree_fir_a_death.png");
	unit[i].interactDistance = 15.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].drop.add("material_wood", 1, 1.00f);
	unit[i].drop.add("material_wood", 1, 0.75f);
	unit[i].drop.add("material_wood", 1, 0.50f);
	unit[i].drop.add("material_wood", 1, 0.25f);
	unit[i].addRef(REF_UNIT_TREE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_fir_b";
	unit[i].center = vec2(52, 170);
	unit[i].shadowOffset = vec2(0, -1);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_fir_b_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_fir_c";
	unit[i].shadowOffset = vec2(0, 0);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_fir_c_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_fir_d";
	unit[i].center = vec2(50, 160);
	unit[i].shadowOffset = vec2(0, -10);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_fir_d_idle.png");
	i += 1;

	// Stones
	unit[i].type = "stone_basic_a";
	unit[i].size = vec2(70, 70);
	unit[i].center = vec2(35, 50);
	unit[i].shadowOffset = vec2(0, -5);
	unit[i].health = 500.00f;
	unit[i].maxHealth = 500.00f;
	unit[i].resource = 275.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_a_idle.png");
	unit[i].interactDistance = 35.00f;
	unit[i].collisionDistance = 25.00f;
	unit[i].drop.add("material_stone", 1, 1.00f);
	unit[i].drop.add("material_stone", 1, 0.75f);
	unit[i].drop.add("material_stone", 1, 0.50f);
	unit[i].drop.add("material_flint", 1, 0.50f);
	unit[i].drop.add("material_flint", 1, 0.40f);
	unit[i].drop.add("material_flint", 1, 0.30f);
	unit[i].drop.add("material_iron_raw", 1, 0.75f);
	unit[i].drop.add("material_iron_raw", 1, 0.50f);
	unit[i].addRef(REF_UNIT_STONE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_b";
	unit[i].center = vec2(35, 60);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_b_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_c";
	unit[i].center = vec2(35, 55);
	unit[i].shadowOffset = vec2(0, -5);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_c_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_d";
	unit[i].center = vec2(35, 55);
	unit[i].shadowOffset = vec2(0, -15);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_d_idle.png");
	i += 1;

	// Campfire
	unit[i].type = "campfire_full";
	unit[i].size = vec2(40, 50);
	unit[i].center = vec2(20, 45);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].resource = 50.00f;
	unit[i].resourceLimit = 200.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("campfire_100p_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("campfire_100p_shadow.png");
	unit[i].animData[ANIM_IDLE].side.frameCount = 4;
	unit[i].animData[ANIM_IDLE].side.frameDelay = 0.20f;
	unit[i].interactDistance = 20.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].light.power = 700.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	unit[i].sound.idle = "campfire";
	unit[i].aura.add(BUFF_HEAT, 100, 500.00f, true, true);
	unit[i].addRef(REF_UNIT_ADDRESOURCE);
	unit[i].addRef(REF_UNIT_BURNOUT_CAMPFIRE);
	i += 1;

	// Tent
	unit[i].type = "tent_basic";
	unit[i].size = vec2(94, 84);
	unit[i].center = vec2(47, 70);
	unit[i].shadowOffset = vec2(0, -15);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tent_basic.png");
	unit[i].interactDistance = 42.00f;
	unit[i].collisionDistance = 35.00f;
	unit[i].addRef(REF_UNIT_PACK);
	unit[i].addRef(REF_UNIT_PACK_TENT);
	i += 1;

	// Dummy units
	unit[i].type = "dummy_flashlight";
	unit[i].size = vec2(0, 0);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].rotateSpeed = 720.00f;
	unit[i].collisionDistance = 0.00f;
	unit[i].light.power = 500.00f;
	unit[i].light.texture = visual.addTexture("lightdir_white.png");
	unit[i].light.directional = true;
	unit[i].light.priority = 1;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	unit[i].addRef(REF_UNIT_NOCOLLISION);
	i += 1;

	unit[i].type = "dummy_torch";
	unit[i].size = vec2(0, 0);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].collisionDistance = 0.00f;
	unit[i].light.power = 300.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	unit[i].addRef(REF_UNIT_NOCOLLISION);
	i += 1;

	// Backgrounds
	unit[i].type = "bg_snow_a";
	unit[i].size = vec2f(500.00f, 500.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_a.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_NOCOLLISION);
	unit[i].addRef(REF_UNIT_NOSELECTION);
	unit[i].addRef(REF_UNIT_RENDERFIRST);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "bg_snow_b";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_b.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "bg_snow_c";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_c.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "bg_snow_d";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_d.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "bg_snow_e";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_e.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "bg_snow_f";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("bg_snow_f.png");
	i += 1;

	// Other
	unit[i].type = "editor";
	unit[i].size = vec2(0, 0);
	unit[i].center = vec2(0, 0);
	unit[i].container.itemLimit = 1000;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	i += 1;

	unit[i].type = "item_a";
	unit[i].size = vec2(30, 30);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("itemWorld_idle.png");
	unit[i].interactDistance = 15.00f;
	unit[i].light.power = 100.00f;
	unit[i].light.texture = visual.addTexture("light_blueWeak.png");
	unit[i].selectionPriority = 3;
	unit[i].addRef(REF_UNIT_PICKUP);
	i += 1;

	unit[i].type = "proj_rifle";
	unit[i].size = vec2(10, 2);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].flyingHeight = 15.00f;
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("missile_rifleNew.png");
	unit[i].rotateSpeed = 5760.00f;
	unit[i].movementSpeed = 1500.00f;
	unit[i].light.power = 25.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].addRef(REF_UNIT_ROTATE);
	unit[i].addRef(REF_UNIT_MISSILE);
	unit[i].addRef(REF_UNIT_NOSELECTION);
	unit[i].addRef(REF_UNIT_DAMAGE_RIFLE);
	i += 1;

	unit[i].type = "arena_wall";
	unit[i].size = vec2(20.00f, 20.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("red.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE);
	i += 1;

	unit[i].type = "fogofwar";
	unit[i].size = vec2(500.00f, 500.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("grey.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_NOSELECTION);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE);
	i += 1;
}