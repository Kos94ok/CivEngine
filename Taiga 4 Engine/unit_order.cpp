
#include "main.h"
#include "math.h"
#include "server.h"
#include "world.h"
#include "game.h"
#include "path.h"
#include "visual.h"
#include "client.h"

int cUnit::addOrder_moveto(sf::Vector2f target, bool overwrite)
{
	// If unit can not move, then ignore
	if (movementSpeed <= 0.00f) { return -1; }
	// If the target is too close, then ignore
	if (math.getDistance(pos.x, pos.y, target.x, target.y) < movementSpeed / 10.00f) { return -1; }

	// Clearing the order list
	if (overwrite) { clearOrders(); }
	else if (orderCounter == 0) { actionTimer = 0.00f; }
	// Check order limit
	if (orderCounter >= LIMIT_ORDERS - 1) {
		console.error << "[cUnit::addOrder_moveto] Order limit reached!" << endl;
	}
	// Adding new order
	order[orderCounter].type = ORDER_MOVETO;
	order[orderCounter].targetPos = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_MOVETO << globalId << target.x << target.y << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_moveto_path(sf::Vector2f target, bool overwrite)
{
	// If unit can not move, then ignore
	if (movementSpeed <= 0.00f) { return -1; }
	// If target is too close, then ignore
	if (math.getDistance(pos, target) < 10.00f) { return -1; }
	// If clicked to the restricted area, then... you know
	vec2i targetChunk = world.getChunkInPos(target);
	vec2f startingPos;
	if (world.map[targetChunk.x][targetChunk.y].type == CHUNK_UNDEFINED) { return -1; }

	game.access.lock();
	if (overwrite) { clearOrders(); }
	else if (orderCounter == 0) { actionTimer = 0.00f; }

	// Checking if unit is stuck
	if (hasRef(REF_UNIT_BESTPATHING))
	{
		pos = path.validatePoint(pos, collisionDistance, globalId);
	}
	// Looking for starting position
	startingPos = pos;
	if (orderCounter > 0)
	{
		if (order[orderCounter - 1].type == ORDER_MOVETO) {
			startingPos = order[orderCounter - 1].targetPos;
		}
		else {
			startingPos = path.validatePoint(game.getUnit(order[orderCounter - 1].targetObject).pos, collisionDistance, globalId);
		}
	}
	// Calculating path
	path.calculate(startingPos, target, collisionDistance, globalId);
	// Check order limit
	if (orderCounter + path.waypointCounter >= LIMIT_ORDERS - 1) {
		console.error << "[cUnit::addOrder_moveto_path] Order limit reached!" << endl;
		path.waypointCounter = LIMIT_ORDERS - orderCounter;
	}
	// Adding orders
	for (int i = 0; i < path.waypointCounter; i++)
	{
		order[orderCounter].type = ORDER_MOVETO;
		order[orderCounter].targetPos = path.waypoint[i];
		orderCounter += 1;
	}
	// Server
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_ORDER_MOVETOPATH << globalId << target.x << target.y << overwrite;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
	// Update info
	updateFacing();
	updateAction();
	updateAnimation();
	game.access.unlock();

	return orderCounter - 1;
}

int cUnit::addOrder_pickup(int target, bool overwrite)
{
	// Clearing the order list
	if (overwrite) { clearOrders(); }
	else if (orderCounter == 0) { actionTimer = 0.00f; }
	// Check order limit
	if (orderCounter >= LIMIT_ORDERS - 1) {
		console.error << "[cUnit::addOrder_pickup] Order limit reached!" << endl;
	}
	// Adding new order
	order[orderCounter].type = ORDER_PICKUP;
	order[orderCounter].targetObject = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_PICKUP << globalId << target << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_harvest(int target, bool overwrite, int powerLevel)
{
	// Clearing the order list
	if (overwrite) { clearOrders(); }
	else if (orderCounter == 0) { actionTimer = 0.00f; }
	// Check order limit
	if (orderCounter >= LIMIT_ORDERS - 1) {
		console.error << "[cUnit::addOrder_harvest] Order limit reached!" << endl;
	}
	// Adding new order
	order[orderCounter].type = ORDER_HARVEST;
	order[orderCounter].targetObject = target;
	order[orderCounter].paramA = powerLevel;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_HARVEST << globalId << target << overwrite << powerLevel;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_packunit(int target, bool overwrite)
{
	// Clearing the order list
	if (overwrite) { clearOrders(); }
	else if (orderCounter == 0) { actionTimer = 0.00f; }
	// Check order limit
	if (orderCounter >= LIMIT_ORDERS - 1) {
		console.error << "[cUnit::addOrder_packunit] Order limit reached!" << endl;
	}
	// Adding new order
	order[orderCounter].type = ORDER_PACKUNIT;
	order[orderCounter].targetObject = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_PACKUNIT << globalId << target << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_death(bool overwrite)
{
	if (core.serverMode || core.localServer)
	{
		// Clearing the order list
		if (overwrite) { clearOrders(); }
		else if (orderCounter == 0) { actionTimer = 0.00f; }
		// Check order limit
		if (orderCounter >= LIMIT_ORDERS - 1) {
			console.error << "[cUnit::addOrder_death] Order limit reached!" << endl;
		}
		// Adding new order
		order[orderCounter].type = ORDER_DEATH;
		orderCounter += 1;

		// Update info
		updateFacing();
		updateAction();
		updateAnimation();

		// Server
		sf::Packet data;
		data << MSG_ORDER_DEATH << globalId << overwrite;
		server.sendPacket(PLAYERS_REMOTE, data);

		return orderCounter - 1;
	}
	return -1;
}

void cUnit::removeOrder(int id)
{
	for (int i = id; i < orderCounter - 1; i++)
	{
		order[i] = order[i + 1];
	}
	orderCounter -= 1;
	if (id == 0) { actionTimer = 0.00f; }
}

void cUnit::clearOrders()
{
	orderCounter = 0;
	actionTimer = 0.00f;
	// Reset progress bar
	if (globalId == client.unit && visual.progress.enabled) { visual.disableProgressBar(); }
}

void cUnit::removeAllOrders()
{
	orderCounter = 0;
	actionTimer = 0.00f;

	updateFacing();
	updateAction();
	updateAnimation();

	// Reset progress bar
	if (globalId == client.unit && visual.progress.enabled) { visual.disableProgressBar(); }
	// Server side
	if (core.serverSide())
	{
		sf::Packet data;
		data << MSG_ORDER_STOP << globalId;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}