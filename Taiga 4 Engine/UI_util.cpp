
#include "main.h"
#include "UI.h"
#include "visual.h"
#include "math.h"

//sf::Text brushText;

void cUI::createLine(sf::Vector2f begin, sf::Vector2f end, int ref, int priority)
{
	// TODO: Get those values from texture data
	float endTexSizeX = 3.00f;
	float endTexSizeY = 5.00f;

	int id = ui.addElement("image", sf::Vector2f(begin.x, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_line_left.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(endTexSizeX, endTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(end.x - 3.00f, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_line_right.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(endTexSizeX, endTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(begin.x + 3.00f, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_line_middle.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(end.x - begin.x - endTexSizeX * 2.00f, endTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }
}

void cUI::createBorder(sf::Vector2f begin, sf::Vector2f end, int ref, int priority)
{
	// TODO: Get those values from texture data
	float cornerTexSizeX = 4.00f;
	float cornerTexSizeY = 4.00f;

	float offsetX = end.x - begin.x;
	float offsetY = end.y - begin.y;
	begin.x = math.round(begin.x);
	begin.y = math.round(begin.y);
	end.x = begin.x + offsetX;
	end.y = begin.y + offsetY;

	int id = ui.addElement("image", sf::Vector2f(begin.x, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_corner_lt.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(begin.x + cornerTexSizeX, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_top.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(end.x - begin.x - cornerTexSizeX * 2.00f, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(end.x - cornerTexSizeX, begin.y));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_corner_rt.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(end.x - cornerTexSizeX, begin.y + cornerTexSizeY));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_right.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, end.y - begin.y - cornerTexSizeY * 2.00f);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(end.x - cornerTexSizeX, end.y - cornerTexSizeY));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_corner_rb.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(begin.x + cornerTexSizeX, end.y - cornerTexSizeY));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_bottom.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(end.x - begin.x - cornerTexSizeX * 2.00f, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(begin.x, end.y - cornerTexSizeY));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_corner_lb.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, cornerTexSizeY);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }

	id = ui.addElement("image", sf::Vector2f(begin.x, begin.y + cornerTexSizeY));
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_border_left.png");
	ui.element[ui.getElementId(id)].size = sf::Vector2f(cornerTexSizeX, end.y - begin.y - cornerTexSizeY * 2.00f);
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = priority;
	if (ref != -1) { ui.element[ui.getElementId(id)].addRef(ref); }
}

int cUI::createText(sf::Vector2f pos, string str, string tip, int ref)
{
	sf::Text brushText;
	sf::FloatRect floatRect;
	int id = ui.addElement("text", sf::Vector2f(math.round(pos.x), math.round(pos.y)));
	ui.element[ui.getElementId(id)].setText(str);
	ui.element[ui.getElementId(id)].textSize = 24;
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	ui.element[ui.getElementId(id)].textColor = sf::Color(255, 255, 255);
	brushText.setFont(visual.fontMain);
	brushText.setString(str);
	brushText.setCharacterSize(24);
	floatRect = brushText.getGlobalBounds();
	ui.element[ui.getElementId(id)].size = sf::Vector2f(floatRect.width, floatRect.height);
	if (tip.length() > 0) {
		ui.element[ui.getElementId(id)].tooltip.setText(tip);
		ui.element[ui.getElementId(id)].tooltip.delay = 0.50f;
		ui.element[ui.getElementId(id)].tooltip.ignoreOrigin = true;
		ui.element[ui.getElementId(id)].tooltip.offset.x = -15.00f;
		ui.element[ui.getElementId(id)].tooltip.offset.y = -15.00f;
	}
	if (ref != -1) {
		ui.element[ui.getElementId(id)].addRef(ref);
	}
	ui.element[ui.getElementId(id)].textRect = floatRect;
	return id;
}

int cUI::createBackground(sf::Vector2f begin, sf::Vector2f end, int alpha, int ref)
{
	int id = ui.addElement("image", begin);
	ui.element[ui.getElementId(id)].size = sf::Vector2f(end.x - begin.x, end.y - begin.y);
	ui.element[ui.getElementId(id)].alpha = alpha;
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	if (ref != -1) {
		ui.element[ui.getElementId(id)].addRef(ref);
	}
	ui.element[ui.getElementId(id)].ignoreOrigin = true;
	return id;
}

void cUIElement::setFadeTimer(float time, int type)
{
	fadeType = type;
	fadeTimerMax = time;
	if (type == FADE_IN) { fadeTimer = 0.00f; }
	else if (type == FADE_OUT) { fadeTimer = time; }
}

void cUIElement::resetFadeTimer()
{
	fadeType = FADE_STOP;
	fadeTimer = 0.00f;
	fadeTimerMax = 0.00f;
}

float cUIElement::getFade()
{
	if (fadeTimerMax <= 0.00f) { return 1.00f; }
	if (fadeTimer <= 0.00f) { return 0.00f; }
	return fadeTimer / fadeTimerMax;
}

void cUI::setFadeTimeByRef(int ref, float time, int type)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].hasRef(ref))
		{
			element[i].setFadeTimer(time, type);
		}
	}
}

void cUI::toggleDisplay()
{
	displayed = !displayed;
}