
#include "main.h"
#include "UI.h"
#include "game.h"
#include "camera.h"
#include "visual.h"

void cUI::openInventory()
{
	//game.pause();
	access.lock();
	int id;
	ui.invOpened = true;
	sf::Vector2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_GAMEPLAY, 0.10f);

	// Opening full page
	if (ui.inventoryPage == INV_FULL)
	{
		// Background
		id = ui.addElement("image", center);
		ui.element[ui.getElementId(id)].size = sf::Vector2f(camera.res.x - 30, camera.res.y - 30);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_FULL_BG);
		ui.element[ui.getElementId(id)].button.action = "-";
		ui.element[ui.getElementId(id)].alpha = 100.00f * game.ambientLight / 255.00f + 100.00f;
		// Close button
		id = ui.addElement("button", sf::Vector2f(camera.res.x - 15 - 12 - 5, 15 + 12 + 5));
		ui.element[ui.getElementId(id)].size = sf::Vector2f(24, 24);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_close_white.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_close_orange.png");
		ui.element[ui.getElementId(id)].button.action = "invToggle";
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_BUTTON);
		// Title
		id = ui.createText(sf::Vector2f(70.00f, 40.00f), "Inventory", "", REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].textSize = 30;
		ui.element[ui.getElementId(id)].tooltip.setText("Are you sure you need a tooltip here?");
		ui.element[ui.getElementId(id)].tooltip.delay = 1.00f;
		ui.element[ui.getElementId(id)].tooltip.offset.y = -5.00f;
		// Top line
		ui.createLine(sf::Vector2f(35.00f, 80.00f), sf::Vector2f(camera.res.x - 35.00f, 80.00f), REF_UI_INVENTORY);

		// Item list categories
			// All
		id = ui.addElement("button", sf::Vector2f(550.00f * resModX + 30.00f, 180.00f));
		ui.element[ui.getElementId(id)].size = sf::Vector2f(40, 40);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_empty_white.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_empty_orange.png");
		ui.element[ui.getElementId(id)].button.action = "category_all";
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].tooltip.setText("All items");
		ui.element[ui.getElementId(id)].tooltip.offset.x = 12.00f;
		ui.element[ui.getElementId(id)].tooltip.offset.y = -6.00f;
		ui.element[ui.getElementId(id)].tooltip.ignoreOrigin = true;
		ui.element[ui.getElementId(id)].priority = 9;
			// Equipped
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 225.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Equipped");
		ui.element[ui.getElementId(id)].button.action = "category_equipped";
			// Weapons and tools
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 285.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Weapons / Tools");
		ui.element[ui.getElementId(id)].button.action = "category_weapons";
			// Clothes
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 330.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Armor / Clothes");
		ui.element[ui.getElementId(id)].button.action = "category_armor";
			// Materials
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 375.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Raw materials");
		ui.element[ui.getElementId(id)].button.action = "category_materials";
			// Consumables
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 420.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Consumables");
		ui.element[ui.getElementId(id)].button.action = "category_consumables";
			// Other
		id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(550.00f * resModX + 30.00f, 465.00f));
		ui.element[ui.getElementId(id)].tooltip.setText("Other");
		ui.element[ui.getElementId(id)].button.action = "category_other";

		// Item list border
		ui.createBackground(sf::Vector2f(60.00f, 160.00f), sf::Vector2f(550.00f * resModX, 750.00f * resModY), 150, REF_UI_INVENTORY);
		ui.createBorder(sf::Vector2f(60.00f, 160.00f), sf::Vector2f(550.00f * resModX, 750.00f * resModY), REF_UI_INVENTORY);
		// Item list line
		ui.createLine(sf::Vector2f(80.00f, 220.00f), sf::Vector2f(530.00f * resModX, 220.00f * resModY), REF_UI_INVENTORY);
		// Item list test str
		//ui.createText(sf::Vector2f(60.00f + 320.00f * resModX, 180.00f), "ABC", "Item weight", REF_UI_INVENTORY);
		// Item list weight str
		id = ui.createText(sf::Vector2f(60.00f + 420.00f * resModX, 180.00f), "W", "Item weight", REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].priority = 11;

		// Item description border
		ui.createText(sf::Vector2f(720.00f * resModX, 125.00f), "Item description", "", REF_UI_INVENTORY);
		ui.createBackground(sf::Vector2f(700.00f * resModX, 160.00f), sf::Vector2f(1200.00f * resModX, 450.00f * resModY), 150, REF_UI_INVENTORY);
		ui.createBorder(sf::Vector2f(700.00f * resModX, 160.00f), sf::Vector2f(1200.00f * resModX, 450.00f * resModY), REF_UI_INVENTORY);
		// Craft border
		ui.createText(sf::Vector2f(720.00f * resModX, 500.00f * resModY - 35.00f), "Crafting", "Click on items to add them to crafting menu", REF_UI_INVENTORY);
		ui.createBackground(sf::Vector2f(700.00f * resModX, 500.00f * resModY), sf::Vector2f(1200.00f * resModX, 750.00f * resModY), 150, REF_UI_INVENTORY);
		ui.createBorder(sf::Vector2f(700.00f * resModX, 500.00f * resModY), sf::Vector2f(1200.00f * resModX, 750.00f * resModY), REF_UI_INVENTORY);
		// Craft line
		ui.createLine(sf::Vector2f(720.00f * resModX, 750.00f * resModY - 60.00f), sf::Vector2f(1180.00f * resModX, 750.00f * resModY - 60.00f), REF_UI_INVENTORY);

		// Item list
		ui.updateInterfaceItemList();
		ui.updateInterfaceItemDescr();
	}
	ui.setFadeTimeByRef(REF_UI_INVENTORY, 0.10f, FADE_IN);
	access.unlock();
}

void cUI::closeInventory()
{
	access.lock();
	ui.invOpened = false;

	ui.removeElementsByRef(REF_UI_INVENTORY, 0.10f);
	ui.updateFull();
	access.unlock();
	//game.resume();
}