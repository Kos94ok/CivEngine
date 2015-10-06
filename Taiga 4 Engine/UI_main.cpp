
#include "main.h"
#include "UI.h"
#include "database.h"

int cUI::addElement(string type, sf::Vector2f pos)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (!element[i].isValid)
		{
			element[i] = database.getUIElement(type);
			element[i].pos = pos;
			element[i].globalId = elementGlobalCounter++;
			element[i].isValid = true;
			lastCreatedElement = i;
			return elementGlobalCounter - 1;
		}
	}
	return -1;
}

int cUI::addElement(cUIElement elem, sf::Vector2f pos)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (!element[i].isValid)
		{
			element[i] = elem;
			element[i].pos = pos;
			element[i].globalId = elementGlobalCounter++;
			element[i].isValid = true;
			lastCreatedElement = i;
			return elementGlobalCounter - 1;
		}
	}
	return -1;
}

void cUI::removeElement(int id, float time)
{
	id = getElementId(id);
	if (id != -1)
	{
		if (time <= 0.00f) { element[id].isValid = false; element[id].resetFadeTimer(); }
		else { element[id].setFadeTimer(time, FADE_OUT); }
	}
}

void cUI::removeElementsByRef(int ref, float time)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].hasRef(ref))
		{
			if (time <= 0.00f) { element[i].isValid = false; element[i].resetFadeTimer(); }
			else { element[i].setFadeTimer(time, FADE_OUT); }
		}
	}
}

int cUI::getElementId(int id)
{
	if (id == lastCalledElementId) { return lastCalledElement; }
	lastCalledElementId = id;
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].globalId == id) {
			lastCalledElement = i;
			return i;
		}
	}
	lastCalledElement = -1;
	return -1;
}

bool cUIElement::hasText() { return (text.length() > 0); }
bool cUITooltip::hasText() { return (text.length() > 0); }

void cUIElement::setText(string txt)
{
	text = txt;
	textDisplay = sf::String(text, locale("russian"));
}

void cUITooltip::setText(string txt)
{
	text = txt;
	textDisplay = sf::String(text, locale("russian"));
}

int cUI::findByRef(int ref)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].hasRef(ref)) { return i; }
	}
	return -1;
}