
#include "main.h"
#include "editor.h"
#include "game.h"
#include "math.h"

void cEditor::genBlueprint_Normal()
{
	float dist = 1000.00f, localDist;
	vec2 pos;
	game.clearUnits(FILTER_ONLYEDITOR);
	int total = 0;
	float randVal;
	// Background
	game.addUnit("bg_snow_", vec2f(0.00f, 0.00f), -1, math.rand(0, 3));
	// Objects
	for (int i = 0; i < math.rand(12, 20) && total < 100; i++)
	{
		dist = 1000.00f;
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2), math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2));
		for (int y = 0; y < game.unitCounter; y++)
		{
			localDist = math.getDistance(game.unit[y].pos, pos);
			if (localDist < dist) { dist = localDist; }
		}

		if (dist > 25.00f) {
			randVal = math.randf(0.00f, 1.00f);
			if (randVal < 0.40f) {
				game.addUnit("tree_basic_", pos, -1, math.rand(0, 3));
			}
			else if (randVal < 0.80f) {
				game.addUnit("tree_fir_", pos, -1, math.rand(0, 3));
			}
			else {
				game.addUnit("stone_basic_", pos, -1, math.rand(0, 3));
			}
		}
		else { i -= 1; }
		total += 1;
	}
	total = 0;
}

void cEditor::genBlueprint_Blocked()
{
	vec2 pos;
	game.clearUnits(FILTER_ONLYEDITOR);
	int total = 0;
	float randVal;
	// Background
	game.addUnit("bg_snow_", vec2f(0.00f, 0.00f), -1, math.rand(0, 3));
	// Objects
	for (int i = 0; i < math.rand(35, 50) && total < 100; i++)
	{
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2), math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2));
		randVal = math.randf(0.00f, 1.00f);
		if (randVal < 0.40f) {
			game.addUnit("tree_basic_", pos, -1, math.rand(0, 3));
		}
		else if (randVal < 0.80f) {
			game.addUnit("tree_fir_", pos, -1, math.rand(0, 3));
		}
		else {
			game.addUnit("stone_basic_", pos, -1, math.rand(0, 3));
		}
		total += 1;
	}
}

void cEditor::genBlueprint()
{
	if (autogenType == CHUNK_NORMAL) { genBlueprint_Normal(); }
	else if (autogenType == CHUNK_BLOCKED) { genBlueprint_Blocked(); }
	else { console << "[cEditor::genBlueprint] Unknown autogenType (" << autogenType << ")\n"; }
}