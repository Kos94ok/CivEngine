
#pragma once
#include "precomp.h"

class cCamera
{
public:
	bool moveWithCharacter;
	bool lockedToCharacter;
	sf::Vector2i res;
	sf::Vector2f pos;
	float zoomFactor;

	float moveUpdateTimer;

	sf::Vector2f moveVector;
	bool isMoving();
	void move(sf::Vector2f offset);
	void moveto(sf::Vector2f newPos);
	void setZoom(float zoom);
	void adjustZoom(int delta);
	sf::FloatRect getCameraRect(vec2f offset);
	bool isPositionValid(vec2f loc);
	bool isCameraMoveValid(vec2f offset);

	cCamera() {
		moveUpdateTimer = 0.00f;
		zoomFactor = 1.00f;
		res.x = 1280;
		res.y = 800;
		moveWithCharacter = false;
		lockedToCharacter = false;
	}
};

extern cCamera camera;