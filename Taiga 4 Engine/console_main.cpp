
#include "main.h"
#include "console.h"
#include "client.h"

// Initializing the console class
cConsole::cConsole()
{
	for (int i = 0; i < LIMIT_CMD; i++) { console.cmdServerOnly[i] = true; }
	info.type = SUBCMD_INFO;
	echo.type = SUBCMD_ECHO;
	error.type = SUBCMD_ERROR;
	debug.type = SUBCMD_DEBUG;
	history[SUBCMD_ALL].push_back("[CONSOLE] All Messages Tab");
	history[SUBCMD_ALL].push_back("============================");
	history[SUBCMD_INFO].push_back("[CONSOLE] Info Messages Tab");
	history[SUBCMD_INFO].push_back("============================");
	history[SUBCMD_ECHO].push_back("[CONSOLE] Echo Messages Tab");
	history[SUBCMD_ECHO].push_back("============================");
	history[SUBCMD_ERROR].push_back("[CONSOLE] Error Messages Tab");
	history[SUBCMD_ERROR].push_back("============================");
	history[SUBCMD_DEBUG].push_back("[CONSOLE] Debug Messages Tab");
	history[SUBCMD_DEBUG].push_back("============================");

	online = true;
	displayed = false;
	scrollOffset = 0;
	cmdHistoryPos = 0;
	displayedPage = SUBCMD_ALL;
	clearInput();
	CreateDirectoryA("Logs", 0);
	// Help command
	int cmdID = 0;
	console.cmdRegex[cmdID].assign("^help *");
	console.cmdWrong[cmdID] = "help";
	console.cmdSyntax[cmdID] = "\"help (str)\" - Display available commands with (filter)";
	console.cmdFunc[cmdID] = cmd_help;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^say *");
	console.cmdWrong[cmdID] = "say";
	console.cmdSyntax[cmdID] = "\"say [str]\" - Say the [string] to chat";
	console.cmdFunc[cmdID] = cmd_say;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^echo *");
	console.cmdWrong[cmdID] = "echo";
	console.cmdSyntax[cmdID] = "\"echo [str]\" - Echo the [string] to console";
	console.cmdFunc[cmdID] = cmd_echo;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^clear");
	console.cmdWrong[cmdID] = "clear";
	console.cmdSyntax[cmdID] = "\"clear\" - Clear the console";
	console.cmdFunc[cmdID] = cmd_clear;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^macro [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "macro";
	console.cmdSyntax[cmdID] = "\"macro [str]\" - Load the [macro] script";
	console.cmdFunc[cmdID] = cmd_macro;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.add [a-zA-Z0-9-]+");
	console.cmdWrong[cmdID] = "unit.add";
	console.cmdSyntax[cmdID] = "\"unit.add [str] (num) (num)\" - Create a unit from [type] at (X) (Y)";
	console.cmdFunc[cmdID] = cmd_unit_add;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.moveto [0-9]+ [0-9-]+ [0-9-]+");
	console.cmdWrong[cmdID] = "unit.moveto";
	console.cmdSyntax[cmdID] = "\"unit.moveto [num] [num] [num] (bool)\" - Move a unit with [global ID] to [X] [Y]";
	console.cmdFunc[cmdID] = cmd_unit_moveto;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.setspeed [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "unit.setspeed";
	console.cmdSyntax[cmdID] = "\"unit.setspeed [num] [num]\" - Set the [unit]'s movement speed to [value]";
	console.cmdFunc[cmdID] = cmd_unit_setspeed;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.remove [0-9]+");
	console.cmdWrong[cmdID] = "unit.remove";
	console.cmdSyntax[cmdID] = "\"unit.remove [num]\" - Remove a unit with [global ID]";
	console.cmdFunc[cmdID] = cmd_unit_remove;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.getlist");
	console.cmdWrong[cmdID] = "unit.getlist";
	console.cmdSyntax[cmdID] = "\"unit.getlist (str)\" - Display list of units of (type)";
	console.cmdFunc[cmdID] = cmd_unit_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.order.moveto [0-9]+ [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "unit.order.moveto";
	console.cmdSyntax[cmdID] = "\"unit.order.moveto [num] [num] [num]\" - Order [unit] to go to [X] [Y]";
	console.cmdFunc[cmdID] = cmd_unit_order_moveto;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.order.getlist [0-9]+");
	console.cmdWrong[cmdID] = "unit.order.getlist";
	console.cmdSyntax[cmdID] = "\"unit.order.getlist [num]\" - Get the list of orders for [unit]";
	console.cmdFunc[cmdID] = cmd_unit_order_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.add [0-9]+ [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "unit.item.add";
	console.cmdSyntax[cmdID] = "\"unit.item.add [num] [str] (num)\" - Give [unit] one [item] (amount) times";
	console.cmdFunc[cmdID] = cmd_unit_item_add;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.remove [0-9]+ [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "unit.item.remove";
	console.cmdSyntax[cmdID] = "\"unit.item.remove [num] [str] (num)\" - Remove from [unit] one [item] (amount) times";
	console.cmdFunc[cmdID] = cmd_unit_item_remove;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.getlist [0-9]+");
	console.cmdWrong[cmdID] = "unit.item.getlist";
	console.cmdSyntax[cmdID] = "\"unit.item.getlist [num]\" - Get the list of items for [unit]";
	console.cmdFunc[cmdID] = cmd_unit_item_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.buff.add [0-9]+ [a-zA-Z0-9-_]");
	console.cmdWrong[cmdID] = "unit.buff.add";
	console.cmdSyntax[cmdID] = "\"unit.buff.add [num] [str] (num) (num)\" - Give [unit] a buff with [type] and (power), lasting for (time)";
	console.cmdFunc[cmdID] = cmd_unit_buff_add;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.buff.remove [0-9]+ [a-zA-Z0-9-_]+");
	console.cmdWrong[cmdID] = "unit.buff.remove";
	console.cmdSyntax[cmdID] = "\"unit.buff.remove [num] [str]\" - Remove all the buffs from [unit] with [type]";
	console.cmdFunc[cmdID] = cmd_unit_buff_remove;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.buff.getlist [0-9]+");
	console.cmdWrong[cmdID] = "unit.buff.getlist";
	console.cmdSyntax[cmdID] = "\"unit.buff.getlist [num]\" - Get the list of active buffs for [unit]";
	console.cmdFunc[cmdID] = cmd_unit_buff_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^camera.attach");
	console.cmdWrong[cmdID] = "camera.attach";
	console.cmdSyntax[cmdID] = "\"camera.attach\" - Attach the camera to controlled unit";
	console.cmdFunc[cmdID] = cmd_camera_attach;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^camera.lock");
	console.cmdWrong[cmdID] = "camera.lock";
	console.cmdSyntax[cmdID] = "\"camera.lock\" - Lock the camera to controlled unit";
	console.cmdFunc[cmdID] = cmd_camera_lock;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.timemod [0-9]+");
	console.cmdWrong[cmdID] = "game.timemod";
	console.cmdSyntax[cmdID] = "\"game.timemod [num]\" - Set the game speed to [percentage]";
	console.cmdFunc[cmdID] = cmd_game_timemod;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.settime [0-9.,]+");
	console.cmdWrong[cmdID] = "game.settime";
	console.cmdSyntax[cmdID] = "\"game.settime [num]\" - Set the game [time] of the day";
	console.cmdFunc[cmdID] = cmd_game_settime;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.setlight [0-9.,]+");
	console.cmdWrong[cmdID] = "game.setlight";
	console.cmdSyntax[cmdID] = "\"game.setlight [num]\" - Set the ambient light to [value]";
	console.cmdFunc[cmdID] = cmd_game_setlight;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^weather.set [a-zA-Z0-9.,]+");
	console.cmdWrong[cmdID] = "weather.set";
	console.cmdSyntax[cmdID] = "\"weather.set [str] (str)\" - Set the weather to [type] with (power)";
	console.cmdFunc[cmdID] = cmd_weather_set;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^weather.getlist");
	console.cmdWrong[cmdID] = "weather.getlist";
	console.cmdSyntax[cmdID] = "\"weather.getlist\" - Display the list of possible weather types";
	console.cmdFunc[cmdID] = cmd_weather_getlist;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^client.connect [0-9.:a-zA-Z]+");
	console.cmdWrong[cmdID] = "client.connect";
	console.cmdSyntax[cmdID] = "\"client.connect [ip]\" - Connect to the server at [address]";
	console.cmdFunc[cmdID] = cmd_client_connect;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^client.disconnect");
	console.cmdWrong[cmdID] = "client.disconnect";
	console.cmdSyntax[cmdID] = "\"client.disconnect\" - Disconnect from server";
	console.cmdFunc[cmdID] = cmd_client_disconnect;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^server.open");
	console.cmdWrong[cmdID] = "server.open";
	console.cmdSyntax[cmdID] = "\"server.open\" - Open the local server";
	console.cmdFunc[cmdID] = cmd_server_open;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.kick [0-9]+");
	console.cmdWrong[cmdID] = "player.kick";
	console.cmdSyntax[cmdID] = "\"player.kick [num]\" - Kick the [player] from server";
	console.cmdFunc[cmdID] = cmd_player_kick;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.setunit [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "player.setunit";
	console.cmdSyntax[cmdID] = "\"player.setunit [num] [num]\" - Assign the [player]'s unit to [global id]";
	console.cmdFunc[cmdID] = cmd_player_setunit;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.getlist");
	console.cmdWrong[cmdID] = "player.getlist";
	console.cmdSyntax[cmdID] = "\"player.getlist\" - Display the list of players on local server";
	console.cmdFunc[cmdID] = cmd_player_getlist;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^ui.getlist *");
	console.cmdWrong[cmdID] = "ui.getlist";
	console.cmdSyntax[cmdID] = "\"ui.getlist (str)\" - Display the list of ui elements with (filter)";
	console.cmdFunc[cmdID] = cmd_ui_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.enable");
	console.cmdWrong[cmdID] = "editor.enable";
	console.cmdSyntax[cmdID] = "\"editor.enable\" - Enable the editor mode";
	console.cmdFunc[cmdID] = cmd_editor_enable;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.save [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "editor.save";
	console.cmdSyntax[cmdID] = "\"editor.save [str]\" - Save active blueprint as [file]";
	console.cmdFunc[cmdID] = cmd_editor_save;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.load [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "editor.load";
	console.cmdSyntax[cmdID] = "\"editor.load [str]\" - Load [file] as active blueprint";
	console.cmdFunc[cmdID] = cmd_editor_load;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.autogen");
	console.cmdWrong[cmdID] = "editor.autogen";
	console.cmdSyntax[cmdID] = "\"editor.autogen (num) (num)\" - Generate (amount) of blueprints starting numeration at (index)";
	console.cmdFunc[cmdID] = cmd_editor_autogen;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.setgentype [0-9]+");
	console.cmdWrong[cmdID] = "editor.setgentype";
	console.cmdSyntax[cmdID] = "\"editor.setgentype [num]\" - Set [type] as autogen type";
	console.cmdFunc[cmdID] = cmd_editor_setgentype;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^database.unit.getlist *");
	console.cmdWrong[cmdID] = "database.unit.getlist";
	console.cmdSyntax[cmdID] = "\"database.unit.getlist (str)\" - Get unit list from the database with (filter)";
	console.cmdFunc[cmdID] = cmd_database_unit_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^database.item.getlist *");
	console.cmdWrong[cmdID] = "database.item.getlist";
	console.cmdSyntax[cmdID] = "\"database.item.getlist (str)\" - Get item list from the database with (filter)";
	console.cmdFunc[cmdID] = cmd_database_item_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^database.buff.getlist *");
	console.cmdWrong[cmdID] = "database.buff.getlist";
	console.cmdSyntax[cmdID] = "\"database.buff.getlist (str)\" - Get buff list from the database with (filter)";
	console.cmdFunc[cmdID] = cmd_database_buff_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^database.reload");
	console.cmdWrong[cmdID] = "database.reload";
	console.cmdSyntax[cmdID] = "\"database.reload\" - Reload the database";
	console.cmdFunc[cmdID] = cmd_database_reload;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^settings.load");
	console.cmdWrong[cmdID] = "settings.load";
	console.cmdSyntax[cmdID] = "\"settings.load\" - Load the settings from the file";
	console.cmdFunc[cmdID] = cmd_settings_load;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^settings.save");
	console.cmdWrong[cmdID] = "settings.save";
	console.cmdSyntax[cmdID] = "\"settings.save\" - Save the settings to the file";
	console.cmdFunc[cmdID] = cmd_settings_save;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^settings.update");
	console.cmdWrong[cmdID] = "settings.update";
	console.cmdSyntax[cmdID] = "\"settings.update\" - Update the settings file contents";
	console.cmdFunc[cmdID] = cmd_settings_update;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^world.saveto [a-zA-Z0-9.]+ [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "world.saveto";
	console.cmdSyntax[cmdID] = "\"world.saveto [str] [str]\" - Save current world to [savefile] as [world]";
	console.cmdFunc[cmdID] = cmd_world_saveto;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^world.loadfrom [a-zA-Z0-9.]+ [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "world.loadfrom";
	console.cmdSyntax[cmdID] = "\"world.loadfrom [str] [str]\" - Load from [savefile] [world]";
	console.cmdFunc[cmdID] = cmd_world_loadfrom;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^exit");
	console.cmdWrong[cmdID] = "exit";
	console.cmdSyntax[cmdID] = "\"exit\" - Close the game";
	console.cmdFunc[cmdID] = cmd_exit;
	console.cmdServerOnly[cmdID] = false;
}

// Parsing user commands (mostly for debugging)
bool cConsole::parseCommand(string cmd)
{
	bool commandParsed = false;

	regex arg_any(" [a-zA-Z0-9._-]+");						// Regex for any command argument
	int argCounter = 0;										// Amount of found arguments
	string args[] = { "0", "0", "0", "0" };					// Found arguments
	sregex_iterator it(cmd.begin(), cmd.end(), arg_any);	// Regex iterator for cmd
	sregex_iterator it_end;									// Iterator end condition

	// Echo
	if (cmd.substr(0, 4) == "echo" && cmd.length() >= 5) {
		args[argCounter++] = cmd.substr(5);
	}
	// Other
	else
	{
		// Getting arguments
		while (it != it_end && argCounter < 4)
		{
			args[argCounter++] = it->str().substr(1);	// Removing the first character (whitespace)
			++it;
		}
	}

	// Parsing commands
	for (int i = 0; i < LIMIT_CMD; i++)
	{
		// If the command is syntactically correct, call the function
		if (regex_search(cmd, console.cmdRegex[i]) && console.cmdWrong[i].length() > 0)
		{
			commandParsed = true;
			if (!console.cmdServerOnly[i] || !client.connected || core.localServer) {
				console.cmdFunc[i](args);
			}
			else {
				console.echo << "[CMD] Only server can use this command!" << "\n";
			}
		}
	}

	// Incorrect commands
	if (!commandParsed)
	{
		for (int i = 0; i < LIMIT_CMD; i++)
		{
			// If the command itself is correct, display correct argument syntax
			if (cmd.substr(0, console.cmdWrong[i].length()) == console.cmdWrong[i] && console.cmdWrong[i].length() > 0)
			{
				commandParsed = true;
				console.echo << "[CMD] " << console.cmdSyntax[i] << "\n";
			}
		}
	}
	
	// Unknown command
	if (!commandParsed)
	{
		console.echo << "[CMD] Can not parse the command!" << "\n";
	}

	return true;
}