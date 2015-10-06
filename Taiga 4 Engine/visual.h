
#pragma once
#include "precomp.h"
#include "define.h"

#define SHADER_FULLSCREEN				1
#define SHADER_BLOOM					2
#define SHADER_LIGHT					3
#define SHADER_SHADOW					4
#define SHADER_CAMBLUR					5
#define SHADER_BLUR						6
#define SHADER_COLORBLUR				7

class cTexture
{
public:
	string name;
	sf::Texture handle;
};

class cMousePointer
{
public:
	int type;
	int texture;
	int texturePress;

	void showHardware();
	void hideHardware();
	vec2f getOrigin();
	vec2i getTexSize();
};

class cProgressBar
{
public:
	bool enabled;
	bool manualMode;
	float manualModeStep;
	float timeCurrent;
	float timeMaximum;
	float fadeTimer;
	int getTexFrameCountX();
	int getTexFrameCountY();
	float getCurrentAlpha();
	vec2i getCurrentFrame();
	vec2i getTexFrameSize();
};

class cVisual
{
public:
	int maxTextureSize;

	int gameTexCounter;
	cTexture gameTex[LIMIT_TEXTURE];

	sf::Font fontMain;
	sf::Font fontDescr;
	sf::Font fontConsole;
	sf::Shader shader[LIMIT_SHADERS];
	cMousePointer mouse;
	cProgressBar progress;

	float shadowBrightness;
	int unitsPainted;
	int cloudsPainted;
	int particlesPainted;
	int hoveredUnit;

	vec2f lastMouseClickPos;
	float lastMouseClickTimer;
	vec2f mouseWheelScrollAnchor;

	vector<int> unitHoverQueue;

	void init();
	int addTexture(string name, bool ignoreFilter = false, bool forceFilter = false);
	int addTexture(sf::Texture tex, bool ignoreFilter = false, bool forceFilter = false);
	int createIcon(string filename, string iconName, vec2i pos, vec2f gridSize);

	sf::RenderTexture renderCloud;
	int genHighlighted(string filename);
	int genCloudTexture(vec2f size, vec2f pieceMin, vec2f pieceMax, int pieceCount);

	void setMousePointer(int type);
	void enableProgressBar(float time);
	void enableProgressBarManual(float time, int stepCount);
	void advanceProgressBar();
	void disableProgressBar();

	void registerMouseClick();
	bool isDoubleClick();

	void unitHoverScroll(int delta);

	cVisual() {
		hoveredUnit = -1;
		unitsPainted = 0;
		gameTexCounter = 0;
		maxTextureSize = sf::Texture::getMaximumSize();
	}
};

extern cVisual visual;