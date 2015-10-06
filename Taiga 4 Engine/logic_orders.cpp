
#include "main.h"
#include "logic.h"
#include "game.h"
#include "math.h"
#include "path.h"
#include "client.h"
#include "camera.h"
#include "UI.h"

void cGameLogic::updateOrders(int elapsedTime)
{
	bool unitRemoved = false;
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	float angle, offsetX, offsetY;
	for (int i = 0; i < game.unitCounter; i++)
	{
		unitRemoved = false;
		if (game.unit[i].orderCounter > 0)
		{
			// Action timer
			if (game.unit[i].actionTimer > 0.00f) {
				game.unit[i].actionTimer -= timevar;
				if (game.unit[i].actionTimer < 0.00f) { game.unit[i].actionTimer = 0.00f; }
			}
			else
			{
				// Move to point order
				if (game.unit[i].order[0].type == ORDER_MOVETO)
				{
					game.access.lock();
					// Calculating the movement speed
					float moveSpeed = game.unit[i].movementSpeed;
					if (!game.unit[i].hasRef(REF_UNIT_NOACCELERATION))
					{
						angle = math.convertAngle(math.getAngle(game.unit[i].pos, game.unit[i].order[0].targetPos));
						moveSpeed *= abs(cos((game.unit[i].facingAngle - angle) / 2.00f * math.DEGTORAD));
					}
					// Calculating the point
					if (math.getDistance(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y)
						> moveSpeed * timevar)
					{
						angle = math.getAngle(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y);
						offsetX = moveSpeed * timevar * cos(angle * math.DEGTORAD);
						offsetY = moveSpeed * timevar * sin(angle * math.DEGTORAD);
					}
					else
					{
						offsetX = game.unit[i].order[0].targetPos.x - game.unit[i].pos.x;
						offsetY = game.unit[i].order[0].targetPos.y - game.unit[i].pos.y;
						game.unit[i].removeOrder(0);
					}
					// Moving the unit to correct position
					// Check unit ref
					if (game.unit[i].hasRef(REF_UNIT_BESTPATHING))
					{
						// Validate the point before moving
						vec2f newPoint(game.unit[i].pos.x + offsetX, game.unit[i].pos.y + offsetY);
						if (path.isPointFree(newPoint, game.unit[i].collisionDistance, game.unit[i].globalId)) {
							game.unit[i].pos = newPoint;
						}
					}
					else
					{
						// Just move the unit
						game.unit[i].pos.x += offsetX;
						game.unit[i].pos.y += offsetY;
					}
					// Check the camera status
					if (game.unit[i].globalId == client.unit)
					{
						if (camera.lockedToCharacter) {
							camera.moveto(sf::Vector2f(game.unit[i].pos.x, game.unit[i].pos.y));
							camera.pos.x = math.round(camera.pos.x);
							camera.pos.y = math.round(camera.pos.y);
						}
						else if (camera.moveWithCharacter) {
							camera.move(sf::Vector2f(offsetX, offsetY));
						}
					}
					game.access.unlock();
					// Missile collision check
					if (game.unit[i].hasRef(REF_UNIT_MISSILE) && core.serverSide())
					{
						for (int y = 0; y < game.unitCounter; y++)
						{
							if (i != y && math.getDistance(&game.unit[i], &game.unit[y])
								<= game.getUnitCollisionDistance(&game.unit[i], &game.unit[y])
								&& game.unit[i].owner != game.unit[y].globalId)
							{
								if (game.unit[i].hasRef(REF_UNIT_DAMAGE_RIFLE)) { game.damageUnit(game.unit[y].globalId, value.rifleDamage); }

								unitRemoved = true;
								game.removeUnit(game.unit[i].globalId);
								y = game.unitCounter;
								i -= 1;
							}
						}
					}
				}
				// Unit pack order
				else if (game.unit[i].order[0].type == ORDER_PACKUNIT)
				{
					cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);
					if (core.serverMode || core.localServer) {
						game.packUnitToItem(target->globalId);
					}
					game.unit[i].removeOrder(0);
					ui.updateInterfaceItemList();
				}
				// Unit pickup order
				else if (game.unit[i].order[0].type == ORDER_PICKUP)
				{
					cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);

					if (core.serverMode || core.localServer) {
						for (int y = 0; y < target->container.itemCounter; y++) {
							game.unit[i].addItem(target->container.item[y].type, target->container.amount[y]);
						}
						game.removeUnit(game.unit[i].order[0].targetObject);
					}
					game.unit[i].removeOrder(0);
					ui.updateInterfaceItemList();
				}
				// Resource harvest
				else if (game.unit[i].order[0].type == ORDER_HARVEST)
				{
					if (core.serverSide())
					{
						float resFound;
						cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);
						// If at least some tool is used
						if (game.unit[i].order[0].paramA > 0)
						{
							resFound = game.getUnit(game.unit[i].order[0].targetObject).resource;
							game.unit[i].addResource(resFound);
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].addOrder_death();
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].order[0].paramA = game.unit[i].order[0].paramA;
						}
						// No tools used
						else
						{
							resFound = math.round(game.getUnit(game.unit[i].order[0].targetObject).resource);
							if (resFound == 275) { resFound = 75; }
							else if (resFound == 200) { resFound = 50; }
							else if (resFound == 150) { resFound = 30; }
							else if (resFound == 120) { resFound = 25; }
							else if (resFound == 95) { resFound = 20; }
							else if (resFound == 75) { resFound = 15; }
							else if (resFound >= 10) { resFound = 10; }
							else { resFound = 0; }
							game.unit[i].addResource(resFound);
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].addResource(-resFound);
						}
					}
					game.unit[i].removeOrder(0);
				}
				// Death
				else if (game.unit[i].order[0].type == ORDER_DEATH && core.serverSide())
				{
					int usedPowerLevel = game.unit[i].order[0].paramA;
					// Dropping container items
					if (game.unit[i].container.itemCounter > 0 || game.unit[i].drop.itemCounter > 0)
					{
						int itemId = game.addUnit("item_a", game.getUnit(game.unit[i].globalId).pos);
						cItemContainer cont = game.getUnit(game.unit[i].globalId).container;
						for (int a = 0; a < cont.itemCounter; a++)
						{
							game.getUnit(itemId).addItem(cont.item[a].type, cont.amount[a]);
						}
						game.unit[i].drop.flush(itemId, usedPowerLevel);
						// If nothing dropped, remove the unit
						if (game.getUnit(itemId).container.itemCounter == 0) {
							game.removeUnit(itemId);
						}
					}
					// Removing the unit
					unitRemoved = true;
					game.removeUnit(game.unit[i].globalId);
					i -= 1;
				}

				// If unit is not removed, update it
				if (!unitRemoved)
				{
					game.unit[i].updateFacing();
					game.unit[i].updateAction();
					game.unit[i].updateAnimation();
				}
			}
		}
		// No orders
		else
		{
			// Projectile destruction
			if (game.unit[i].hasRef(REF_UNIT_MISSILE))
			{
				unitRemoved = true;
				game.removeUnit(game.unit[i].globalId);
				i -= 1;
			}
		}
	}
}