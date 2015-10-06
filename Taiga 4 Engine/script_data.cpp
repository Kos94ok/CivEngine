
#include "main.h"
#include "script.h"
#include "game.h"
#include "world.h"
#include "UI.h"
#include "math.h"
#include "server.h"
#include "camera.h"
#include "visual.h"
#include "window.h"
#include "target.h"
#include "path.h"
#include "client.h"

void cScript::shooter_spawnEnemies(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Waiting for player spawn
	while (game.getUnitCount("player") == 0 && !core.shutdown) { Sleep(50); }
	// Spawning the enemies constantly
	while (!core.shutdown)
	{
		// Maximum enemy limit
		if (game.getUnitCount("enemy") < 50)
		{
			// Create a unit
			game.addUnit("enemy", world.spawnPoint + vec2f(math.randf(-1500.00f, 1500.00f), math.randf(-1500.00f, 1500.00f)));
		}
		// Wait
		script.wait(1500);
	}
}

void cScript::shooter_respawnSystem(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Loop
	while (!core.shutdown)
	{
		script.wait(5000);
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (server.player[i].connected && game.isUnitDead(server.player[i].unit))
			{
				server.assignUnit(i, game.addUnit("player", world.spawnPoint + vec2f(math.randf(-500.00f, 500.00f), math.randf(-500.00f, 500.00f))));
			}
		}
	}
}

void cScript::shooter_spawnItems(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Spawning stuff
	game.createDrop(world.spawnPoint + vec2f(0.00f, -1000.00f), "human_eye");
	game.createDrop(world.spawnPoint + vec2f(0.00f, 1000.00f), "human_arm");
	game.createDrop(world.spawnPoint + vec2f(-1000.00f, 0.00f), "human_leg");
	game.createDrop(world.spawnPoint + vec2f(1000.00f, 0.00f), "human_ear");
	game.createDrop(world.spawnPoint + vec2f(-50.00f, 0.00f), "voodoo_recipe");
}

void cScript::test_unitAddSystem(cArg args)
{
	script.wait(5000);

	const int count = 100;

	int number[count];

	for (int a = 0; a < 100; a++)
	{
		for (int i = 0; i < count; i++)
		{
			number[i] = game.addUnit("missingno", vec2f(0.00f, 0.00f));
		}
		for (int i = 0; i < count; i++)
		{
			game.removeUnit(number[i]);
		}

		if (a % 2 == 0) { ui.openInventory(); }
		else { ui.closeInventory(); }
	}
}

void cScript::test_consoleSystem(cArg args)
{
	console.debug << "Starting console test..." << endl;
	script.wait(5000);

	for (int i = 0; i < 100; i++)
	{
		console.debug << "[SCRIPT] Console test string " << i << endl;
		script.wait(200);
	}
}

void cScript::ui_initialMenu(cArg args)
{
	console << "[MAIN] Making some menu" << "\n";
	// Background image
	int id = ui.addElement("image", vec2f(camera.res.x / 2, camera.res.y / 2));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("bg_artBlur.png");
	ui.element[ui.getElementId(id)].size = vec2f(camera.res.x, camera.res.y);
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	// Button background
	id = ui.addElement("image", vec2f(125.00f, 0.00f));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].size = vec2f(250.00f, camera.res.y);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].alpha = 150;
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);

	// Main menu
	id = ui.createText(vec2f(250.00f, 100.00f), "Main Menu");
	ui.element[ui.getElementId(id)].ignoreOrigin = false;
	ui.element[ui.getElementId(id)].textSize = 40.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	// Full name
	id = ui.createText(vec2f(camera.res.x / 2, 50.00f), NAME_FULL, "Its a tooltip!");
	ui.element[ui.getElementId(id)].ignoreOrigin = false;
	ui.element[ui.getElementId(id)].textSize = 36.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	// Version number
	id = ui.createText(vec2f(camera.res.x - 330.00f, camera.res.y - 20.00f), "Version: " + NAME_VERSION_FULL + " (" + NAME_BUILDTIME_DATE + ", " + NAME_BUILDTIME_TIME + ")",
		"But not in early access!");
	ui.element[ui.getElementId(id)].textSize = 16.00f;
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);

	float vertPos = 200.00f;
	id = ui.addElement("button", sf::Vector2f(250.00f, vertPos + 0.00f));
	ui.element[ui.getElementId(id)].size.x = 250.00f;
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].alpha = 150;
	ui.element[ui.getElementId(id)].textSize = 22.00f;
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
	ui.element[ui.getElementId(id)].setText("Taiga Mini");
	ui.element[ui.getElementId(id)].button.action = "start_taigaMaxi";
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(250.00f, vertPos + 35.00f));
	ui.element[ui.getElementId(id)].setText("Generic Shooter");
	ui.element[ui.getElementId(id)].button.action = "start_genericShooter";
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(250.00f, vertPos + 70.00f));
	ui.element[ui.getElementId(id)].setText("Quick Connect");
	ui.element[ui.getElementId(id)].button.action = "connect_temp";
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
	id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(250.00f, vertPos + 105.00f));
	ui.element[ui.getElementId(id)].setText("Editor");
	ui.element[ui.getElementId(id)].button.action = "start_editor";
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);

	id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(250.00f, camera.res.y - 50.00f));
	ui.element[ui.getElementId(id)].setText("Exit");
	ui.element[ui.getElementId(id)].button.action = "exit";
	ui.element[ui.getElementId(id)].addRef(REF_UI_MAINMENU);
}

void cScript::server_sendChunkData(cArg args)
{
	int i, arg0, arg1;
	stringstream(args[0]) >> i;
	stringstream(args[1]) >> arg0;
	stringstream(args[2]) >> arg1;
	server.sendChunkData(i, arg0, arg1);
}

void cScript::unit_torch(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Arguments: Flashlight unit id, Player index
	int unitId, playerId;
	stringstream(args[0]) >> unitId;
	stringstream(args[1]) >> playerId;

	while (!core.shutdown)
	{
		game.access.lock();
		cUnit* targetUnit = &game.getUnit(unitId);
		cUnit* ownerUnit = &game.getUnit(server.player[playerId].unit);
		if (targetUnit->type == "missingno") { game.access.unlock(); return; }
		else if (ownerUnit->type == "missingno" || !ownerUnit->hasBuff(BUFF_TORCH)) {
			game.removeUnit(unitId);
			game.access.unlock();
			return;
		}

		targetUnit->moveTo(ownerUnit->pos);
		game.access.unlock();

		Sleep(3);
	}
}

void cScript::unit_flashlight(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Arguments: Flashlight unit id, Player index
	int unitId, playerId;
	stringstream(args[0]) >> unitId;
	stringstream(args[1]) >> playerId;

	while (!core.shutdown)
	{
		game.access.lock();
		cUnit* targetUnit = &game.getUnit(unitId);
		cUnit* ownerUnit = &game.getUnit(server.player[playerId].unit);
		if (targetUnit->type == "missingno") { game.access.unlock(); return; }
		else if (ownerUnit->type == "missingno" || !ownerUnit->hasBuff(BUFF_FLASHLIGHT)) {
			game.removeUnit(unitId);
			game.access.unlock();
			return;
		}
		
		targetUnit->moveTo(ownerUnit->pos);
		targetUnit->rotateTo(math.convertAngle(math.getAngle(targetUnit->pos, server.player[playerId].mousePos)));
		game.access.unlock();

		Sleep(3);
	}
}

void cScript::unit_bindToMouse(cArg args)
{
	// Arguments: Unique unit reference
	int ref;
	stringstream(args[0]) >> ref;
	while (!core.shutdown && target.activeBuild)
	{
		cUnit* targetUnit = &game.getUnitByRef(ref);
		if (targetUnit->type != "missingno") {
			mutex.renderUnits.lock();
			targetUnit->pos = window.getMousePos(true);
			mutex.renderUnits.unlock();
			target.updateBuildStatus(targetUnit->globalId);
		}
		Sleep(5);
	}
}