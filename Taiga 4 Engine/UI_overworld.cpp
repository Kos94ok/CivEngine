
#include "UI.h"
#include "overworld.h"
#include "camera.h"
#include "visual.h"
#include "game.h"

void cUIOverworld::open()
{
	game.pause();
	ui.access.lock();
	int id;
	vec2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;
	isDisplayed = true;

	//ui.addElement
	ui.removeElementsByRef(REF_UI_OVERWORLD);
	ui.removeElementsByRef(REF_UI_GAMEPLAY, 0.10f);

	// Background
	id = ui.addElement("image", center);
	ui.element[ui.getElementId(id)].size = sf::Vector2f(camera.res.x - 300, camera.res.y - 300);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD);
	ui.element[ui.getElementId(id)].button.action = "-";
	ui.element[ui.getElementId(id)].alpha = 100.00f * game.ambientLight / 255.00f + 100.00f;
	// Close button
	id = ui.addElement("button", sf::Vector2f(camera.res.x - 15 - 12 - 5 - 145, 15 + 12 + 5 + 145));
	ui.element[ui.getElementId(id)].size = sf::Vector2f(24, 24);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_close_white.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_close_orange.png");
	ui.element[ui.getElementId(id)].button.action = "overworldToggle";
	ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD);
	ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD_BUTTON);

	// Moveto button
	id = ui.addElement("button", sf::Vector2f(camera.res.x - 15 - 5 - 145, 15 + 5 + 145 + 50.00f));
	ui.element[ui.getElementId(id)].size = sf::Vector2f(96, 48);
	ui.element[ui.getElementId(id)].setText("Move to");
	ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].button.action = "overworldMoveto";
	ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD);
	ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD_BUTTON);

	// Overworld maps
	update();

	ui.access.unlock();
}

void cUIOverworld::close()
{
	isDisplayed = false;
	ui.removeElementsByRef(REF_UI_OVERWORLD, 0.10f);
	ui.updateFull();
	game.resume();
}

void cUIOverworld::update()
{
	int id;
	ui.removeElementsByRef(REF_UI_OVERWORLD_MAP);
	vec2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	vec2f mainOffset = overworld.map[overworld.getSelectedMapIndex()].pos;
	for (cOverworldMap entry : overworld.map)
	{
		id = ui.addElement("button", entry.pos + center - mainOffset);
		ui.element[ui.getElementId(id)].size = vec2f(24.00f, 24.00f);
		ui.element[ui.getElementId(id)].setText(entry.name + " (Level " + to_string(entry.level) + ")");
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_roundbtn.png", false, true);
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_roundbtn_hover.png", false, true);
		ui.element[ui.getElementId(id)].textOffset = vec2f(0.00f, 20.00f);
		ui.element[ui.getElementId(id)].textColorHover = color(255, 127, 0);
		ui.element[ui.getElementId(id)].button.action = "selectOverworldMap";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(entry.id);
		ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD);
		ui.element[ui.getElementId(id)].addRef(REF_UI_OVERWORLD_MAP);

		id = ui.addElement(ui.element[ui.getElementId(id)], entry.pos + center - mainOffset);
		ui.element[ui.getElementId(id)].setText("");
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_roundbtn_overlay.png", false, true);
		ui.element[ui.getElementId(id)].textureHovered = -1;
		ui.element[ui.getElementId(id)].bodyColor = color(175, 10, 10);
		if (entry.id == overworld.activeMap) { ui.element[ui.getElementId(id)].bodyColor = color(10, 10, 175); }
		else if (overworld.isMapAvailable(entry.id)) { ui.element[ui.getElementId(id)].bodyColor = color(10, 175, 10); }
		ui.element[ui.getElementId(id)].hoverColor = ui.element[ui.getElementId(id)].bodyColor;
	}
}