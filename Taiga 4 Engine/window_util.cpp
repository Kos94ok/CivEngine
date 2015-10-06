
#include "main.h"
#include "window.h"
#include "camera.h"
#include "settings.h"
#include "game.h"

void cWindow::showWarning(int msg, bool critical)
{
	string message = "Unknown warning";
	if (msg == MSG_NOSHADERSUPPORT) { 
		message = "Shaders are not supported on your system. Lighting system and some other features are disabled.";
	}
	else if (msg == MSG_SHADERCOMPILE_UNIVERSAL) {
		message = "Universal shader compilation failed. Shadows are forced to \"low\". Some effects are disabled.";
	}
	else if (msg == MSG_SHADERCOMPILE_LIGHT) {
		message = "Light shader compilation failed. Lighting system is forced to \"low\".";
	}
	else if (msg == MSG_SHADERCOMPILE_OTHER) {
		message = "One or more shaders are failed to compile. Some effects may be lost";
	}
	else if (msg == MSG_TEXTURESIZE_LIMIT) {
		message = "Maximum texture size reached. Resolution will be reduced.";
	}
	
	// Windows routine
#ifdef SYSTEM_WINDOWS
	MessageBoxA(window.winHandle.getSystemHandle(), message.c_str(), "Warning", MB_ICONWARNING | MB_OK);
#endif
}

void cWindow::updateAspectRatio()
{
	float minX = 0.00f;
	float minY = 0.00f;
	float maxX = camera.res.x;
	float maxY = camera.res.y;
	
	winHandle.setView(sf::View(sf::FloatRect(minX, minY, maxX, maxY)));
}

sf::Vector2f cWindow::getMousePos(bool useMatrix)
{
	sf::Vector2i mousePosI = sf::Mouse::getPosition(winHandle);
	sf::Vector2f mousePos(mousePosI.x, mousePosI.y);

	// Screen resize fix
	mousePos.x *= (float)camera.res.x / (float)winHandle.getSize().x;
	mousePos.y *= (float)camera.res.y / (float)winHandle.getSize().y;

	if (useMatrix)
	{
		// Sample mod fix
		mousePos.x *= settings.sampleMod;
		mousePos.y *= settings.sampleMod;
		mutex.mainMatrix.lock();
		mousePos = matrixHandle.getInverse().transformPoint(mousePos);
		mutex.mainMatrix.unlock();
	}
	return mousePos;
}

sf::Vector2f cWindow::getScreenSize()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	return sf::Vector2f(desktop.right, desktop.bottom);
}
