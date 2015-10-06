
#include "main.h"
#include "database.h"
#include "visual.h"
#include "math.h"

void cDatabase::init()
{
	// Locking the access
	access.lock();
	// Clearing everything
	database.clear();
	// Loading assets
	console << "[DATABASE] Loading Sounds" << endl;
	loadSounds();
	console << "[DATABASE] Loading Items" << endl;
	loadItems();
	console << "[DATABASE] Loading Units" << endl;
	loadUnits();
	console << "[DATABASE] Loading UI" << endl;
	loadUI();
	console << "[DATABASE] Loading Particles" << endl;
	loadParticles();
	console << "[DATABASE] Loading Textures" << endl;
	loadTextures();
	console << "[DATABASE] Loading Music" << endl;
	loadMusic();
	console << "[DATABASE] Loading External" << endl;
	loadExternal();
	console << "[DATABASE] Main database ready" << endl;
	// Releasing the lock 
	access.unlock();
}

void cDatabase::loadUI()
{
	int i = 0;
	uiElement[i].type = "missingno";
	uiElement[i].size = vec2(20, 20);
	i += 1;

	uiElement[i].type = "icon";
	uiElement[i].addRef(REF_UI_ICON);
	i += 1;

	uiElement[i].type = "button";
	uiElement[i].addRef(REF_UI_BUTTON);
	uiElement[i].size = vec2(128.00f, 32.00f);
	uiElement[i].texture = visual.addTexture("alpha.png");
	uiElement[i].textureHovered = visual.addTexture("ui_btn_hover.png");
	uiElement[i].button.action = "noAction";
	i += 1;

	uiElement[i].type = "text";
	uiElement[i].addRef(REF_UI_TEXT);
	uiElement[i].setText("");
	uiElement[i].size = vec2(0.00f, 0.00f);
	i += 1;

	uiElement[i].type = "image";
	uiElement[i].addRef(REF_UI_IMAGE);
	uiElement[i].textureHovered = -1;
	i += 1;

	uiElement[i].type = "mouseWheelScroll";
	uiElement[i].addRef(REF_UI_MOUSEWHEELSCROLL);
	uiElement[i].texture = visual.addTexture("mouse_progressBG.png");
	uiElement[i].textureHovered = uiElement[i].texture;
	i += 1;

	uiElement[i] = getUIElement("text");
	uiElement[i].type = "civ_text";
	uiElement[i].textColor = color(255, 255, 255);
	uiElement[i].textSize = 12;
	uiElement[i].textFont = FONT_DESCR;
	i += 1;

	uiElement[i].type = "civ_btn";
	uiElement[i].size = vec2(256.00f, 32.00f);
	uiElement[i].setText("NO TEXT");
	uiElement[i].texture = visual.addTexture("black.png");
	uiElement[i].textureHovered = visual.addTexture("black.png");
	uiElement[i].textColor = color(255, 255, 255);
	uiElement[i].textColorHover = color(0, 255, 255);
	uiElement[i].addRef(REF_UI_BUTTON);
	uiElement[i].addRef(REF_UI_CIV);
	uiElement[i].button.action = "noAction";
	i += 1;


	uiElement[i].type = "icon_health";
	uiElement[i].size = vec2(64, 64);
	uiElement[i].tooltip.delay = 0.00f;
	uiElement[i].tooltip.setText("Health");
	uiElement[i].tooltip.offset.y = -20.00f;
	uiElement[i].texture = visual.addTexture("ui_icon_health.png");
	uiElement[i].bar.texture = visual.addTexture("ui_icon_health_full.png");
	uiElement[i].button.action = "-";
	uiElement[i].addRef(REF_UI_GAME);
	i += 1;

	uiElement[i] = database.getUIElement("icon_health");
	uiElement[i].type = "icon_mana";
	uiElement[i].size = vec2(64, 64);
	uiElement[i].tooltip.setText("Mana");
	i += 1;

	uiElement[i].type = "button_test";
	uiElement[i].setText("");
	uiElement[i].size = vec2(128, 32);
	uiElement[i].texture = visual.addTexture("ui_btn.png");
	uiElement[i].textureHovered = visual.addTexture("ui_btn_hover.png");
	uiElement[i].addRef(REF_UI_MAINMENU);
	uiElement[i].button.action = "button_test";
	uiElement[i].tooltip.delay = 0.50f;
	uiElement[i].tooltip.setText("");
	uiElement[i].tooltip.offset.y = -20.00f;
	i += 1;
}

void cDatabase::loadParticles()
{
	int i = 0;
	particle[i].type = "missingno";
	particle[i].size = vec2f(20, 20);
	particle[i].texture = visual.addTexture("black.png");
	i += 1;

	// Weather: Snow
	particle[i].type = "weather_snow";
	particle[i].size = vec2(8, 8);
	particle[i].movementSpeed = 200.00f;
	particle[i].texture = visual.addTexture("particle_snow.png");
	particle[i].addRef(REF_PARTICLE_ONSCREEN);
	i += 1;
}

void cDatabase::loadTextures()
{
	database.texture[TEX_UI_TOOLTIP] = visual.addTexture("ui_tooltip.png");
	database.texture[TEX_WORLD_GROUND] = visual.addTexture("bg_snow.jpg");
	database.texture[TEX_SELECTION_CIRCLE] = visual.addTexture("selectionWhite512.png");
	database.texture[TEX_CLOUD] = visual.addTexture("weather_clouds.png");
	database.texture[TEX_MOUSE_DEFAULT] = visual.addTexture("mouse_default.png");
	database.texture[TEX_MOUSE_DEFAULTPRESS] = visual.addTexture("mouse_defaultPress.png");
	database.texture[TEX_MOUSE_POINT] = visual.addTexture("mouse_point.png");
	database.texture[TEX_MOUSE_TARGET] = visual.addTexture("mouse_target40.png");
	database.texture[TEX_MOUSE_PROGRESS] = visual.addTexture("mouse_progress.png");
	database.texture[TEX_MOUSE_PROGRESSBG] = visual.addTexture("mouse_progressBG.png");
}

void cDatabase::generateTextures()
{
	console << "[DATABASE] Generating textures" << endl;
	// Generating cloud textures
	for (int i = 0; i < LIMIT_GENTEXCOUNT_CLOUD; i++)
	{
		float cloudSize = math.randf(300.00f, 500.00f);
		database.texture[TEX_GEN_CLOUD + i] = visual.genCloudTexture(
			vec2f(cloudSize, cloudSize),
			vec2f(cloudSize - 250.00f, cloudSize - 250.00f),
			vec2f(cloudSize - 150.00f, cloudSize - 150.00f),
			math.rand(3, 5));
	}
	console << "[DATABASE] Textures ready" << endl;
}