
#pragma once
#include "precomp.h"

class cWindow
{
public:
	sf::RenderWindow winHandle;
	sf::RenderTexture texHandle;
	sf::RenderTexture texHandleShadow;
	sf::RenderTexture texHandleLight;
	sf::RenderTexture texHandleLightMult;
	sf::RenderTexture texHandleTop;
	sf::Transform matrixHandle;

	void mainEvent();
	void mainPaint();

	void paintTileMap();
	void paintUnits();
	void paintClouds();
	void paintParticles();
	void paintLighting();
	void paintPostFX();
	void paintUI();
	void paintDebugInfo();
	void paintConsole();
	void paintMousePointer();

	void updateAspectRatio();
	void showWarning(int msg, bool critical = false);

	sf::Vector2f getMousePos(bool useMatrix = false);
	sf::Vector2f getScreenSize();
};

extern cWindow window;

void windowMain();