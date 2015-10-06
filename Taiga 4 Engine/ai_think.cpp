
#include "main.h"
#include "ai.h"
#include "game.h"
#include "math.h"

void cAI::think_enemy(int globalId)
{
	cUnit* me = &game.getUnit(globalId);

	vector<int> idList;
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].type == "player")
		{
			idList.push_back(game.unit[i].globalId);
		}
	}
	if (idList.size() > 0)
	{
		int target = idList[math.rand(0, (int)idList.size() - 1)];

		if (math.getDistance(me, &game.getUnit(target)) < 50.00f)
		{
			game.getUnit(target).addHealth(-value.enemyDamage);
		}
		else { me->addOrder_moveto_path(game.getUnit(target).pos); }
	}
}

void cAI::think_rabbit(int globalId)
{
	cUnit* me = &game.getUnit(globalId);

	// Determine new goal
	if (me->ai.int_goal == AIGOAL_NONE || math.randf(0.00f, 1.00f) < 0.10f) {
		me->ai.int_goal = AIGOAL_WANDER;
	}
	// Flee
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].hasRef(REF_UNIT_SCARY) && math.getDistance(me, &game.unit[i]) < 250.00f)
		{
			me->ai.int_goal = AIGOAL_FLEE;
			me->ai.int_targetUnit = game.unit[i].globalId;
			break;
		}
	}

	// Act according to the goal
	if (me->ai.int_goal == AIGOAL_WANDER)
	{
		if (me->orderCounter == 0 && math.randf(0.00f, 1.00f) < 0.30f) {
			me->addOrder_moveto_path(me->pos + vec2f(-500.00f, 500.00f));
		}
	}
	else if (me->ai.int_goal == AIGOAL_FLEE)
	{
		cUnit* target = &game.getUnit(me->ai.int_targetUnit);
		me->addOrder_moveto_path(math.polar(target->pos, 350.00f, math.getAngle(target, me) + math.randf(-25.00f, 25.00f)));
	}
}