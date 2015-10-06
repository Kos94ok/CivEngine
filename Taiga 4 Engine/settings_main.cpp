
#include "main.h"
#include "settings.h"
#include "camera.h"
#include "window.h"
#include "math.h"
#include "client.h"

void cSettings::setDefault()
{
	//========================================
	// Display
	//========================================
	// Screen mode [Fullscreen / Window / Borderless]
	this->screenMode = 1;
	// Vertical sync [Fixes the image tearing]
	this->enableVertSync = 0;
	// Sample modifier [Render resolution over display resolution]
	this->sampleMod = 1.00f;
	// Framerate limit [Maximum allowed framerate]
	this->framerateLimit = 0;

	//========================================
	// Visual
	//========================================
	// Antialiasing [Hardware-dependant type of AA, improves image quality]
	this->antialiasingLevel = 0;
	// Fullscreen shaders [Required for less important effects]
	this->enableScreenShaders = 1;
	// Night shadows [Moon can also cast objects' shadows]
	this->enableNightShadows = 1;
	// Alternate shadows [Prevents shadows from stacking]
	this->enableBetterShadows = 1;
	// Better lighting [Enables brighter lights]
	this->enableBetterLight = 1;
	// Texture filtering [Reduces image pixelization]
	this->enableTextureSmoothing = 1;
	// Camera blur [Game world is blurred when the camera is moving]
	this->enableCameraBlur = 1;
	// Particle shadows [Enable snow or rain shadow casting]
	this->enableParticleShadows = 1;
	// High precision shadows [Enable shader mode for particle shadows]
	this->enableBetterParticleShadows = 1;
	// High precision shadows [Enable high-precision mode for particle shadows]
	this->enablePreciseParticleShadows = 1;
	// Cloud density [Change the amount of clouds on the screen]
	this->cloudDensity = 1.00f;
	// Particle density [Change the amount of particles on the screen]
	this->particleDensity = 1.00f;
	// Enable unit highlight color [Change the unit color when highlighted]
	int enableUnitHighlightColor = 1;
	// Enable unit highlight outline [Add outline to unit when hightlighted]
	int enableUnitHighlightOutline = 1;
	// Unit highlight color
	this->visualUnitHoverColor = util.convertUnitHighlightColor(1);
	// Unit highlight outline color
	this->visualUnitHoverOutline = util.convertUnitHighlightColor(2);

	//========================================
	// Gameplay
	//========================================
	// Lazy cast [Active items trigger on mouse click]
	this->enableLazyCast = 1;
	// Quick cast [Active items trigger on button release]
	this->enableQuickCast = 0;
	// Mouse scroll [Mouse outside of the window moves the camera]
	this->enableMouseScroll = 1;
	// Dynamic tooltips [Tooltips are moving with the mouse]
	this->enableDynamicTooltips = 0;
	// Mouse scroll speed [How fast the camera moves with mouse]
	this->cameraScrollSpeedMouse = 100;
	// Keyboard scroll speed [How fast the camera moves with keyboard]
	this->cameraScrollSpeedKeyboard = 100;

	//========================================
	// Chat
	//========================================
	// Chat X-axis position
	this->chatPosX = 0;
	// Chat Y-axis position
	this->chatPosY = -50;
	// Chat window width
	this->chatSizeX = 400;
	// Chat window height
	this->chatSizeY = 250;
	// Chat font size
	this->chatFontSize = 14;
	// Chat line spacing [Extra space between text lines in chat]
	this->chatLineSpacing = 1;
	// Chat scroll speed [Skipped line count when scrolling]
	this->chatScrollSpeed = 1;
	// Enable chat timestamps [Display message arrival time]
	this->enableChatTimestamps = 0;

	//========================================
	// Console
	//========================================
	// Console scroll speed
	this->consoleScrollSpeed = 3;
	// Console font size
	this->consoleFontSize = 14;
	// Console line spacing
	this->consoleLineSpacing = 0;
	// Enable console timestamps
	this->enableConsoleTimestamps = 0;

	//========================================
	// Audio
	//========================================
	// Master volume
	this->volMaster = 1.00f;
	// Music volume
	this->volMusic = 1.00f;
	// Special effect volume
	this->volEffects = 1.00f;
	// Ambient sound volume
	this->volAmbient = 1.00f;
	// Weather sound volume
	this->volWeather = 1.00f;
	// Footstep volume
	this->volFootsteps = 1.00f;
	// Camera listener [Positional sound is calculated from camera position]
	this->enableCameraListener = 1;

	//========================================
	// Keyboard
	//========================================
	this->hkInventory = sf::Keyboard::E;
	this->hkActiveItem[1] = sf::Keyboard::Key(sf::Keyboard::Num0);
	this->hkActiveItem[2] = sf::Keyboard::Key(sf::Keyboard::Num1);
	this->hkActiveItem[3] = sf::Keyboard::Key(sf::Keyboard::Num2);
	this->hkActiveItem[4] = sf::Keyboard::Key(sf::Keyboard::Num3);
	this->hkActiveItem[5] = sf::Keyboard::Key(sf::Keyboard::Num4);
	this->hkActiveItem[6] = sf::Keyboard::Key(sf::Keyboard::Num5);
	this->hkActiveItem[7] = sf::Keyboard::Key(sf::Keyboard::Num6);
	this->hkActiveItem[8] = sf::Keyboard::Key(sf::Keyboard::Num7);
	this->hkActiveItem[9] = sf::Keyboard::Key(sf::Keyboard::Num8);
	this->hkCamMove[0] = sf::Keyboard::W;
	this->hkCamMove[1] = sf::Keyboard::S;
	this->hkCamMove[2] = sf::Keyboard::A;
	this->hkCamMove[3] = sf::Keyboard::D;
	this->hkCamToHero = sf::Keyboard::Space;
	this->hkChat = sf::Keyboard::Return;
	this->hkConsole = sf::Keyboard::Tilde;
	this->hkDebugMode = sf::Keyboard::F3;
	this->hkDebugAdvanced = sf::Keyboard::F4;
	this->hkHideInterface = sf::Keyboard::F11;
	this->hkScreenshot = sf::Keyboard::F12;

	//========================================
	// Internal: World generation
	//========================================
	// Starting path counter [Increases the open area close to spawn point]
	this->wgStartingPath = 1;
	// Minimal path length [Increases the total map size]
	this->wgMinimalPathLength = 30;
	// Maximum path length [Decreases the maximum map size]
	this->wgMaximalPathLength = 50;
	// Path fork chance [Increases the amount of paths on the map]
	this->wgPathForkChance = 10;
	/*
	Taiga Mini:
		wgStartingPath = 1;
		wgMinimalPathLength = 3;
		wgMaximalPathLength = 10;
		wgPathForkChance = 20;
	Taiga Maxi:
		wgStartingPath = 3;
		wgMinimalPathLength = 15;
		wgMaximalPathLength = 100;
		wgPathForkChance = 20;
	*/

	//========================================
	// Internal: Day-night cycle
	//========================================
	// Day-night cycle speed
	this->wdDayNightSpeed = 1.00f / 72.00f;
	// Night transition speed
	this->wdNightChangeSpeed = 1.00f;
	/*
	Arcade mode (fast):
	wdDayNightSpeed = 1.00f / 24.00f;
	wdNightChangeSpeed = 3.00f;
	Normal mode:
	wdDayNightSpeed = 1.00f / 72.00f;
	wdNightChangeSpeed = 1.00f;
	*/

	//========================================
	// Internal: Random
	//========================================
	// Dynamic lighting [Enables realistic lighting system]
	this->enableDynamicLight = 1;
	// Shadow blur [Uses shaders to make shadows look more realistic]
	this->shadowBlur = 0;
}

void cSettings::setUltra() {}

void cSettings::load()
{
	cSettingsKey key;
	ifstream file;
	file.open("settings.ini");
	if (!file.good()) {
		console.error << "[ERROR] Can't open the settings file. Returning to default" << "\n";
		settings.save();
		return;
	}
	
	// Looking for values until the end of the file
	bool retVal = true;
	while (retVal)
	{
		retVal = getNextKey(&file, &key);
		// Matching the values
			// Display
		if (key.name == "screenMode") { screenMode = math.stringToInt(key.value); }
		else if (key.name == "screenWidth" && !core.serverMode) { camera.res.x = math.stringToInt(key.value); }
		else if (key.name == "screenHeight" && !core.serverMode) { camera.res.y = math.stringToInt(key.value); }
		else if (key.name == "consoleWidth" && core.serverMode) { camera.res.x = math.stringToInt(key.value); }
		else if (key.name == "consoleHeight" && core.serverMode) { camera.res.y = math.stringToInt(key.value); }
		else if (key.name == "enableVertSync") { enableVertSync = math.stringToInt(key.value); }
		else if (key.name == "sampleMod") { sampleMod = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "framerateLimit") { framerateLimit = math.stringToInt(key.value); }
			// Visual
		else if (key.name == "antialiasingLevel") { antialiasingLevel = math.stringToInt(key.value); }
		else if (key.name == "enableScreenShaders") { enableScreenShaders = math.stringToInt(key.value); }
		else if (key.name == "enableNightShadows") { enableNightShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterShadows") { enableBetterShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterLight") { enableBetterLight = math.stringToInt(key.value); }
		else if (key.name == "enableTextureSmoothing") { enableTextureSmoothing = math.stringToInt(key.value); }
		else if (key.name == "enableCameraBlur") { enableCameraBlur = math.stringToInt(key.value); }
		else if (key.name == "enableParticleShadows") { enableParticleShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterParticleShadows") { enableBetterParticleShadows = math.stringToInt(key.value); }
		else if (key.name == "enablePreciseParticleShadows") { enablePreciseParticleShadows = math.stringToInt(key.value); }
		else if (key.name == "shadowBlur") { shadowBlur = math.stringToInt(key.value); }
		else if (key.name == "cloudDensity") { cloudDensity = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "particleDensity") { particleDensity = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "enableUnitHighlightColor") { enableUnitHighlightColor = math.stringToInt(key.value); }
		else if (key.name == "enableUnitHighlightOutline") { enableUnitHighlightOutline = math.stringToInt(key.value); }
		else if (key.name == "visualUnitHoverColorCode") { visualUnitHoverColor = util.convertUnitHighlightColor(math.stringToInt(key.value)); }
		else if (key.name == "visualUnitHoverColor16") { visualUnitHoverColor = util.parseOldschoolColorCode(key.value); }
		else if (key.name == "visualUnitHoverOutlineCode") { visualUnitHoverOutline = util.convertUnitHighlightColor(math.stringToInt(key.value)); }
		else if (key.name == "visualUnitHoverOutline16") { visualUnitHoverOutline = util.parseOldschoolColorCode(key.value); }
			// Gameplay
		else if (key.name == "enableLazyCast") { enableLazyCast = math.stringToInt(key.value); }
		else if (key.name == "enableQuickCast") { enableQuickCast = math.stringToInt(key.value); }
		else if (key.name == "enableMouseScroll") { enableMouseScroll = math.stringToInt(key.value); }
		else if (key.name == "enableDynamicTooltips") { enableDynamicTooltips = math.stringToInt(key.value); }
		else if (key.name == "cameraScrollSpeedMouse") { cameraScrollSpeedMouse = math.stringToInt(key.value); }
		else if (key.name == "cameraScrollSpeedKeyboard") { cameraScrollSpeedKeyboard = math.stringToInt(key.value); }
			// Chat
		else if (key.name == "playerName") { client.playerName = key.value; }
		else if (key.name == "chatPosX") { chatPosX = math.stringToInt(key.value); }
		else if (key.name == "chatPosY") { chatPosY = math.stringToInt(key.value); }
		else if (key.name == "chatSizeX") { chatSizeX = math.stringToInt(key.value); }
		else if (key.name == "chatSizeY") { chatSizeY = math.stringToInt(key.value); }
		else if (key.name == "chatFontSize") { chatFontSize = math.stringToInt(key.value); }
		else if (key.name == "chatLineSpacing") { chatLineSpacing = math.stringToInt(key.value); }
		else if (key.name == "chatScrollSpeed") { chatScrollSpeed = math.stringToInt(key.value); }
		else if (key.name == "enableChatTimestamps") { enableChatTimestamps = math.stringToInt(key.value); }
			// Console
		else if (key.name == "consoleFontSize") { consoleFontSize = math.stringToInt(key.value); }
		else if (key.name == "consoleLineSpacing") { consoleLineSpacing = math.stringToInt(key.value); }
		else if (key.name == "consoleScrollSpeed") { consoleScrollSpeed = math.stringToInt(key.value); }
		else if (key.name == "enableConsoleTimestamps") { enableConsoleTimestamps = math.stringToInt(key.value); }
			// Audio
		else if (key.name == "volumeMaster") { volMaster = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeMusic") { volMusic = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeEffects") { volEffects = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeAmbient") { volAmbient = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeWeather") { volWeather = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeFootsteps") { volFootsteps = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "enableCameraListener") { enableCameraListener = math.stringToInt(key.value); }
			// Keyboard
		else if (key.name == "hkInventory") { hkInventory = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem01") { hkActiveItem[1] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem02") { hkActiveItem[2] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem03") { hkActiveItem[3] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem04") { hkActiveItem[4] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem05") { hkActiveItem[5] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem06") { hkActiveItem[6] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem07") { hkActiveItem[7] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem08") { hkActiveItem[8] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem09") { hkActiveItem[9] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveUp") { hkCamMove[0] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveDown") { hkCamMove[1] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveLeft") { hkCamMove[2] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveRight") { hkCamMove[3] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamToHero") { hkCamToHero = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkChat") { hkChat = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkConsole") { hkConsole = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkDebugMode") { hkDebugMode = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkDebugAdvanced") { hkDebugAdvanced = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkHideInterface") { hkHideInterface = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkScreenshot") { hkScreenshot = sf::Keyboard::Key(math.stringToInt(key.value)); }
	}

	// Fix the dependancies
	if (enableScreenShaders == 0) { enableBetterShadows = 0; enableBetterLight = 0; }

	file.close();
}

void cSettings::save()
{
	ofstream file;
	file.open("settings.ini");
	if (file.good())
	{
		file << "[Display]" << "\n";
		file << "screenMode = " << screenMode << "\n";
		file << "screenWidth = " << camera.res.x << "\n";
		file << "screenHeight = " << camera.res.y << "\n";
		file << "enableVertSync = " << enableVertSync << "\n";
		file << "sampleMod = " << math.round(sampleMod * 100.00f) << "\n";
		file << "framerateLimit = " << framerateLimit << "\n";

		file << endl << "[Visual]" << "\n";
		file << "antialiasingLevel = " << antialiasingLevel << "\n";
		file << "enableScreenShaders = " << enableScreenShaders << "\n";
		file << "enableNightShadows = " << enableNightShadows << "\n";
		file << "enableBetterShadows = " << enableBetterShadows << "\n";
		file << "enableBetterLight = " << enableBetterLight << "\n";
		file << "enableTextureSmoothing = " << enableTextureSmoothing << "\n";
		file << "enableCameraBlur = " << enableCameraBlur << "\n";
		file << "shadowBlur = " << shadowBlur << "\n";
		file << "enableParticleShadows = " << enableParticleShadows << "\n";
		file << "enableBetterParticleShadows = " << enableBetterParticleShadows << "\n";
		file << "enablePreciseParticleShadows = " << enablePreciseParticleShadows << "\n";
		file << "cloudDensity = " << math.round(cloudDensity * 100.00f) << "\n";
		file << "particleDensity = " << math.round(particleDensity * 100.00f) << "\n";
		file << "enableUnitHighlightColor = " << enableUnitHighlightColor << "\n";
		file << "enableUnitHighlightOutline = " << enableUnitHighlightOutline << "\n";

		file << endl << "[Gameplay]" << "\n";
		file << "enableLazyCast = " << enableLazyCast << "\n";
		file << "enableQuickCast = " << enableQuickCast << "\n";
		file << "enableMouseScroll = " << enableMouseScroll << "\n";
		file << "enableDynamicTooltips = " << enableDynamicTooltips << "\n";
		file << "cameraScrollSpeedMouse = " << cameraScrollSpeedMouse << "\n";
		file << "cameraScrollSpeedKeyboard = " << cameraScrollSpeedKeyboard	 << "\n";

		file << endl << "[Chat]" << "\n";
		file << "playerName = " << client.playerName << "\n";
		file << "chatPosX = " << chatPosX << "\n";
		file << "chatPosY = " << chatPosY << "\n";
		file << "chatSizeX = " << chatSizeX << "\n";
		file << "chatSizeY = " << chatSizeY << "\n";
		file << "chatFontSize = " << chatFontSize << "\n";
		file << "chatLineSpacing = " << chatLineSpacing << "\n";
		file << "chatScrollSpeed = " << chatScrollSpeed << "\n";
		file << "enableChatTimestamps = " << enableChatTimestamps << "\n";

		file << endl << "[Console]" << "\n";
		file << "consoleFontSize = " << consoleFontSize << "\n";
		file << "consoleLineSpacing = " << consoleLineSpacing << "\n";
		file << "consoleScrollSpeed = " << consoleScrollSpeed << "\n";
		file << "enableConsoleTimestamps = " << enableConsoleTimestamps << "\n";

		file << endl << "[Audio]" << "\n";
		file << "volumeMaster = " << math.round(volMaster * 100.00f) << "\n";
		file << "volumeMusic = " << math.round(volMusic * 100.00f) << "\n";
		file << "volumeEffects = " << math.round(volEffects * 100.00f) << "\n";
		file << "volumeAmbient = " << math.round(volAmbient * 100.00f) << "\n";
		file << "volumeWeather = " << math.round(volWeather * 100.00f) << "\n";
		file << "volumeFootsteps = " << math.round(volFootsteps * 100.00f) << "\n";
		file << "enableCameraListener = " << enableCameraListener << "\n";

		file << endl << "[Keyboard]" << "\n";
		file << "hkInventory = " << hkInventory << "\n";
		file << "hkActiveItem01 = " << hkActiveItem[1] << "\n";
		file << "hkActiveItem02 = " << hkActiveItem[2] << "\n";
		file << "hkActiveItem03 = " << hkActiveItem[3] << "\n";
		file << "hkActiveItem04 = " << hkActiveItem[4] << "\n";
		file << "hkActiveItem05 = " << hkActiveItem[5] << "\n";
		file << "hkActiveItem06 = " << hkActiveItem[6] << "\n";
		file << "hkActiveItem07 = " << hkActiveItem[7] << "\n";
		file << "hkActiveItem08 = " << hkActiveItem[8] << "\n";
		file << "hkActiveItem09 = " << hkActiveItem[9] << "\n";
		file << "hkCamMoveUp = " << hkCamMove[0] << "\n";
		file << "hkCamMoveDown = " << hkCamMove[1] << "\n";
		file << "hkCamMoveLeft = " << hkCamMove[2] << "\n";
		file << "hkCamMoveRight = " << hkCamMove[3] << "\n";
		file << "hkCamToHero = " << hkCamToHero << "\n";
		file << "hkChat = " << hkChat << "\n";
		file << "hkConsole = " << hkConsole << "\n";
		file << "hkDebugMode = " << hkDebugMode << "\n";
		file << "hkDebugAdvanced = " << hkDebugAdvanced << "\n";
		file << "hkHideInterface = " << hkHideInterface << "\n";
		file << "hkScreenshot = " << hkScreenshot << "\n";
	}
	else { console.error << "[ERROR] Can't save the settings file!" << "\n"; }
	file.close();
}

void cSettings::updateFile()
{
	char buf[512];
	string buffer;
	cSettingsKey key;
	// Swap current settings file to old
	CopyFileA("settings.ini", "settings_old", false);

	// Open the old file
	ifstream oldfile;
	oldfile.open("settings_old");
	if (!oldfile.good()) {
		console.error << "[cSettings::updateFile] Can't read the old file!" << "\n";
		settings.save();
		return;
	}

	ofstream file;
	file.open("settings.ini");

	// Looking for values until the end of the file
	bool retVal = true;
	while (!oldfile.eof())
	{
		oldfile.getline(buf, 512);
		buffer = buf;
		key.name = buffer.substr(0, buffer.find(" ="));
		// Matching the values
		// Display
		if (key.name == "screenMode") { file << key.name + " = " << screenMode << endl; }
		else if (key.name == "screenWidth" && !core.serverMode) { file << key.name + " = " << camera.res.x << endl; }
		else if (key.name == "screenHeight" && !core.serverMode) { file << key.name + " = " << camera.res.y << endl; }
		else if (key.name == "consoleWidth" && core.serverMode) { file << key.name + " = " << camera.res.x << endl; }
		else if (key.name == "consoleHeight" && core.serverMode) { file << key.name + " = " << camera.res.y << endl; }
		else if (key.name == "enableVertSync") { file << key.name + " = " << enableVertSync << endl; }
		else if (key.name == "sampleMod") { file << key.name + " = " << math.round(sampleMod * 100.00f) << endl; }
		else if (key.name == "framerateLimit") { file << key.name + " = " << framerateLimit << endl; }
		// Visual
		else if (key.name == "antialiasingLevel") { file << key.name + " = " << antialiasingLevel << endl; }
		else if (key.name == "enableScreenShaders") { file << key.name + " = " << enableScreenShaders << endl; }
		else if (key.name == "enableNightShadows") { file << key.name + " = " << enableNightShadows << endl; }
		else if (key.name == "enableBetterShadows") { file << key.name + " = " << enableBetterShadows << endl; }
		else if (key.name == "enableBetterLight") { file << key.name + " = " << enableBetterLight << endl; }
		else if (key.name == "enableTextureSmoothing") { file << key.name + " = " << enableTextureSmoothing << endl; }
		else if (key.name == "enableCameraBlur") { file << key.name + " = " << enableCameraBlur << endl; }
		else if (key.name == "shadowBlur") { file << key.name + " = " << shadowBlur << endl; }
		else if (key.name == "enableParticleShadows") { file << key.name + " = " << enableParticleShadows << endl; }
		else if (key.name == "enableBetterParticleShadows") { file << key.name + " = " << enableBetterParticleShadows << endl; }
		else if (key.name == "enablePreciseParticleShadows") { file << key.name + " = " << enablePreciseParticleShadows << endl; }
		else if (key.name == "cloudDensity") { file << key.name + " = " << math.round(cloudDensity * 100.00f) << endl; }
		else if (key.name == "particleDensity") { file << key.name + " = " << math.round(particleDensity * 100.00f) << endl; }
		else if (key.name == "enableUnitHightlightColor") { file << key.name + " = " << enableUnitHighlightColor << endl; }
		else if (key.name == "enableUnitHightlightOutline") { file << key.name + " = " << enableUnitHighlightOutline << endl; }
		//else if (key.name == "visualUnitHoverColorCode") { visualUnitHoverColor = util.convertUnitHighlightColor(math.stringToInt(key.value)); }
		//else if (key.name == "visualUnitHoverColor16") { visualUnitHoverColor = util.parseOldschoolColorCode(key.value); }
		// Gameplay
		else if (key.name == "enableLazyCast") { file << key.name + " = " << enableLazyCast << endl; }
		else if (key.name == "enableQuickCast") { file << key.name + " = " << enableQuickCast << endl; }
		else if (key.name == "enableMouseScroll") { file << key.name + " = " << enableMouseScroll << endl; }
		else if (key.name == "enableDynamicTooltips") { file << key.name + " = " << enableDynamicTooltips << endl; }
		else if (key.name == "cameraScrollSpeedMouse") { file << key.name + " = " << cameraScrollSpeedMouse << endl; }
		else if (key.name == "cameraScrollSpeedKeyboard") { file << key.name + " = " << cameraScrollSpeedKeyboard << endl; }
		// Chat
		else if (key.name == "playerName") { file << key.name + " = " << client.playerName << endl; }
		else if (key.name == "chatPosX") { file << key.name + " = " << chatPosX << endl; }
		else if (key.name == "chatPosY") { file << key.name + " = " << chatPosY << endl; }
		else if (key.name == "chatSizeX") { file << key.name + " = " << chatSizeX << endl; }
		else if (key.name == "chatSizeY") { file << key.name + " = " << chatSizeY << endl; }
		else if (key.name == "chatFontSize") { file << key.name + " = " << chatFontSize << endl; }
		else if (key.name == "chatLineSpacing") { file << key.name + " = " << chatLineSpacing << endl; }
		else if (key.name == "chatScrollSpeed") { file << key.name + " = " << chatScrollSpeed << endl; }
		else if (key.name == "enableChatTimestamps") { file << key.name + " = " << enableChatTimestamps << endl; }
		// Console
		else if (key.name == "consoleFontSize") { file << key.name + " = " << consoleFontSize << endl; }
		else if (key.name == "consoleLineSpacing") { file << key.name + " = " << consoleLineSpacing << endl; }
		else if (key.name == "consoleScrollSpeed") { file << key.name + " = " << consoleScrollSpeed << endl; }
		else if (key.name == "enableConsoleTimestamps") { file << key.name + " = " << enableConsoleTimestamps << endl; }
		// Audio
		else if (key.name == "volumeMaster") { file << key.name + " = " << math.round(volMaster * 100.00f) << endl; }
		else if (key.name == "volumeMusic") { file << key.name + " = " << math.round(volMusic * 100.00f) << endl; }
		else if (key.name == "volumeEffects") { file << key.name + " = " << math.round(volEffects * 100.00f) << endl; }
		else if (key.name == "volumeAmbient") { file << key.name + " = " << math.round(volAmbient * 100.00f) << endl; }
		else if (key.name == "volumeWeather") { file << key.name + " = " << math.round(volWeather * 100.00f) << endl; }
		else if (key.name == "volumeFootsteps") { file << key.name + " = " << math.round(volFootsteps * 100.00f) << endl; }
		else if (key.name == "enableCameraListener") { file << key.name + " = " << enableCameraListener << endl; }
		// Keyboard
		else if (key.name == "hkInventory") { file << key.name + " = " << hkInventory << endl; }
		else if (key.name == "hkActiveItem01") { file << key.name + " = " << hkActiveItem[1] << endl; }
		else if (key.name == "hkActiveItem02") { file << key.name + " = " << hkActiveItem[2] << endl; }
		else if (key.name == "hkActiveItem03") { file << key.name + " = " << hkActiveItem[3] << endl; }
		else if (key.name == "hkActiveItem04") { file << key.name + " = " << hkActiveItem[4] << endl; }
		else if (key.name == "hkActiveItem05") { file << key.name + " = " << hkActiveItem[5] << endl; }
		else if (key.name == "hkActiveItem06") { file << key.name + " = " << hkActiveItem[6] << endl; }
		else if (key.name == "hkActiveItem07") { file << key.name + " = " << hkActiveItem[7] << endl; }
		else if (key.name == "hkActiveItem08") { file << key.name + " = " << hkActiveItem[8] << endl; }
		else if (key.name == "hkActiveItem09") { file << key.name + " = " << hkActiveItem[9] << endl; }
		else if (key.name == "hkCamMoveUp") { file << key.name + " = " << hkCamMove[0] << endl; }
		else if (key.name == "hkCamMoveDown") { file << key.name + " = " << hkCamMove[1] << endl; }
		else if (key.name == "hkCamMoveLeft") { file << key.name + " = " << hkCamMove[2] << endl; }
		else if (key.name == "hkCamMoveRight") { file << key.name + " = " << hkCamMove[3] << endl; }
		else if (key.name == "hkCamToHero") { file << key.name + " = " << hkCamToHero << endl; }
		else if (key.name == "hkChat") { file << key.name + " = " << hkChat << endl; }
		else if (key.name == "hkConsole") { file << key.name + " = " << hkConsole << endl; }
		else if (key.name == "hkDebugMode") { file << key.name + " = " << hkDebugMode << endl; }
		else if (key.name == "hkDebugAdvanced") { file << key.name + " = " << hkDebugAdvanced << endl; }
		else if (key.name == "hkHideInterface") { file << key.name + " = " << hkHideInterface << endl; }
		else if (key.name == "hkScreenshot") { file << key.name + " = " << hkScreenshot << endl; }
		// Other
		else { file << buffer << endl; }
	}

	file.close();
	oldfile.close();
	DeleteFileA("settings_old");
}