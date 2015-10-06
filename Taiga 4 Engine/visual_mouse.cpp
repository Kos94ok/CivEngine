
#include "visual.h"
#include "math.h"
#include "database.h"
#include "window.h"
#include "target.h"
#include "camera.h"
#include "console.h"

void cVisual::setMousePointer(int type)
{
	mouse.type = type;
	if (mouse.type == POINTER_HARDWARE) { mouse.showHardware(); }
	else if (mouse.type == POINTER_DEFAULT) {
		mouse.texture = database.texture[TEX_MOUSE_DEFAULT];
		mouse.texturePress = database.texture[TEX_MOUSE_DEFAULTPRESS];
	}
	else if (mouse.type == POINTER_POINT) {
		mouse.texture = database.texture[TEX_MOUSE_POINT];
		mouse.texturePress = database.texture[TEX_MOUSE_POINT];
	}
	else if (mouse.type == POINTER_TARGET) {
		mouse.texture = database.texture[TEX_MOUSE_TARGET];
		mouse.texturePress = database.texture[TEX_MOUSE_TARGET];
	}
	else if (mouse.type == POINTER_NONE) {
		mouse.texture = addTexture("alpha.png");
		mouse.texturePress = addTexture("alpha.png");
	}

	// Hide hardware pointer
	mouse.hideHardware();
	if (mouse.type != POINTER_HARDWARE) { mouse.hideHardware(); }
}

void cMousePointer::showHardware() { window.winHandle.setMouseCursorVisible(true); }
void cMousePointer::hideHardware() { window.winHandle.setMouseCursorVisible(false); }

vec2f cMousePointer::getOrigin()
{
	vec2f retVal = vec2f(0.00f, 0.00f);
	vec2i texSize;
	if (type == POINTER_TARGET || type == POINTER_POINT) {
		texSize = getTexSize();
		retVal = vec2f(texSize.x / 2.00f, texSize.y / 2.00f);
	}
	return retVal;
}

vec2i cMousePointer::getTexSize()
{
	sf::Vector2u size = visual.gameTex[texture].handle.getSize();
	return vec2i(size.x, size.y);
}


void cVisual::enableProgressBar(float time)
{
	progress.enabled = true;
	progress.manualMode = false;
	progress.timeCurrent = 0.00f;
	progress.timeMaximum = time;
	progress.fadeTimer = 0.00f;
	setMousePointer(POINTER_NONE);
}

void cVisual::enableProgressBarManual(float time, int stepCount)
{
	enableProgressBar(time);
	progress.manualMode = true;
	progress.manualModeStep = time / stepCount;
}

void cVisual::advanceProgressBar()
{
	progress.timeCurrent += progress.manualModeStep;
	if (progress.timeCurrent > progress.timeMaximum) {
		progress.timeCurrent = progress.timeMaximum;
	}
}

int cProgressBar::getTexFrameCountX() { return 9; }
int cProgressBar::getTexFrameCountY() { return 8; }

float cProgressBar::getCurrentAlpha()
{
	if (enabled) {
		return fadeTimer / value.progressBarFadeInTimer;
	}
	else {
		return fadeTimer / value.progressBarFadeOutTimer;
	}
}

vec2i cProgressBar::getCurrentFrame()
{
	int frameLimitX = getTexFrameCountX();
	int frameLimitY = getTexFrameCountY();

	float pos = timeCurrent / timeMaximum;
	int curFrame = math.round(pos * (frameLimitX * frameLimitY));
	int curFrameY = curFrame / frameLimitX;
	int curFrameX = curFrame - curFrameY * frameLimitX;
	if (curFrame >= frameLimitX * frameLimitY) {
		curFrameX = frameLimitX - 1;
		curFrameY = frameLimitY - 1;
	}

	return vec2i(curFrameX, curFrameY);
}

vec2i cProgressBar::getTexFrameSize()
{
	sf::Vector2u fullSize = visual.gameTex[database.texture[TEX_MOUSE_PROGRESS]].handle.getSize();
	return vec2i(fullSize.x / getTexFrameCountX(), fullSize.y / getTexFrameCountY());
}

void cVisual::disableProgressBar()
{
	progress.enabled = false;
	progress.fadeTimer = value.progressBarFadeOutTimer;
	setMousePointer(POINTER_DEFAULT);
}

void cVisual::registerMouseClick()
{
	lastMouseClickPos = window.getMousePos(false);
	lastMouseClickTimer = value.mouseDoubleClickTimer;
}

bool cVisual::isDoubleClick()
{
	if (lastMouseClickTimer <= 0.00f) { return false; }
	if (math.getDistance(window.getMousePos(false), lastMouseClickPos) > 1.00f) { return false; }
	return true;
}

void cVisual::unitHoverScroll(int delta)
{
	if (hoveredUnit == -1) { return; }

	mutex.renderUnits.lock();
	for (int i = 0; i < (int)unitHoverQueue.size(); i++)
	{
		if (unitHoverQueue[i] == hoveredUnit) {
			hoveredUnit = unitHoverQueue[i + delta];
			if (i + delta >= (int)unitHoverQueue.size()) {
				console.debug << "[DEBUG] Test 1: " << delta << endl;
				hoveredUnit = unitHoverQueue[i + delta - (int)unitHoverQueue.size()];
			}
			else if (i + delta < 0) {
				console.debug << "[DEBUG] Test 2: " << delta << endl;
				hoveredUnit = unitHoverQueue[i + delta + (int)unitHoverQueue.size()];
			}
			break;
		}
	}
	mutex.renderUnits.unlock();
}