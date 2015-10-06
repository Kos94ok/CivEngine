
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
int miners = 0;
int woodcutters = 0;

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
}


void cScript::civ_callback(cUIButton* btn, int parent)
{
	if (btn->action == "civ_endTurn") { civ_endTurn(0); }
	else if (btn->action == "civ_addMiner") { civ_addMiner(0); }
	else if (btn->action == "civ_remMiner") { civ_remMiner(0); }
	else if (btn->action == "civ_addCutter") { civ_addCutter(0); }
	else if (btn->action == "civ_remCutter") { civ_remCutter(0); }
	else if (btn->action == "civ_addHuman") { civ_addHuman(0); }
	script.ui_showMainScreen(0);
}

void cScript::civ_endTurn(cArg args)
{
	tempGold += 5 * miners;
	tempLumber += 5 * woodcutters;

	// ВСЕ ПЛОХО
	if (math.randf(0.00f, 1.00f) < 0.10f) {
		killHuman(math.rand(1, 3), WORK_RANDOM);
		chat.log << "ВСЕ ПЛОХО!" << endl;
	}
}

void cScript::ui_showMainScreen(cArg args)
{
	ui.removeElementsByRef(REF_UI_CIV);

	int gold = tempGold;
	int lumber = tempLumber;
	string text = "Золото: " + to_string(gold);
	ui.createText(vec2f(0.00f, 0.00), text, "Это как бабло, только золото", REF_UI_CIV);
	text = "Дерево: " + to_string(lumber);
	ui.createText(vec2f(0.00f, 16.00), text, "Деревяшки.", REF_UI_CIV);
	text = "Население: " + to_string(humanTotal) + " (" + to_string(getFreePeople()) + ")";
	ui.createText(vec2f(0.00f, 32.00), text, "ПИПЛЗЫ.", REF_UI_CIV);


	text = "Копатели: " + to_string(miners) + " / " + to_string(humanTotal);
	ui.createText(vec2f(0, 90), text, "", REF_UI_CIV);

	ui.addElement("civ_btn", vec2f(16, 150));
	ui.getLast()->size = vec2f(32, 32);
	ui.getLast()->button.action = "civ_addMiner";
	ui.getLast()->setText("+");
	ui.getLast()->textFont = FONT_DESCR;

	ui.addElement(*ui.getLast(), vec2f(50, 150));
	ui.getLast()->button.action = "civ_remMiner";
	ui.getLast()->setText("-");
	ui.getLast()->textFont = FONT_DESCR;

	text = "Рубители: " + to_string(woodcutters) + " / " + to_string(humanTotal);
	ui.createText(vec2f(300, 90), text, "", REF_UI_CIV);

	ui.addElement("civ_btn", vec2f(316, 150));
	ui.getLast()->size = vec2f(32, 32);
	ui.getLast()->button.action = "civ_addCutter";
	ui.getLast()->setText("+");
	ui.getLast()->textFont = FONT_DESCR;

	ui.addElement(*ui.getLast(), vec2f(350, 150));
	ui.getLast()->button.action = "civ_remCutter";
	ui.getLast()->setText("-");
	ui.getLast()->textFont = FONT_DESCR;


	ui.addElement("civ_btn", vec2f(300.00f, 300.00f));
	ui.getLast()->button.action = "civ_endTurn";
	ui.getLast()->setText("????????? ???");

	ui.addElement("civ_btn", vec2f(500.00f, 300.00f));
	ui.getLast()->button.action = "civ_endTurn";
	ui.getLast()->setText("Новый батон!");

	ui.addElement("civ_btn", vec2f(0, 200));
	ui.getLast()->button.action = "civ_addHuman";
	ui.getLast()->setText("Born human");








}

void cScript::civ_addMiner(cArg args)
{
	if (getFreePeople() > 0) {
		miners += 1;
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
		woodcutters += 1;
	}
}

void cScript::civ_remCutter(cArg args)
{
	if (woodcutters > 0) {
		woodcutters -= 1;
	}
}

void cScript::civ_addHuman(cArg args)
{
	humanTotal++;
}