
#include "main.h"
#include "script.h"
#include "UI.h"
#include "math.h"
#include "chat.h"

#define WORK_RANDOM		0
#define WORK_MINER		1
#define WORK_CUTTER		2

int tempGold = 0;
int tempLumber = 0;
int humanTotal = 10;
int humanLimit = 20;
int miners = 0;
int woodcutters = 0;

int upgGoldmine = 0;
int upgLumbermill = 0;

int getFreePeople() {
	return humanTotal - miners - woodcutters;
}

void killHuman(int count, int type) {
	int humansKilled = 0;
	if (type == WORK_RANDOM) {
		do
		{
			float val = math.rand(0, 2);
			if (val == 0 && miners > 0) { humanTotal -= 1; miners -= 1; humansKilled += 1; }
			else if (val == 1 && woodcutters > 0) { humanTotal -= 1; woodcutters -= 1; humansKilled += 1; }
			else if (val == 2 && getFreePeople() > 0) { humanTotal -= 1; humansKilled += 1; }
		} while (humansKilled < count && humanTotal > 0);
	}
	else if (type == WORK_MINER) {
		humansKilled = min(count, miners);
		miners -= humansKilled;
		humanTotal -= humansKilled;
	}
	else if (type == WORK_CUTTER) {
		humansKilled = min(count, woodcutters);
		woodcutters -= humansKilled;
		humanTotal -= humansKilled;
	}
}


void cScript::civ_callback(cUIButton* btn, int parent)
{
	if (btn->action == "civ_endTurn") { civ_endTurn(0); }
	else if (btn->action == "civ_addMiner") { civ_addMiner(0); }
	else if (btn->action == "civ_remMiner") { civ_remMiner(0); }
	else if (btn->action == "civ_addCutter") { civ_addCutter(0); }
	else if (btn->action == "civ_remCutter") { civ_remCutter(0); }
	else if (btn->action == "civ_addHuman") { civ_addHuman(1, 0); }
	else if (btn->action == "civ_buildHouse") { civ_buildHouse(0); }
	script.ui_showMainScreen(0);
}

void cScript::civ_endTurn(cArg args)
{
	tempGold += 5 * miners;
	tempLumber += 5 * woodcutters;

	// New children
	int freePeople = getFreePeople();
	if (freePeople >= 2) {
		int count = math.round(freePeople * math.randf(0.05f, 0.20f));
		if (count > 0) {
			chat.log << "Children in the village: +" << count << endl;
			humanTotal += count;
			if (humanTotal > humanLimit) {
				int dif = humanTotal - humanLimit;
				humanTotal = humanLimit;
				chat.log << "WE HAVE KILLED " << dif << " CHILDREN" << endl;
			}
		}
	}
	// Something bad
	if (math.randf(0.00f, 1.00f) < 0.20f) {
		int dead;
		float val = math.randf(0.00f, 1.00f);
		// Plague
		if (val <= 0.10f) {
			dead = humanTotal * math.randf(0.10f, 0.30f);
			killHuman(dead, WORK_RANDOM);
			chat.log << "The plague has taken " << dead << " lives..." << endl;
		}
		// Rockfall
		else if (val <= 0.40f) {
			dead = max(1, min((int)(miners * math.randf(0.10f, 0.20f) + math.rand(1, 5)), miners));
			killHuman(dead, WORK_MINER);
			if (miners > 0) { chat.log << "There was a big rockfall in the mines! We have lost " << dead << " lives!" << endl; }
			else { chat.log << "There was a big rockfall in the mines! Thankfully, there was nobody in the mines." << endl; }
		}
		// Small raid
		else if (val <= 0.70f && humanTotal > 25) {
			dead = math.rand(0, humanTotal - 10);
			killHuman(dead, WORK_RANDOM);
			chat.log << "Some idiots just raided a village. We have lost " << dead << " good guys." << endl;
		}
		// Big raid
		else if (humanTotal > 100) {
			dead = math.rand(math.rand(3, 15), math.rand(humanTotal - 10, humanTotal));
			killHuman(dead, WORK_RANDOM);
			chat.log << "That was a huge attack. We have lost " << dead << " people." << endl;
		}
	}
}

void cScript::ui_showMainScreen(cArg args)
{
	ui.removeElementsByRef(REF_UI_CIV);

	int gold = tempGold;
	int lumber = tempLumber;

	// Information window
	vec2f winPos = vec2f(0, 0);
	string text = "Gold: " + to_string(gold);
	ui.createText(winPos + vec2f(0.00, 0.00), text, "", REF_UI_CIV);
	text = "Lumber: " + to_string(lumber);
	ui.createText(winPos + vec2f(0.00f, 16.00), text, "", REF_UI_CIV);
	text = "Population: " + to_string(humanTotal) + " (" + to_string(getFreePeople()) + ")" + " / " + to_string(humanLimit);
	ui.createText(winPos + vec2f(0.00f, 32.00), text, "", REF_UI_CIV);

	
	// Population control window
	winPos = vec2f(100, 170);
	text = "Working:";
	ui.createText(winPos + vec2f(10, 0), text, "", REF_UI_CIV);

	text = "Mine: " + to_string(miners) + " / " + to_string(humanTotal);
	ui.createText(winPos + vec2f(0, 20), text, "", REF_UI_CIV);

	ui.addElement("civ_btn", winPos + vec2f(16, 80));
	ui.getLast()->size = vec2f(32, 32);
	ui.getLast()->button.action = "civ_addMiner";
	ui.getLast()->setText("+");
	ui.getLast()->textFont = FONT_DESCR;

	ui.addElement(*ui.getLast(), winPos + vec2f(50, 80));
	ui.getLast()->button.action = "civ_remMiner";
	ui.getLast()->setText("-");
	ui.getLast()->textFont = FONT_DESCR;

	text = "Lumbermill: " + to_string(woodcutters) + " / " + to_string(humanTotal);
	ui.createText(winPos + vec2f(300, 20), text, "", REF_UI_CIV);

	ui.addElement("civ_btn", winPos + vec2f(316, 80));
	ui.getLast()->size = vec2f(32, 32);
	ui.getLast()->button.action = "civ_addCutter";
	ui.getLast()->setText("+");
	ui.getLast()->textFont = FONT_DESCR;

	ui.addElement(*ui.getLast(), winPos + vec2f(350, 80));
	ui.getLast()->button.action = "civ_remCutter";
	ui.getLast()->setText("-");
	ui.getLast()->textFont = FONT_DESCR;

	// Building buttons
	winPos = vec2f(0, 700);
	ui.addElement("civ_btn", winPos + vec2f(64, 32));
	ui.getLast()->button.action = "civ_buildHouse";
	ui.getLast()->tooltip.setText("Human limit +20, costs 100/50");
	ui.getLast()->setText("Build House");

	// Final buttons window
	winPos = vec2f(100, 300);
	ui.addElement("civ_btn", winPos + vec2f(0.00f, 0.00f));
	ui.getLast()->button.action = "civ_endTurn";
	ui.getLast()->setText("End Turn");

	/*ui.addElement("civ_btn", vec2f(100, 200));
	ui.getLast()->button.action = "civ_addHuman";
	ui.getLast()->setText("Create Human");*/


}

void cScript::civ_addMiner(cArg args)
{
	if (getFreePeople() > 0) {
		miners += max(1, getFreePeople() / 10);
	}
}

void cScript::civ_remMiner(cArg args)
{
	if (miners > 0) {
		miners -= 1;
	}
}

void cScript::civ_addCutter(cArg args)
{
	if (getFreePeople() > 0) {
		woodcutters += max(1, getFreePeople() / 10);
	}
}

void cScript::civ_remCutter(cArg args)
{
	if (woodcutters > 0) {
		woodcutters -= 1;
	}
}

void cScript::civ_addHuman(int amount, cArg args)
{
	humanTotal += amount;
	if (humanTotal > humanLimit)
	{
		humanTotal = humanLimit;
		console << "You have reached the limit";
	}
}

void cScript::civ_buildHouse(cArg args)
{
	if (tempGold >= 100 && tempLumber >= 50)
	{
		tempGold -= 100;
		tempLumber -= 50;
		humanLimit += 20;
	}
}