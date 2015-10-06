
#include "UI.h"
#include "chat.h"
#include "camera.h"
#include "visual.h"
#include "settings.h"

void cUIChat::open()
{
	ui.access.lock();
	int id;
	vec2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT);

	vec2f chatPos = chat.getPos();
	vec2f chatSize = chat.getSize();

	if (chat.inFocus || chat.tempDisplay)
	{
		// Background
		id = ui.addElement("image", chatPos + chatSize / 2.00f);
		ui.element[ui.getElementId(id)].size = chatSize;
		ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
		//ui.element[ui.getElementId(id)].button.action = "-";
		ui.element[ui.getElementId(id)].priority = 2;
		ui.element[ui.getElementId(id)].alpha = 200.00f;
		if (chat.tempDisplay) {
			ui.element[ui.getElementId(id)].alpha = 125.00f;
		}
		// Border
		ui.createBorder(chatPos, chatPos + chatSize, REF_UI_CHAT, 3);
		// Scroll buttons
		if (!chat.tempDisplay)
		{
			id = ui.addElement("button", chatPos + vec2f(chatSize.x - 14.00f, 14.00f));
			ui.element[ui.getElementId(id)].size = vec2f(24.00, 24.00f);
			ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_up_white.png");
			ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_up_orange.png");
			ui.element[ui.getElementId(id)].button.action = "chat_scrollUp";
			ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
			ui.element[ui.getElementId(id)].priority = 3;
			id = ui.addElement("button", chatPos + vec2f(chatSize.x - 14.00f, 40.00f));
			ui.element[ui.getElementId(id)].size = vec2f(24.00, 24.00f);
			ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_down_white.png");
			ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_down_orange.png");
			ui.element[ui.getElementId(id)].button.action = "chat_scrollDown";
			ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
			ui.element[ui.getElementId(id)].priority = 3;
		}
	}

	update();
	updateInput();
	//ui.setFadeTimeByRef(REF_UI_CHAT, 0.10f, FADE_IN);

	ui.access.unlock();
}

void cUIChat::close()
{
	if (chat.inFocus) { ui.removeElementsByRef(REF_UI_CHAT, 0.10f); }
	else { ui.removeElementsByRef(REF_UI_CHAT, 0.10f); }
}

void cUIChat::update()
{
	ui.access.lock();
	int id;
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT_TEXT);

	vec2f chatPos = chat.getPos();
	vec2f chatSize = chat.getSize();

	int curTime = timeGetTime(), elementsDisplayed = 0;
	// Text
	for (int i = max(0, (int)chat.history[chat.displayedPage].size() - chat.getLineCount() - chat.scrollOffset); i < (int)chat.history[chat.displayedPage].size() - max(0, chat.scrollOffset); i++)
	{
		if (chat.inFocus || chat.tempDisplay || curTime - chat.historyTimer[chat.displayedPage][i] < value.chatRecentMessageTimer * 1000.00f)
		{
			id = ui.createText(vec2f(0.00f, 0.00f), chat.history[chat.displayedPage][i], "", REF_UI_CHAT);
			if (chat.inFocus || chat.tempDisplay) {
				ui.element[ui.getElementId(id)].pos = vec2f(chatPos.x + 7.00f, (i - max(0, (int)chat.history[chat.displayedPage].size() - chat.getLineCount()) + chat.scrollOffset) *
					(settings.chatFontSize + settings.chatLineSpacing) + chatPos.y + 3.00f);
				ui.element[ui.getElementId(id)].textColor = color(255, 255, 255);
			}
			else {
				ui.element[ui.getElementId(id)].pos = vec2f(chatPos.x + 7.00f, (elementsDisplayed * (settings.chatFontSize + settings.chatLineSpacing) + chatPos.y + 3.00f));
				ui.element[ui.getElementId(id)].textOutlineThickness = 1.00f;
				ui.element[ui.getElementId(id)].textOutlineColor = color(0, 0, 0);
				ui.element[ui.getElementId(id)].textColor = color(255, 250, 50);
			}
			ui.element[ui.getElementId(id)].setText(chat.history[chat.displayedPage][i].toAnsiString(locale("russian")));
			ui.element[ui.getElementId(id)].textFont = FONT_DESCR;
			ui.element[ui.getElementId(id)].textSize = settings.chatFontSize;
			ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT_TEXT);
			ui.element[ui.getElementId(id)].button.action = "-";
			ui.element[ui.getElementId(id)].priority = 4;
			elementsDisplayed += 1;
		}
	}

	ui.access.unlock();
}

void cUIChat::updateInput()
{
	ui.access.lock();

	ui.removeElementsByRef(REF_UI_CHAT_INPUT);

	if (chat.inFocus)
	{
		vec2f chatPos = chat.getPos();
		vec2f chatSize = chat.getSize();
		int id = ui.createText(vec2f(chatPos.x + 7.00f, chatPos.y + chatSize.y - settings.chatFontSize - 8.00f), chat.inputDisplay, "", REF_UI_CHAT);
		ui.element[ui.getElementId(id)].textFont = FONT_DESCR;
		ui.element[ui.getElementId(id)].textSize = settings.chatFontSize;
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT_TEXT);
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT_INPUT);
		ui.element[ui.getElementId(id)].button.action = "-";
		ui.element[ui.getElementId(id)].priority = 4;
	}

	ui.access.unlock();
}