
#include "main.h"
#include "chat.h"
#include "saveload.h"

string cSave::getChunkFilePath(vec2i pos)
{
	return "Savefiles//" + save.savefileName + "//" + save.worldName + "//" + to_string(pos.x) + "-" + to_string(pos.y) + ".chunk";
}

void cSave::saveScreenshot(sf::Image img)
{
	string filename = util.getCurrentTimeString(true, true) + ".png";
	CreateDirectory(L"Screenshots", NULL);
	img.saveToFile("Screenshots//" + filename);
	chat.logMessage(LOGMSG_SCREENSHOT, cArg(filename));
	console << "[cSave::saveScreenshot] Screenshot saved as " << "Screenshots//" + filename << endl;
	util.screenshotRequested = false;
}