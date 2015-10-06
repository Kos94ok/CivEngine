
#include "main.h"
#include "util.h"

void cUtil::detectVideoCard()
{
	string vendor = reinterpret_cast< char const * >(glGetString(GL_VENDOR));
	if (vendor == "Intel") {
		core.videoCard = VIDEO_INTEL;
		console << "[MAIN] Detected Intel video card" << "\n";
	}
	else if (vendor.substr(0, 6) == "NVIDIA") {
		core.videoCard = VIDEO_NVIDIA;
		console << "[MAIN] Detected Nvidia video card" << "\n";
	}
	else if (vendor.substr(0, 3) == "ATI") { 
		core.videoCard = VIDEO_ATI;
		console << "[MAIN] Detected AMD video card" << "\n";
	}
	else if (vendor.substr(0, 9) == "Microsoft") {
		core.videoCard = VIDEO_MICROSOFT;
		console << "[MAIN] Detected Microsoft video card" << "\n";
	}
	else {
		core.videoCard = VIDEO_OTHER;
		console << "[MAIN] Unknown video card: " << vendor << "\n";
	}
}

int cUtil::getInventorySortingType()
{
	return SORT_BYNAME;
}

void cUtil::checkLog(string filename)
{
	int fileSize;
	ifstream file;
	ofstream clr;

	file.open(filename);
	if (file.good())
	{
		file.seekg(0, file.end);
		fileSize = (int)file.tellg();
		file.close();
		// Filesize is more than a megabyte
		if (fileSize > 1000000) {
			clr.open(filename);
			clr.close();
		}
	}
}

void cUtil::checkLogFiles()
{
	// Game mode
	if (!core.serverMode)
	{
		// Main log
		checkLog("Logs//game.txt");
		// Chat log
		checkLog("Logs//chat.txt");
	}
	// Server mode
	else
	{
		checkLog("Logs//server.txt");
	}
}

string cUtil::getCurrentTimeString(bool removeSemi, bool includeDay)
{
	time_t t = time(0);
	struct tm time;
	localtime_s(&time, &t);
	string retVal = "";
	if (includeDay)
	{
		// Day
		if (time.tm_mday < 10) {
			retVal += "0";
		}
		retVal += to_string(time.tm_mday);
		if (!removeSemi) { retVal += ":"; }
		// Month
		if (time.tm_mon < 9) {
			retVal += "0";
		}
		retVal += to_string(time.tm_mon + 1);
		if (!removeSemi) { retVal += ":"; }
		// Year
		retVal += to_string(time.tm_year - 100);
		// Divider
		retVal += "_";
	}
	// Hours
	if (time.tm_hour < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_hour);
	if (!removeSemi) { retVal += ":"; }
	// Minutes
	if (time.tm_min < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_min);
	if (!removeSemi) { retVal += ":"; }
	// Seconds
	if (time.tm_sec < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_sec);
	return retVal;
}

sf::Color cUtil::convertUnitHighlightColor(int code)
{
	switch (code)
	{
	case 0:
		return parseOldschoolColorCode("bbffcc");
		break;
	case 1:
		return parseOldschoolColorCode("00ffff");
		break;
	case 2:
		return parseOldschoolColorCode("00ff55");
		break;
	case 3:
		return parseOldschoolColorCode("ffff99");
		break;
	case 4:
		return parseOldschoolColorCode("ffcc99");
		break;
	case 5:
		return parseOldschoolColorCode("ffbbcc");
		break;
	case 6:
		return parseOldschoolColorCode("cccccc");
		break;
	case 7:
		return parseOldschoolColorCode("bbffff");
		break;
	}
	return sf::Color(255, 200, 50);
}

sf::Color cUtil::parseOldschoolColorCode(string code)
{
	sf::Color retVal;
	string val;
	int value, converted;
	for (int i = 0; i < 3; i++)
	{
		// First digit
		val = code.substr(i * 2, 1);
		value = 0;
		if (val == "a") { value += 16 * 10; }
		else if (val == "b") { value += 16 * 11; }
		else if (val == "c") { value += 16 * 12; }
		else if (val == "d") { value += 16 * 13; }
		else if (val == "e") { value += 16 * 14; }
		else if (val == "f") { value += 16 * 15; }
		else { stringstream(val) >> converted; value += converted * 16; }

		// Second digit
		val = code.substr(i * 2 + 1, 1);
		if (val == "a") { value += 10; }
		else if (val == "b") { value += 11; }
		else if (val == "c") { value += 12; }
		else if (val == "d") { value += 13; }
		else if (val == "e") { value += 14; }
		else if (val == "f") { value += 15; }
		else { stringstream(val) >> converted; value += converted; }

		// Flushing
		if (i == 0) { retVal.r = value; }
		if (i == 1) { retVal.g = value; }
		if (i == 2) { retVal.b = value; }
	}

	return retVal;
}

bool cUtil::intersects(vec2f pos, vec2f rectPos, vec2f rectSize)
{
	if (pos.x < rectPos.x) { return false; }
	if (pos.y < rectPos.y) { return false; }
	if (pos.x > rectPos.x + rectSize.x) { return false; }
	if (pos.y > rectPos.y + rectSize.y) { return false; }
	return true;
}

void cUtil::makeScreenshot() {
	screenshotRequested = true;
}

void cUtil::checkLaunchStatus()
{
	ifstream file;
	file.open("..//Taiga 4 Engine.sln");
	if (file.good()) {
		console << "[MAIN] Project file detected. Calculating statistics..." << endl;
		file.close();
		codeStats.calculate();
		console << "[MAIN] - Header files: " << codeStats.headerFiles << endl;
		console << "[MAIN] - Source files: " << codeStats.sourceFiles << endl;
		console << "[MAIN] - Total lines of code: " << codeStats.linesOfCode << endl;
	}
}

void cCodeStats::calculate()
{
	headerFiles = 0;
	sourceFiles = 0;
	linesOfCode = 0;

	ifstream file;
	char buf[256];

	vector<string> fileArray;
	WIN32_FIND_DATAA dataStruct;
	HANDLE hFind;

	hFind = FindFirstFileA("..//Taiga 4 Engine//*", &dataStruct);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (strlen(dataStruct.cFileName) > 2) { fileArray.push_back(dataStruct.cFileName); }
		while (FindNextFileA(hFind, &dataStruct))
		{
			if (strlen(dataStruct.cFileName) > 2) { fileArray.push_back(dataStruct.cFileName); }
		}
	}

	for (auto& filename : fileArray)
	{
		bool toParse = false;
		if (filename.substr(filename.length() - 2) == ".h") { headerFiles += 1; toParse = true; }
		else if (filename.substr(filename.length() - 4) == ".cpp") { sourceFiles += 1; toParse = true; }

		if (toParse) {
			file.open("..//Taiga 4 Engine//" + filename);
			while (file.good() && !file.eof())
			{
				file.getline(buf, 256);
				linesOfCode += 1;
			}
			file.close();
		}
	}
}

int cUtilTimer::getElapsedTimeForThread(int threadId)
{
	int retVal;
	int curTime = timeGetTime();
	if (curTime - threadTimer[threadId] > 1000) {
		retVal = 0;
	}
	else {
		retVal = curTime - threadTimer[threadId];
	}
	threadTimer[threadId] = curTime;
	return retVal;
}