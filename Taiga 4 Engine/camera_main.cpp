
#include "main.h"
#include "camera.h"
#include "client.h"
#include "UI.h"
#include "window.h"
#include "math.h"
#include "world.h"

bool cCamera::isMoving()
{
	return (moveVector.x != 0.00f || moveVector.y != 0.00f);
}

void cCamera::move(sf::Vector2f offset)
{
	camera.pos += offset;

	sf::Packet data;
	// Camera position update
	if (client.connected && !core.localServer)
	{
		data << MSG_CONTROLS_CAMERA << pos.x << pos.y;
		client.sendPacket(data);
		data.clear();
	}
	// Mouse hold order
	if (client.connected && ui.mouseStateLMB == MOUSE_CONTROLS_MOVEMENT && moveUpdateTimer <= 0.00f)
	{
		moveUpdateTimer = 0.05f;
		sf::Vector2f mousePos = window.getMousePos(true);
		// Sending data to server
		data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << false;
		client.sendPacket(data);
		data.clear();
	}
}

void cCamera::moveto(sf::Vector2f newPos)
{
	move(newPos - pos - sf::Vector2f(res.x / 2.00f, res.y / 2.00f));
}

void cCamera::setZoom(float zoom)
{
	if (zoomFactor != zoom)
	{
		zoomFactor = zoom;
		// Enable the blur
		moveVector.x = 0.50f;
		moveVector.y = 0.50f;
	}
}

void cCamera::adjustZoom(int delta)
{
	setZoom(zoomFactor + zoomFactor * 0.20f * delta);
	if (!core.advancedDebug)
	{
		if (zoomFactor > 7.00f) { setZoom(7.00f); }
		if (zoomFactor < 1.00f) { zoomFactor = 1.00f; }
	}
}

sf::FloatRect cCamera::getCameraRect(vec2f offset)
{
	return sf::FloatRect(pos.x + offset.x, pos.y + offset.y, res.x, res.y);
}

bool cCamera::isPositionValid(vec2f loc)
{
	vec2i chunk;
	/*
	// Horizontal check
	for (float x = loc.x; x < loc.x + camera.res.x; x += LIMIT_CHUNKSIZE)
	{
		chunk = world.getChunkInPos(vec2f(loc.x + x, loc.y));
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
		chunk = world.getChunkInPos(vec2f(loc.x + x, loc.y + camera.res.y));
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	}
	// Vertical check
	for (float y = loc.y; y < loc.y + camera.res.y; y += LIMIT_CHUNKSIZE)
	{
		chunk = world.getChunkInPos(vec2f(loc.x, loc.y + y));
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
		chunk = world.getChunkInPos(vec2f(loc.x + camera.res.x, loc.y + y));
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	}
	chunk = world.getChunkInPos(vec2f(loc.x + camera.res.x, loc.y + camera.res.y));
	if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	*/
	return true;
}

bool cCamera::isCameraMoveValid(vec2f offset)
{
	// Only one-directional moves allowed!
	vec2i chunk;
	vec2f checkPos;
	// Move horizontally
	if (offset.x != 0.00f)
	{
		if (offset.x > 0.00f) { checkPos = vec2f(pos.x + res.x + offset.x, pos.y); }
		else { checkPos = vec2f(pos.x + offset.x, pos.y); }
		// General positions
		for (int y = 0; y < res.y / LIMIT_CHUNKSIZE; y++)
		{
			chunk = world.getChunkInPos(checkPos);
			if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
			checkPos.y += LIMIT_CHUNKSIZE;
		}
		// Final position
		checkPos.y = pos.y + res.y;
		chunk = world.getChunkInPos(checkPos);
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	}
	// Move vertically
	else if (offset.y != 0.00f)
	{
		if (offset.y > 0.00f) { checkPos = vec2f(pos.x, pos.y + res.y + offset.y); }
		else { checkPos = vec2f(pos.x, pos.y + offset.y); }
		// General positions
		for (int x = 0; x < res.x / LIMIT_CHUNKSIZE; x++)
		{
			chunk = world.getChunkInPos(checkPos);
			if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
			checkPos.x += LIMIT_CHUNKSIZE;
		}
		// Final position
		checkPos.x = pos.x + res.x;
		chunk = world.getChunkInPos(checkPos);
		if (world.map[chunk.x][chunk.y].type == CHUNK_UNDEFINED) { return false; }
	}
	return true;
}