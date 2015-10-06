
#pragma once
#include "precomp.h"

class cSettingsKey
{
public:
	std::string name;
	std::string value;
};

class cSettings
{
public:
	//========================================
	// Display
	//========================================
	// Screen mode [Fullscreen / Window / Borderless]
	int screenMode;
	// Vertical sync [Fixes the image tearing]
	int enableVertSync;
	// Sample modifier [Render resolution over display resolution]
	float sampleMod;
	// Framerate limit [Maximum allowed framerate]
	int framerateLimit;

	//========================================
	// Visual
	//========================================
	// Antialiasing [Hardware-dependant type of AA, improves image quality]
	int antialiasingLevel;
	// Fullscreen shaders [Required for less important effects]
	int enableScreenShaders;
	// Night shadows [Moon can also cast objects' shadows]
	int enableNightShadows;
	// Alternate shadows [Prevents shadows from stacking]
	int enableBetterShadows;
	// Better lighting [Enables brighter lights]
	int enableBetterLight;
	// Texture filtering [Reduces image pixelization]
	int enableTextureSmoothing;
	// Camera blur [Game world is blurred when the camera is moving]
	int enableCameraBlur;
	// Particle shadows [Enable snow or rain shadow casting]
	int enableParticleShadows;
	// High precision shadows [Enable shader mode for particle shadows]
	int enableBetterParticleShadows;
	// High precision shadows [Enable high-precision mode for particle shadows]
	int enablePreciseParticleShadows;
	// Cloud density [Change the amount of clouds on the screen]
	float cloudDensity;
	// Particle density [Change the amount of particles on the screen]
	float particleDensity;
	// Enable unit highlight color [Change the unit color when highlighted]
	int enableUnitHighlightColor;
	// Enable unit highlight outline [Add outline to unit when hightlighted]
	int enableUnitHighlightOutline;
	// Unit highlight color
	sf::Color visualUnitHoverColor;
	// Unit highline outline color
	sf::Color visualUnitHoverOutline;

	//========================================
	// Gameplay
	//========================================
	// Lazy cast [Active items trigger on mouse click]
	int enableLazyCast;
	// Quick cast [Active items trigger on button release]
	int enableQuickCast;
	// Mouse scroll [Mouse outside of the window moves the camera]
	int enableMouseScroll;
	// Dynamic tooltips [Tooltips are moving with the mouse]
	int enableDynamicTooltips;
	// Mouse scroll speed [How fast the camera moves with mouse]
	int cameraScrollSpeedMouse;
	// Keyboard scroll speed [How fast the camera moves with keyboard]
	int cameraScrollSpeedKeyboard;

	//========================================
	// Chat
	//========================================
	// Chat window width
	int chatSizeX;
	// Chat window height
	int chatSizeY;
	// Chat X-axis position
	int chatPosX;
	// Chat Y-axis position
	int chatPosY;
	// Chat font size
	int chatFontSize;
	// Chat line spacing [Extra space between text lines in chat]
	int chatLineSpacing;
	// Chat scroll speed [Skipped line count when scrolling]
	int chatScrollSpeed;
	// Enable chat timestamps [Display message arrival time]
	int enableChatTimestamps;

	//========================================
	// Console
	//========================================
	// Console scroll speed
	int consoleScrollSpeed;
	// Console font size
	int consoleFontSize;
	// Console line spacing
	int consoleLineSpacing;
	// Enable console timestamps
	int enableConsoleTimestamps;

	//========================================
	// Audio
	//========================================
	// Master volume
	float volMaster;
	// Music volume
	float volMusic;
	// Special effect volume
	float volEffects;
	// Ambient sound volume
	float volAmbient;
	// Weather sound volume
	float volWeather;
	// Footstep volume
	float volFootsteps;
	// Camera listener [Positional sound is calculated from camera position]
	int enableCameraListener;

	//========================================
	// Keyboard
	//========================================
	sf::Keyboard::Key hkInventory;
	sf::Keyboard::Key hkActiveItem[LIMIT_ACTIVEBUTTONS];
	sf::Keyboard::Key hkCamMove[4];
	sf::Keyboard::Key hkCamToHero;
	sf::Keyboard::Key hkChat;
	sf::Keyboard::Key hkConsole;
	sf::Keyboard::Key hkDebugMode;
	sf::Keyboard::Key hkDebugAdvanced;
	sf::Keyboard::Key hkHideInterface;
	sf::Keyboard::Key hkScreenshot;

	//========================================
	// Internal: World generation
	//========================================
	// Starting path counter [Increases the open area close to spawn point]
	int wgStartingPath;
	// Minimal path length [Increases the total map size]
	int wgMinimalPathLength;
	// Maximum path length [Decreases the maximum map size]
	int wgMaximalPathLength;
	// Path fork chance [Increases the amount of paths on the map]
	int wgPathForkChance;

	//========================================
	// Internal: Day-night cycle
	//========================================
	// Day-night cycle speed
	float wdDayNightSpeed;
	// Night transition speed
	float wdNightChangeSpeed;

	//========================================
	// Internal: Random
	//========================================
	// Dynamic lighting [Enables realistic lighting system]
	int enableDynamicLight;
	// Shadow blur [Uses shaders to make shadows look more realistic]
	int shadowBlur;

	//========================================
	// Functions
	//========================================
	void load();
	void save();
	void updateFile();
	void setDefault();

	bool getNextKey(std::ifstream* file, cSettingsKey *key);

	//========================================
	// Graphics presets
	//========================================
	void setUltra();
};

extern cSettings settings;