
#pragma once
#include "precomp.h"
#include "define.h"
#include "util.h"

class cSubConsole
{
public:
	int type;
};

class cConsole
{
public:
	cMutex access;

	bool online;
	bool displayed;
	int displayedPage;
	int scrollOffset;
	int cmdHistoryPos;

	bool parseCommand(std::string cmd);

	bool cmdServerOnly[LIMIT_CMD];
	std::regex cmdRegex[LIMIT_CMD];
	std::string cmdWrong[LIMIT_CMD];
	std::string cmdSyntax[LIMIT_CMD];
	std::function<void(std::string[])> cmdFunc[LIMIT_CMD];

	cSubConsole info;
	cSubConsole echo;
	cSubConsole error;
	cSubConsole debug;
	std::string input;
	sf::String inputDisplay;
	std::string waitingQueue;
	std::vector<sf::String> history[5];
	std::vector<std::string> cmdHistory;

	cConsole();
	void show();
	void hide();
	void toggle();
	int getLineCount();
	void scroll(int dir);
	void resetScroll();
	void addToInput(sf::String str);
	void removeLastFromInput();
	void flushInput();
	void clearInput();
	void scrollHistory(int dir);
	void clear();

	void output(std::string str, int subConsole = SUBCMD_INFO);
};

extern cConsole console;

cConsole& operator << (cConsole& cmd, std::string str);
cConsole& operator << (cConsole& cmd, int i);
cConsole& operator << (cConsole& cmd, float f);
cSubConsole& operator << (cSubConsole& cmd, std::string str);
cSubConsole& operator << (cSubConsole& cmd, int i);
cSubConsole& operator << (cSubConsole& cmd, float f);

void cmd_help(std::string args[]);
void cmd_say(std::string args[]);
void cmd_echo(std::string args[]);
void cmd_clear(std::string args[]);
void cmd_macro(std::string args[]);
void cmd_unit_add(std::string args[]);
void cmd_unit_moveto(std::string args[]);
void cmd_unit_setspeed(std::string args[]);
void cmd_unit_remove(std::string args[]);
void cmd_unit_getlist(std::string args[]);
void cmd_unit_order_moveto(std::string args[]);
void cmd_unit_order_getlist(std::string args[]);
void cmd_unit_item_add(std::string args[]);
void cmd_unit_item_remove(std::string args[]);
void cmd_unit_item_getlist(std::string args[]);
void cmd_unit_buff_add(std::string args[]);
void cmd_unit_buff_remove(std::string args[]);
void cmd_unit_buff_getlist(std::string args[]);
void cmd_camera_attach(std::string args[]);
void cmd_camera_lock(std::string args[]);
void cmd_game_timemod(std::string args[]);
void cmd_game_settime(std::string args[]);
void cmd_game_setlight(std::string args[]);
void cmd_weather_set(std::string args[]);
void cmd_weather_getlist(std::string args[]);
void cmd_client_connect(std::string args[]);
void cmd_client_disconnect(std::string args[]);
void cmd_server_open(std::string args[]);
void cmd_player_kick(std::string args[]);
void cmd_player_setunit(std::string args[]);
void cmd_player_getlist(std::string args[]);
void cmd_ui_getlist(std::string args[]);
void cmd_editor_enable(std::string args[]);
void cmd_editor_save(std::string args[]);
void cmd_editor_load(std::string args[]);
void cmd_editor_autogen(std::string args[]);
void cmd_editor_setgentype(std::string args[]);
void cmd_database_unit_getlist(std::string args[]);
void cmd_database_item_getlist(std::string args[]);
void cmd_database_buff_getlist(std::string args[]);
void cmd_database_reload(std::string args[]);
void cmd_settings_load(std::string args[]);
void cmd_settings_save(std::string args[]);
void cmd_settings_update(std::string args[]);
void cmd_world_saveto(std::string args[]);
void cmd_world_loadfrom(std::string args[]);
void cmd_exit(std::string args[]);