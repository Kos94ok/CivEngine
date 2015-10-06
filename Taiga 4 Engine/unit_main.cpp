
#include "main.h"
#include "unit.h"
#include "craft.h"
#include "UI.h"
#include "server.h"
#include "math.h"
#include "chat.h"
#include "client.h"
#include "visual.h"

bool cCharacter::animAvailable(int animType)
{
	return (animData[animType].side.tex != -1);
}

void cUnit::addResource(float d, bool sendMessage) { setResource(resource + d, sendMessage); }

void cUnit::setResource(float value, bool sendMessage)
{
	value = min(value, resourceLimit);
	int delta = value - resource;
	resource = value;

	craft.checkActiveRecipe();
	ui.updateInterfaceItemList();

	if (sendMessage && (core.serverMode || core.localServer))
	{
		sf::Packet data;
		data << MSG_UNIT_SETRESOURCE << globalId << value;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
	// Add to chat log
	if (sendMessage && globalId == client.unit) {
		if (delta >= 0) { chat.logMessage(LOGMSG_RESOURCE_ADD, cArg(type, to_string(delta))); }
		else { chat.logMessage(LOGMSG_RESOURCE_REMOVE, cArg(type, to_string(-delta))); }
	}
}

void cUnit::moveTo(vec2 newPos)
{
	float delta = max(pos.x - newPos.x, pos.y - newPos.y);
	pos = newPos;

	if ((core.serverMode || core.localServer) && abs(delta) > 0.005f)
	{
		sf::Packet data;
		data << MSG_UNIT_MOVETO << globalId << pos.x << pos.y;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::rotateTo(float newAngle)
{
	float delta = targetFacingAngle - newAngle;
	targetFacingAngle = newAngle;

	if ((core.serverMode || core.localServer) && abs(delta) > 0.01f)
	{
		sf::Packet data;
		data << MSG_UNIT_ROTATETO << globalId << newAngle;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::addHealth(float d) { setHealth(health + d); }
void cUnit::addMaxHealth(float d) { setMaxHealth(maxHealth + d); }

void cUnit::setHealth(float hp)
{
	health = hp;

	ui.updateInterface();

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_SETHEALTH << globalId << hp;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::setMaxHealth(float hp)
{
	maxHealth = hp;

	ui.updateInterface();

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_SETMAXHEALTH << globalId << hp;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::addItem(string type, int count, bool toLog)
{
	container.add(type, count);
	container.sort(util.getInventorySortingType());
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_ADDITEM << globalId << type << count << toLog;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
	// Add to chat log
	if (globalId == client.unit && toLog) {
		chat.logMessage(LOGMSG_ITEM_ADD, cArg(type, to_string(count)));
	}
}

void cUnit::removeItem(string type, int count, bool toLog)
{
	container.remove(type, count);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEITEM << globalId << type << count << toLog;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
	// Add to chat log
	if (globalId == client.unit && toLog) {
		chat.logMessage(LOGMSG_ITEM_REMOVE, cArg(type, to_string(count)));
	}
}

void cUnit::removeItem(int id, int count, bool toLog)
{
	container.remove(id, count);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEITEM << globalId << container.get(id).type << count << toLog;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
	// Add to chat log
	if (globalId == client.unit && toLog) {
		chat.logMessage(LOGMSG_ITEM_REMOVE, cArg(container.get(id).type, to_string(count)));
	}
}

void cUnit::addBuff(int type, float duration, int power, int owner)
{
	buff.add(type, duration, power, owner);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_ADDBUFF << globalId << type << duration << power << owner;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cUnit::addBuff(cBuff entry)
{
	addBuff(entry.type, entry.duration, entry.power, entry.ownerUnit);
}

void cUnit::removeBuff(int type)
{
	buff.remove(type);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEBUFF << globalId << type;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

bool cUnit::hasBuff(int type)
{
	return buff.getPower(type) > 0;
}

void cUnit::setLifeTimer(float time)
{
	lifeTimer.enabled = true;
	lifeTimer.time = time;
}

void cUnit::resetLifeTimer()
{
	lifeTimer.enabled = false;
}

void cUnit::updateFacing()
{
	if (orderCounter == 0) { return; }
	float angle;

	if (order[0].type == ORDER_MOVETO) {
		angle = math.getAngle(pos.x, pos.y, order[0].targetPos.x, order[0].targetPos.y);
	}
	else {
		angle = math.getAngle(pos.x, pos.y, game.getUnit(order[0].targetObject).pos.x, game.getUnit(order[0].targetObject).pos.y);
	}
	rotateTo(math.convertAngle(angle));
}

void cUnit::updateAction()
{
	if (orderCounter == 0 || actionTimer > 0.00f) { return; }
	
	int logMsg = -1;
	if (order[0].type == ORDER_PICKUP) { actionTimer = 0.00f; }
	else if (order[0].type == ORDER_HARVEST)
	{
		logMsg = LOGMSG_PROGRESS_HARVEST;
		// Selecting the work time
		if (order[0].paramA == POWER_HAND) { actionTimer = 3.00f; }
		else if (order[0].paramA == POWER_STONE) { actionTimer = 10.00f; }
		else if (order[0].paramA == POWER_FLINT) { actionTimer = 8.00f; }
		else if (order[0].paramA == POWER_IRON) { actionTimer = 6.00f; }
		else if (order[0].paramA == POWER_STEEL) { actionTimer = 5.00f; }
	}
	else if (order[0].type == ORDER_PACKUNIT) { actionTimer = 6.00f; logMsg = LOGMSG_PROGRESS_PACK; }
	else if (order[0].type == ORDER_DEATH)
	{
		if (animAvailable(ANIM_DEATH))
		{
			// Force the animation
			anim.play(ANIM_DEATH);
			actionTimer = (animDisplay.data.frameCount - 1) * animDisplay.data.frameDelay;
		}
	}
	// Creating the progress bar
	if (globalId == client.unit && actionTimer > 0.00f) {
		visual.enableProgressBar(actionTimer);
		if (logMsg != -1) { chat.logMessage(logMsg, NULL); }
	}
}

void cUnit::updateAnimation()
{
	if (orderCounter == 0) { anim.play(ANIM_IDLE); }
	else
	{
		if (order[0].type == ORDER_MOVETO && animAvailable(ANIM_MOVE)) { anim.play(ANIM_MOVE); }
		else if (order[0].type == ORDER_PICKUP && animAvailable(ANIM_WORK)) { anim.play(ANIM_WORK); }
		else if (order[0].type == ORDER_PACKUNIT && animAvailable(ANIM_WORK)) { anim.play(ANIM_WORK); }
		else if (order[0].type == ORDER_HARVEST && animAvailable(ANIM_WORK)) { anim.play(ANIM_WORK); }
		else if (order[0].type == ORDER_DEATH && animAvailable(ANIM_DEATH)) { anim.play(ANIM_DEATH); }
	}
}