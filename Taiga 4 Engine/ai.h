
#pragma once
void AICoreMain();

#define AIGOAL_NONE					-1
#define AIGOAL_WANDER				0
#define AIGOAL_FLEE					1
#define AIGOAL_KILL					2

class cAIBrain
{
public:
	// Core
	bool enabled;
	int thinkTimer;
	function<void(int globalId)> thinkFunc;

	// Intelligence
	int int_goal;
	int int_targetUnit;
	vec2f int_attractedTo;

	// Constructors
	cAIBrain() {
		int_goal = -1;
		thinkTimer = 0;
		enabled = false;
	}
	cAIBrain(function<void(int globalId)> function) {
		enabled = true;
		thinkFunc = function;
	}
	cAIBrain& operator= (function<void(int globalId)> function) {
		enabled = true;
		thinkFunc = function;

		return *this;
	}
};

class cAI
{
public:
	bool enabled;

	int localTime;
	
	// Perform regular actions
	void tick(int elapsedTime);
	// Enable AI
	void enable() { enabled = true; }
	// Disable AI
	void disable() { enabled = false; }

	// Thinking functions
	static void think_enemy(int globalId);
	static void think_rabbit(int globalId);

	cAI()
	{
		enabled = false;
		localTime = 0;
	}
};

extern cAI ai;