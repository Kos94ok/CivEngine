
#include "main.h"
#include "chat.h"
#include "UI.h"
#include "database.h"

void cChat::logMessage(int msg, cArg args)
{
	if (core.serverMode) { return; }
	int argi[4];
	string type, name;
	switch (msg)
	{
	case LOGMSG_ITEM_ADD:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Obtained: " << database.getItem(type).displayName << " x" << argi[1] << endl;
		break;
	case LOGMSG_ITEM_REMOVE:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Lost: " << database.getItem(type).displayName << " x" << argi[1] << endl;
		break;
	case LOGMSG_SCREENSHOT:
		// Args: Filename
		name = args[0];
		log << "Screenshot saved as " << name << endl;
		break;
	case LOGMSG_RESOURCE_ADD:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Obtained: Resource x" << argi[1] << endl;
		break;
	case LOGMSG_RESOURCE_REMOVE:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Lost: Resource x" << argi[1] << endl;
		break;
	case LOGMSG_PROGRESS_HARVEST:
		log << "Harvesting..." << endl;
		break;
	case LOGMSG_PROGRESS_PACK:
		log << "Packing..." << endl;
		break;
	}

	ui.updateChatWindow();
	chat.noFocusTimer = value.chatNoFocusTimer;
}