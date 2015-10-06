
#pragma once
#include "precomp.h"
#include "define.h"
#include "refSystem.h"
#include "util.h"

class cUIButton
{
public:
	std::string action;
	std::string args[8];

	void callbackLeft(int parent);
	void callbackRight(int parent);

	cUIButton() {
		action = "";
	}
};

class cUITooltip
{
public:
	float delay;
	sf::Vector2f offset;
	std::string text;
	sf::String textDisplay;
	float alpha;
	bool ignoreOrigin;
	sf::Vector2f pos;

	void display();
	bool hasText();
	void setText(std::string txt);
	
	cUITooltip() {
		text = "";
		delay = 0.00f;
		alpha = 0.00f;
		ignoreOrigin = false;
		offset = sf::Vector2f(0.00f, 0.00f);
	}
};

class cUIBar
{
public:
	bool vertical;
	int texture;
	float minOffset;
	float value;
	float maxValue;
	float maxOffset;

	cUIBar() {
		vertical = false;
		texture = -1;

		minOffset = 0.00f;
		value = 0.00f;
		maxValue = 1.00f;
		maxOffset = 0.00f;
	}
};

class cUIElement : public cReference
{
public:
	bool isValid;
	int globalId;
	int priority;
	string type;
	bool ignoreOrigin;
	sf::Vector2f pos;
	sf::Vector2f size;
	int textSize;
	sf::Vector2f textOffset;
	sf::Color textColor;
	sf::Color textColorHover;
	string text;
	sf::String textDisplay;
	float textOutlineThickness;
	sf::Color textOutlineColor;
	int textFont;
	sf::Color bodyColor;
	sf::Color hoverColor;
	int alpha;
	int hoverAlpha;
	bool isHovered;
	int texture;
	int textureHovered;
	cUIButton button;
	cUITooltip tooltip;
	cUIBar bar;
	sf::FloatRect textRect;

	bool hasText();
	void setText(string txt);

	int fadeType;
	float fadeTimer;
	float fadeTimerMax;
	void setFadeTimer(float time, int type);
	void resetFadeTimer();
	float getFade();

	cUIElement() {
		isValid = false;
		size = sf::Vector2f(20, 20);
		ignoreOrigin = false;
		priority = 5;

		fadeType = FADE_STOP;
		fadeTimer = 0.00f;
		fadeTimerMax = 0.00f;

		refCounter = 0;
		for (int i = 0; i < LIMIT_REFERENCE; i++) { ref[i] = false; }

		alpha = 255;
		textSize = 16;
		textColor = sf::Color(200, 200, 200, alpha);
		textColorHover = textColor;
		textFont = FONT_MAIN;
		textOffset = sf::Vector2f(0.00f, 0.00f);
		textOutlineThickness = 0.00f;

		bodyColor = sf::Color(255, 255, 255);
		hoverColor = sf::Color(255, 255, 255);

		isHovered = false;
		hoverAlpha = 0;
		texture = -1;
		textureHovered = -1;
	}
};

class cUIChat
{
public:
	void open();
	void close();
	void update();
	void updateInput();
};

class cUIOverworld
{
public:
	bool isDisplayed = false;

	void open();
	void close();
	void update();
};

class cUI
{
public:
	bool displayed;

	int mouseStateLMB;
	int mouseStateRMB;
	float mouseNotMoving;
	int lastCalledElement;
	int lastCalledElementId;
	int lastCreatedElement;

	cUIElement element[LIMIT_UI_ELEMENTS];
	int elementGlobalCounter;

	float clickTimer;
	int hoverHashSum;
	sf::Vector2i mouseLastPos;

	// Elements
	int addElement(string type, sf::Vector2f pos);
	int addElement(cUIElement elem, sf::Vector2f pos);
	int findByRef(int ref);
	void removeElement(int id, float time = 0.00f);
	void removeElementsByRef(int ref, float time = 0.00f);
	void setFadeTimeByRef(int ref, float time, int type);
	int getElementId(int id);
	cUIElement* getLast();

	// Update
	cMutex access;
		// Full clear and initialization
	void updateFull();
		// Update timers and values
	void updateInterface();
		// Clear and rebuild item description
	void updateInterfaceItemDescr();
		// Clear and rebuild item lists
	void updateInterfaceItemList();
		// Clear and rebuild active item buttons
	void updateInterfaceEquipment();
		// Clear and rebuild chat window
	void updateChatWindow();

	// Inventory
	bool invOpened;
	int inventoryPage;
	int inventoryCategory;
	void openInventory();
	void closeInventory();
	// Chat
	cUIChat wndChat;
	// Overworld
	cUIOverworld wndOverworld;

	// Util
	void createLine(sf::Vector2f begin, sf::Vector2f end, int ref = -1, int priority = 5);
	void createBorder(sf::Vector2f begin, sf::Vector2f end, int ref = -1, int priority = 5);
	int createText(sf::Vector2f pos, std::string str, std::string tip = "", int ref = -1);
	int createBackground(sf::Vector2f begin, sf::Vector2f end, int alpha = 255, int ref = -1);

	// Context menu
	int contextMenuTarget;
	int contextMenuTargetId;
	void createContextMenuForItem(int targetItem);
	void createContextMenuForCraftItem(int targetItem);
	void createContextMenuForCraftResult(int targetItem);
	void createContextMenuForGround();
	void createContextMenuForUnit(int targetUnit);
	void clearContextMenu();

	// Other
	void toggleDisplay();

	cUI() {
		displayed = true;
		lastCalledElementId = -1;
		clickTimer = 0.00f;
		invOpened = false;
		inventoryPage = INV_FULL;
		inventoryCategory = CATEGORY_ALL;
		mouseStateLMB = MOUSE_FREE;
		mouseStateRMB = MOUSE_FREE;
		elementGlobalCounter = 0;
		mouseNotMoving = 0.00f;
		contextMenuTarget = CONTEXTMENU_NOTHING;
	}
};

extern cUI ui;