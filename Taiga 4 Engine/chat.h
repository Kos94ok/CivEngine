
#include "precomp.h"
#include "define.h"
#include "util.h"

#define LOGMSG_ITEM_ADD					0
#define LOGMSG_ITEM_REMOVE				1
#define LOGMSG_SCREENSHOT				2
#define LOGMSG_RESOURCE_ADD				3
#define LOGMSG_RESOURCE_REMOVE			4
#define LOGMSG_PROGRESS_HARVEST			20
#define LOGMSG_PROGRESS_PACK			21

class cChatTab
{
public:
	int type;
};

class cChat
{
public:
	bool inFocus;
	bool displayed;
	bool tempDisplay;
	int displayedPage;
	int scrollOffset;

	float noFocusTimer;

	cChatTab main;
	cChatTab players;
	cChatTab log;
	std::string input;
	sf::String inputDisplay;
	std::string waitingQueue;
	std::vector<sf::String> history[3];
	std::vector<int> historyTimer[3];

	cChat();
	void show(bool focus);
	void hide();
	void toggle();
	void scroll(int dir);
	void resetScroll();
	void addToInput(sf::String str);
	void removeLastFromInput();
	void flushInput();
	void updateInputDisplay();
	int getLineCount();
	sf::Vector2f getPos();
	sf::Vector2f getSize();
	void clearInput();
	void clear();

	void logMessage(int message, cArg args);

	void output(std::string str, int chatTab = CHATTAB_LOG);
};

extern cChat chat;

cChat& operator << (cChat& target, std::string str);
cChat& operator << (cChat& target, int i);
cChat& operator << (cChat& target, float f);
cChatTab& operator << (cChatTab& target, std::string str);
cChatTab& operator << (cChatTab& target, int i);
cChatTab& operator << (cChatTab& target, float f);