
#include "main.h"
#include "unit.h"
#include "animation.h"
#include "logic.h"
#include "game.h"

void cUnit::updateDisplayAnim()
{
	int animId = anim.type;
	int curFrame = anim.curFrame;

	bool upDownAvailable = (animData[animId].up.tex != -1 && animData[animId].down.tex != -1);
	bool diagAvailable = (animData[animId].upDiag.tex != -1 && animData[animId].downDiag.tex != -1);

	// Right
	if (hasRef(REF_UNIT_ROTATE)
		|| ((diagAvailable && (facingAngle >= 337.50f || facingAngle < 22.50f))
		|| (upDownAvailable && !diagAvailable && (facingAngle >= 315.00f || facingAngle < 45.00f))
		|| (!upDownAvailable && (facingAngle >= 270.00 || facingAngle < 90.00f))))
	{
		animDisplay.data = animData[animId].side;
		animDisplay.texMod = 1;
	}
	// Up diagonal right
	else if (diagAvailable && (facingAngle >= 22.50f && facingAngle < 67.50f))
	{
		animDisplay.data = animData[animId].upDiag;
		animDisplay.texMod = 1;
	}
	// Up
	else if ((diagAvailable && (facingAngle >= 67.50f && facingAngle < 112.50f))
		|| (upDownAvailable && !diagAvailable && (facingAngle >= 45.00f && facingAngle < 135.00f)))
	{
		animDisplay.data = animData[animId].up;
		animDisplay.texMod = 1;
	}
	// Up diagonal left
	else if (diagAvailable && (facingAngle >= 112.50f && facingAngle < 157.50f))
	{
		animDisplay.data = animData[animId].upDiag;
		animDisplay.texMod = -1;
	}
	// Left
	else if ((diagAvailable && (facingAngle >= 157.50f && facingAngle < 202.50f))
		|| (upDownAvailable && !diagAvailable && (facingAngle >= 135.00f && facingAngle < 225.00f))
		|| (!upDownAvailable && (facingAngle >= 90.00f || facingAngle < 270.00f)))
	{
		animDisplay.data = animData[animId].side;
		animDisplay.texMod = -1;
	}
	// Down diagonal left
	else if (diagAvailable && (facingAngle >= 202.50f && facingAngle < 247.50f))
	{
		animDisplay.data = animData[animId].downDiag;
		animDisplay.texMod = -1;
	}
	// Down
	else if ((diagAvailable && (facingAngle >= 247.50f && facingAngle < 292.50f))
		|| (upDownAvailable && !diagAvailable && (facingAngle >= 225.00f && facingAngle < 315.00f)))
	{
		animDisplay.data = animData[animId].down;
		animDisplay.texMod = 1;
	}
	// Down diagonal right
	else if (diagAvailable && (facingAngle >= 292.50f && facingAngle < 337.50f))
	{
		animDisplay.data = animData[animId].downDiag;
		animDisplay.texMod = 1;
	}
}

cCurrentAnim::cCurrentAnim()
{
	reset();
}

void cCurrentAnim::reset()
{
	type = ANIM_IDLE;
	curFrame = 0;
	curFrameTimer = 0.00f;
	game.getUnit(ownerUnit).updateDisplayAnim();
}

void cCurrentAnim::play(int animType, bool forceInterrupt)
{
	// Don't interrupt the animation if not needed
	if (animType != type || forceInterrupt)
	{
		reset();
		type = animType;
		game.getUnit(ownerUnit).updateDisplayAnim();
	}
}