
#pragma once
#include "precomp.h"
#include "game.h"
#include "UI.h"

class cAPI
{
public:
	/* 
	Function returns a real handle of unit with specific ID.
	In case a unit is not found, the "missingno" unit is
	returned. The "missingno" type should never be changed.
	*/
	cUnit& unitHandle(int id, bool isGlobal = true);
	/*
	Function returns a separate copy of unit with specific ID.
	Modifying the values of this unit does not have any effect
	and therefore must be used only for reading. If the unit
	is not found, the "missingno" unit is returned.
	*/
	cUnit unitCopy(int id, bool isGlobal = true);
	/*
	Functions returns a real handle of last created unit.
	*/
	cUnit& lastCreatedUnit();
	/*
	Function returns local controlled unit. Any changes with
	this unit should only be done when the server is in
	local mode.
	*/
	cUnit& controlledUnit();
	/*
	The unit gets the order to move to specified point. The
	pathfinding system is used to detect the proper path. If
	the point is unreachable, unit receives a stop order.
	If overwrite == true, the order is executed immediately.
	Otherwise it is added to the queue.
	*/
	void issueMoveOrder(sf::Vector2f pos, int id, bool overwrite = true, bool isGlobal = true);
	/*
	The unit stops immediately and cancels all the orders.
	*/
	void issueStopOrder(int id, bool isGlobal = true);
	/*
	Function returns a real handle of element with specific ID.
	In case a unit is not found, the "missingno" element is
	returned. The "missingno" type should never be changed.
	*/
	cUIElement& elementHandle(int id);
	/*
	Function returns a separate copy of element with specific ID.
	Modifying the values of this element does not have any effect
	and therefore must be used only for reading. If the element
	is not found, the "missingno" element is returned.
	*/
	cUIElement elementCopy(int id);
	/*
	Function returns a real handle of last created UI element.
	*/
	cUIElement& lastCreatedElement();
};

extern cAPI api;