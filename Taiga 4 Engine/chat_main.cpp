
#include "chat.h"
#include "main.h"
#include "UI.h"
#include "client.h"
#include "settings.h"

cChat::cChat()
{
	main.type = CHATTAB_ALL;
	players.type = CHATTAB_PLAYERS;
	log.type = CHATTAB_LOG;

	inFocus = false;
	displayed = false;
}

void cChat::output(std::string str, int chatTab)
{
	string backup;

	waitingQueue += str;
	if (str.length() >= 1 && str.substr(str.length() - 1) == "\n")
	{
		backup = waitingQueue;
		// Adding time string
		if (waitingQueue.length() > 1) {
			waitingQueue = util.getCurrentTimeString() + " - " + waitingQueue;
		}

		// Flushing to file
		ofstream file;
		if (!core.serverMode) { file.open("Logs//chat.txt", ios::app); }
		file << waitingQueue;
		file.close();

		// Returning to backup if no timestamp is needed
		if (!settings.enableChatTimestamps) {
			waitingQueue = backup;
		}

		// Flushing
		int timer = timeGetTime();
		history[CHATTAB_ALL].push_back(sf::String(waitingQueue, locale("russian")));
		historyTimer[CHATTAB_ALL].push_back(timer);
		history[chatTab].push_back(sf::String(waitingQueue, locale("russian")));
		historyTimer[chatTab].push_back(timer);
		waitingQueue.clear();
		if (scrollOffset != 0 && (int)history[CHATTAB_ALL].size() > getLineCount())
		{
			scrollOffset += 1;
		}

		// Update UI
		//if (displayed) { ui.wndChat.update(); }
		//else { show(false); }
		ui.updateChatWindow();
	}
}

void cChat::addToInput(sf::String str)
{
	input.append(str.toAnsiString(locale("russian")));
	updateInputDisplay();
}

void cChat::removeLastFromInput()
{
	if (input.length() == 0) { return; }
	input.erase(input.length() - 1);
	updateInputDisplay();
}

void cChat::flushInput()
{
	if (input.length() == 0)
	{
		//noFocusTimer = 2.00f;
		chat.hide();
		//ui.wndChat.update();
	}
	else
	{
		chat.scrollOffset = 0;
		//chat.players << "> " << input << endl;

		sf::Packet data;
		data << MSG_PLAYER_CHAT << input;
		client.sendPacket(data);
		data.clear();

		noFocusTimer = value.chatNoFocusTimer;
		inFocus = false;
		tempDisplay = true;
		ui.wndChat.open();
	}

	//parseCommand(input);

	clearInput();
}

void cChat::clearInput()
{
	input.clear();
	inputDisplay.clear();
	updateInputDisplay();
}

void cChat::updateInputDisplay()
{
	inputDisplay = "[" + client.playerName + "]: " + input + "_";
}