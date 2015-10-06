
#include "main.h"
#include "console.h"
#include "server.h"
#include "client.h"
#include "game.h"
#include "camera.h"
#include "UI.h"
#include "editor.h"
#include "database.h"
#include "world.h"
#include "settings.h"
#include "weather.h"

// Help command
void cmd_help(string args[])
{
	console.echo << "[CMD] Available commands:" << "\n";
	for (int i = 0; i < LIMIT_CMD; i++)
	{
		if (console.cmdSyntax[i].length() > 0 && (!console.cmdServerOnly[i] || !client.connected || core.localServer)
			&& (args[0] == "0" || console.cmdSyntax[i].find(args[0]) != -1))
		{
			console.echo << "[CMD] " << console.cmdSyntax[i] << "\n";
		}
	}
}

// Say
void cmd_say(string args[])
{
	sf::Packet data;
	data << MSG_PLAYER_CHAT << "Server: " + args[0];
	server.sendPacket(PLAYERS_ALL, data);
	data.clear();
	console << "[CHAT] Server: " << args[0] << endl;
}

// Echo
void cmd_echo(string args[])
{
	console.echo << args[0] << endl;
	if (core.serverMode || core.localServer) {
		server.sendEcho(-1, args[0]);
	}
	else if (client.connected) {
		client.sendEcho(args[0]);
	}
}

// Clear
void cmd_clear(string args[])
{
	console.clear();
}

// Macro command
void cmd_macro(string args[])
{
}

// Unit.add
void cmd_unit_add(string args[])
{
	sf::Packet data;
	float posX, posY;
	stringstream(args[1]) >> posX;
	stringstream(args[2]) >> posY;
	game.addUnit(args[0], sf::Vector2f(posX, posY));
}

// Unit.moveto
void cmd_unit_moveto(string args[])
{
	int id;
	float posX, posY;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> posX;
	stringstream(args[2]) >> posY;
	id = game.getUnitId(id);
	if (id != -1) {
		game.unit[id].pos = sf::Vector2f(posX, posY);
	}
}

// Unit.setspeed
void cmd_unit_setspeed(string args[])
{
	int id;
	float speed;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> speed;
	id = game.getUnitId(id);
	if (id != -1) {
		game.unit[id].movementSpeed = speed;
	}
}

// Unit.remove
void cmd_unit_remove(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	game.removeUnit(id);
}

// Unit.getlist
void cmd_unit_getlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] [LocalId]: GlobalId / Type / PosX / PosY" << "\n";
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (type == "0" || game.unit[i].type.find(type) != -1)
		{
			console.echo << "[CMD] [" << i << "]: " << game.unit[i].globalId << " / " << game.unit[i].type << " / "
				<< game.unit[i].pos.x << " / " << game.unit[i].pos.y << "\n";
		}
	}
}

// Unit.order.moveto
void cmd_unit_order_moveto(string args[])
{
	int id;	float targetX, targetY;	bool overwrite;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> targetX;
	stringstream(args[2]) >> targetY;
	stringstream(args[3]) >> overwrite;

	if (game.getUnitId(id) != -1)
	{
		game.unit[game.getUnitId(id)].addOrder_moveto(sf::Vector2f(targetX, targetY), overwrite);
	}
}

// Unit.order.clear
void cmd_unit_order_clear(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	if (game.getUnitId(id) != -1)
	{
		game.unit[game.getUnitId(id)].orderCounter = 0;
	}
}

// Unit.order.getlist
void cmd_unit_order_getlist(string args[])
{
	int id;		string order;
	stringstream(args[0]) >> id;
	id = game.getUnitId(id);

	console.echo << "[CMD] [Id]: Type / Args..." << "\n";
	for (int i = 0; i < game.unit[id].orderCounter; i++)
	{
		if (game.unit[id].order[i].type == ORDER_IDLE) { order = "idle"; }
		if (game.unit[id].order[i].type == ORDER_MOVETO) { order = "moveto"; }

		console.echo << "[CMD] [" << i << "]: " << order << " / ";
		if (order == "moveto")
		{
			console.echo << game.unit[id].order[i].targetPos.x << " / " << game.unit[id].order[i].targetPos.y << "\n";
		}
	}
}

// Unit.item.add
void cmd_unit_item_add(std::string args[])
{
	int unitId, itemCount = 1;
	string itemType;
	stringstream(args[0]) >> unitId;
	itemType = args[1];
	if (args[2] != "0") { stringstream(args[2]) >> itemCount; }

	game.getUnit(unitId).container.add(itemType, itemCount);
}

// Unit.item.remove
void cmd_unit_item_remove(std::string args[])
{
	int unitId, itemCount = 1;
	string itemType;
	stringstream(args[0]) >> unitId;
	itemType = args[1];
	if (args[2] != "0") { stringstream(args[2]) >> itemCount; }

	game.getUnit(unitId).container.remove(itemType, itemCount);
}

// Unit.item.getlist
void cmd_unit_item_getlist(std::string args[])
{
	int unitId;
	stringstream(args[0]) >> unitId;
	cItemContainer* cont = &game.getUnit(unitId).container;

	console.echo << "[CMD] [Id]: Item type / Display name / Amount" << "\n";
	for (int i = 0; i < cont->itemCounter; i++)
	{
		console.echo << "[CMD] [" << i << "]: " << cont->item[i].type << " / " << cont->item[i].displayName << " / " << cont->amount[i] << "\n";
	}
}

// Unit.buff.add
void cmd_unit_buff_add(std::string args[])
{
	int unitId, power = 1, time = -1;
	string type = args[1];
	stringstream(args[0]) >> unitId;
	if (args[2] != "0") { stringstream(args[2]) >> power; }
	if (args[3] != "0") { stringstream(args[3]) >> time; }

	cUnit* target = &game.getUnit(unitId);
	if (target->type != "missingno") {
		int buffId = util.typeToBuffId(type);
		if (buffId != -1) {
			target->addBuff(buffId, (float)time, power);
		}
	}
}

// Unit.buff.remove
void cmd_unit_buff_remove(std::string args[])
{
	int id;
	string type = args[1];
	stringstream(args[0]) >> id;

	cUnit* target = &game.getUnit(id);
	if (target->type != "missingno") {
		target->removeBuff(util.typeToBuffId(type));
	}
}

// Unit.buff.getlist
void cmd_unit_buff_getlist(std::string args[])
{
	int id;
	stringstream(args[0]) >> id;

	cUnit* target = &game.getUnit(id);
	if (target->type != "missingno") {
		console.echo << "[CMD] [Type]: Time / Power / Owner" << endl;

		for (cBuff entry : target->buff.list)
		{
			console.echo << "[CMD] [" << util.buffIdToType(entry.type) << "]: " << entry.duration << " / " << entry.power << " / " << entry.ownerUnit 
				<< " [" << game.getUnit(entry.ownerUnit).type << "]" << endl;
		}
	}
}

// Camera.attach
void cmd_camera_attach(string args[])
{
	camera.moveWithCharacter = !camera.moveWithCharacter;
}

// Camera.lock
void cmd_camera_lock(string args[])
{
	camera.lockedToCharacter = !camera.lockedToCharacter;
}

// Game.timemod
void cmd_game_timemod(string args[])
{
	float value = 100;
	if (args[0] != "0") { stringstream(args[0]) >> value; }

	core.timeModifier = value / 100.00f;
}

// Game.settime
void cmd_game_settime(string args[])
{
	float time = 12.00f;
	stringstream(args[0]) >> time;
	game.setTimeOfDay(time);
}

// Game.setlight
void cmd_game_setlight(string args[])
{
	float light = 255.00f;
	stringstream(args[0]) >> light;
	game.setAmbientLight(light);
}

// Weather.set
void cmd_weather_set(string args[])
{
	float power;
	if (args[1] == "none") { power = 0.00f; }
	else if (args[1] == "light") { power = 7000.00f; }
	else if (args[1] == "0" || args[1] == "medium") { power = 15000.00f; }
	else if (args[1] == "heavy") { power = 25000.00f; }
	else if (args[1] == "insane") { power = 50000.00f; }
	else { stringstream(args[1]) >> power; }

	if (args[0] == "snow") { weather.set(WEATHER_SNOW, power); weather.changeTo(WEATHER_SNOW, power); }
	else if (args[0] == "cloud") { weather.setClouds(power); weather.changeCloudsTo(power); }
	else if (args[0] == "wind") { weather.setWind(power); weather.changeWindTo(power); }
}

// Weather.getlist
void cmd_weather_getlist(string args[])
{
	console.echo << "[CMD] Weather types:" << endl;
	console.echo << "[CMD] - snow" << endl;
	console.echo << "[CMD] - wind" << endl;
	console.echo << "[CMD] - cloud" << endl;
	console.echo << "[CMD] Weather modifiers:" << endl;
	console.echo << "[CMD] - light" << endl;
	console.echo << "[CMD] - medium" << endl;
	console.echo << "[CMD] - heavy" << endl;
	console.echo << "[CMD] - insane" << endl;
	console.echo << "[CMD] - none" << endl;
}

// Client.connect
void cmd_client_connect(string args[])
{
	string ip = args[0];
	short port = 21045;
	if (args[1] != "0") { stringstream(args[1]) >> port; }
	client.connect(ip, port);
}

// Client.disconnect
void cmd_client_disconnect(string args[])
{
	console.echo << "[CMD] Connection to server closed" << "\n";
	client.disconnect();
}

// Server.open
void cmd_server_open(string args[])
{
	//core.localServer = true;
	//console << "[CMD] Local server operational" << "\n";
	//console << "[CMD] Warning: Local server is not really implemented yet. Expect random bugs." << "\n";
}

// Player.setunit
void cmd_player_setunit(string args[])
{
	int player, unit;
	stringstream(args[0]) >> player;
	stringstream(args[1]) >> unit;
	server.player[player].unit = unit;
}

// Player.getlist
void cmd_player_getlist(string args[])
{
	console.echo << "[CMD] [Id]: Unit Id / Address / Ping" << "\n";
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (server.player[i].connected)
		{
			console.echo << "[CMD] [" << i << "]: " << server.player[i].unit << " / " << server.player[i].socket.getRemoteAddress().toString() 
				<< " / " << server.player[i].ping << " ms" << "\n";
		}
	}
}

// Player.kick
void cmd_player_kick(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	// Closing the connection
	console.echo << "[CMD] Player " << id << " kicked!" << "\n";
	server.player[id].disconnect();
}

// Ui.getlist
void cmd_ui_getlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] [Id]: Local id / Type / Pos X / Pos Y / Action / Args[0]" << "\n";
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (ui.element[i].isValid && (type == "0" || ui.element[i].type.find(type) != -1))
		{
			console.echo << "[CMD] [" << ui.element[i].globalId << "]: " << i << " / " << ui.element[i].type << " / " << ui.element[i].pos.x << " / " <<
				ui.element[i].pos.y << " / " << ui.element[i].button.action << " / " << ui.element[i].button.args[0] << "\n";
		}
	}
}

// Editor.enable
void cmd_editor_enable(string args[])
{
	editor.enable();
}

// Editor.save
void cmd_editor_save(string args[])
{
	editor.saveBlueprint(args[0]);
}

// Editor.load
void cmd_editor_load(string args[])
{
	editor.loadBlueprint(args[0]);
}

// Editor.autogen
void cmd_editor_autogen(string args[])
{
	int count, startIndex = 0;
	stringstream(args[0]) >> count;
	stringstream(args[1]) >> startIndex;
	string index;

	if (count == 0) { editor.genBlueprint(); }
	for (int i = 0; i < count; i++)
	{
		editor.genBlueprint();

		if (i + startIndex < 10) { index = "00" + to_string(i + startIndex); }
		else if (i + startIndex < 100) { index = "0" + to_string(i + startIndex); }
		else { index = to_string(i + startIndex); }
		editor.saveBlueprint("gen_t" + to_string(editor.autogenType) + "_" + index);
	}
}

// Editor.setgentype
void cmd_editor_setgentype(string args[])
{
	int type;
	stringstream(args[0]) >> type;

	editor.autogenType = type;
}

// Database.unit.getlist
void cmd_database_unit_getlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] Type" << "\n";
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		if (database.unit[i].type.length() > 0 && (type == "0" || database.unit[i].type.find(type) != -1))
		{
			console << "- " << database.unit[i].type << endl;
		}
	}
}

// Database.item.getlist
void cmd_database_item_getlist(std::string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] Type / Display name" << "\n";
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (database.item[i].type.length() > 0 && (type == "0" || database.item[i].type.find(type) != -1 || database.item[i].displayName.find(type) != -1))
		{
			console << "- " << database.item[i].type << " / " << database.item[i].displayName << endl;
		}
	}
}

// Database.buff.getlist
void cmd_database_buff_getlist(std::string args[])
{
	string type;
	console.echo << "[CMD] Type" << "\n";
	
	for (int i = 0; i < 64; i++)
	{
		type = util.buffIdToType(i);
		if (type != "missingno") {
			console.echo << "[CMD] - " << type << endl;
		}
	}
}

// Database.reload
void cmd_database_reload(string args[])
{
	database.init();

	for (int y = 0; y < LIMIT_MAP; y++)
	{
		for (int x = 0; x < LIMIT_MAP; x++)
		{
			if (world.map[x][y].isLoaded)
			{
				world.saveChunk(vec2i(x, y));
				world.unloadChunk(vec2i(x, y));
				world.loadChunk(vec2i(x, y));
			}
		}
	}
}

// Settings.load
void cmd_settings_load(std::string args[])
{
	settings.load();
}

// Settings.save
void cmd_settings_save(std::string args[])
{
	settings.save();
}

// Settings.update
void cmd_settings_update(std::string args[])
{
	settings.updateFile();
}

// World.saveTo
void cmd_world_saveto(std::string args[])
{
	world.saveTo(args[0], args[1]);
}

// World.loadFrom
void cmd_world_loadfrom(std::string args[])
{
	world.loadFrom(args[0], args[1]);
}

// Exit command
void cmd_exit(string args[])
{
	console.online = false;
	core.shutdown = true;
}