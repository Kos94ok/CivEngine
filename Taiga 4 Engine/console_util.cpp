
#include "main.h"
#include "console.h"
#include "camera.h"
#include "settings.h"

void cConsole::show()
{
	displayed = true;
#ifdef SYSTEM_WINDOWS
	//ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
}

void cConsole::hide()
{
	displayed = false;
	clearInput();
#ifdef SYSTEM_WINDOWS
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

void cConsole::toggle()
{
	if (!displayed) { show(); displayedPage = SUBCMD_ALL; }
	else if (displayedPage == SUBCMD_ALL && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_INFO; }
	else if (displayedPage == SUBCMD_INFO && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ECHO; }
	else if (displayedPage == SUBCMD_ECHO && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ERROR; }
	else if (displayedPage == SUBCMD_ERROR && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_DEBUG; }
	else if (displayedPage == SUBCMD_DEBUG && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ALL; }
	else { hide(); }
	scrollOffset = 0;
}

int cConsole::getLineCount()
{
	return floor((camera.res.y - (float)settings.consoleFontSize * 1.50) / (settings.consoleFontSize + settings.consoleLineSpacing));
}

void cConsole::scroll(int direction)
{
	access.lock();
	scrollOffset += direction * settings.consoleScrollSpeed;
	if (scrollOffset < 0) { scrollOffset = 0; }
	if (scrollOffset > max(0, (int)history[displayedPage].size() - getLineCount())) { scrollOffset = max(0, (int)history[displayedPage].size() - getLineCount()); }
	access.unlock();
}

void cConsole::resetScroll()
{
	scrollOffset = 0;
}

void cConsole::clear()
{
	history[console.displayedPage].clear();
	resetScroll();
	if (displayedPage == SUBCMD_ALL)
	{
		history[SUBCMD_ALL].push_back("[CONSOLE] All Messages Tab");
		history[SUBCMD_ALL].push_back("============================");
	}
	else if (displayedPage == SUBCMD_INFO)
	{
		history[SUBCMD_INFO].push_back("[CONSOLE] Info Messages Tab");
		history[SUBCMD_INFO].push_back("============================");
	}
	else if (displayedPage == SUBCMD_ECHO)
	{
		history[SUBCMD_ECHO].push_back("[CONSOLE] Echo Messages Tab");
		history[SUBCMD_ECHO].push_back("============================");
	}
	else if (displayedPage == SUBCMD_ERROR)
	{
		history[SUBCMD_ERROR].push_back("[CONSOLE] Error Messages Tab");
		history[SUBCMD_ERROR].push_back("============================");
	}
	else if (displayedPage == SUBCMD_DEBUG)
	{
		history[SUBCMD_DEBUG].push_back("[CONSOLE] Debug Messages Tab");
		history[SUBCMD_DEBUG].push_back("============================");
	}
}