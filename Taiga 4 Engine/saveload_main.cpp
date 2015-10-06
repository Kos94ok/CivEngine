
#include "main.h"
#include "saveload.h"
#include "math.h"

void cSave::flushListToFile(cBlueprintHeader header, vector<cUnitEntry> list, string filename)
{
	ofstream file;
	file.open(filename);
	if (!file.good()) { console.error << "[cSave::flushListToFile] Can't open file " + filename << endl; return; }
	// Writing data
	file << "Type: " << header.type << "\n";
	file << "[Units]" << "\n";
	for (int i = 0; i < (int)list.size(); i++)
	{
		// Writing normal data
		file << "ID: " << list[i].globalId << "\n";
		file << "Type: " << list[i].type << "\n";
		file << "X: " << math.round(list[i].pos.x) << "\n";
		file << "Y: " << math.round(list[i].pos.y) << "\n";
		// Writing file data
		for (int y = 0; y < (int)list[i].itemList.size(); y++)
		{
			// Normal data
			file << "Type_Item: " << list[i].itemList[y].type << "\n";
			file << "Count_Item: " << list[i].itemList[y].count << "\n";
			// Item end line
			file << "---" << "\n";
		}
		// Unit end line
		file << "===" << "\n";
	}
	file.close();
}

cBlueprintHeader cSave::getHeaderFromFile(string filename)
{
	cBlueprintHeader retVal;
	char buffer[256];
	string buf;
	cUnitEntry entry;
	ifstream file;
	file.open(filename);
	if (file.good())
	{
		file.getline(buffer, 256);	buf = buffer;
		// Reading until some category appears
		while (buf.substr(0, 1) != "[" && !file.eof())
		{
			// If the line is ok, parsing the line
			if (buf.length() > 0)
			{
				if (buf.substr(0, 4) == "Type")
				{
					retVal.type = math.stringToInt(buf.substr(5));
				}
			}
			else { console.error << "[cSave::getHeaderFromFile] Unexpected end of file (" << filename << ")\n"; retVal.isValid = false; }
			// Reading the next line
			file.getline(buffer, 256);	buf = buffer;
		}
		// Closing
		file.close();
	}
	return retVal;
}

vector<cUnitEntry> cSave::getListFromFile(string filename)
{
	vector<cUnitEntry> retVal;
	char buffer[256];
	string buf;
	cUnitEntry entry;
	cItemEntry itemEntry;
	ifstream file;
	file.open(filename);
	if (file.good())
	{
		// Skipping to unit lines
		do
		{
			file.getline(buffer, 256);
			buf = buffer;
		} while (!file.eof() && buf != "[Units]");
		// Reading units
		while (!file.eof())
		{
			file.getline(buffer, 256);	buf = buffer;
			// Global Id
			if (buf.find("ID:") != -1) { entry.globalId = math.stringToInt(buf.substr(4)); }
			// Type
			else if (buf.find("Type:") != -1) { entry.type = buf.substr(6); }
			// Pos X
			else if (buf.find("X:") != -1) { entry.pos.x = math.stringToInt(buf.substr(3)); }
			// Pos Y
			else if (buf.find("Y:") != -1) { entry.pos.y = math.stringToInt(buf.substr(3)); }
			// Item type
			else if (buf.find("Type_Item:") != -1) {
				itemEntry.type = buf.substr(11);
				itemEntry.count = 1;
			}
			// Item count
			else if (buf.find("Count_Item:") != -1) {
				itemEntry.count = math.stringToInt(buf.substr(12));
			}
			// Pushing the item
			else if (buf.find("---") != -1) { entry.itemList.push_back(itemEntry); }
			// Pushing the unit
			else if (buf.find("===") != -1) { retVal.push_back(entry); entry.itemList.clear(); }
		}
		// Closing
		file.close();
	}
	return retVal;
}