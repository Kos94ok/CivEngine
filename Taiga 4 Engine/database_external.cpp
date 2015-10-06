
#include "main.h"
#include "database.h"
#include "math.h"

bool cDatabaseExternal::getFileList()
{
	string filename;
	HANDLE hFile;
	WIN32_FIND_DATAA dataStruct;

	hFile = FindFirstFileA("Data//Database//*", &dataStruct);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			filename = dataStruct.cFileName;
			if (filename.length() > 2 && filename.substr(filename.length() - 3) == ".db")
			{
				fileList.push_back("Data//Database//" + filename);
			}
		} while (FindNextFileA(hFile, &dataStruct));
	}
	return true;
}

bool cDatabaseExternal::parseFile(int index)
{
	ifstream file;
	cDatabaseKey key;
	string valueStr; int valueInt;
	file.open(fileList[index]);

	if (file.good())
	{
		// Parsing variables
		string cmd_activeType = "UNIT";
		string cmd_activeObject = "missingno";

		while (!file.eof())
		{
			// Getting values
			getNextKey(&file, &key);
			valueStr = key.value;
			valueInt = math.stringToInt(key.value);
			// Parsing the line
			if (key.name == "cmd_activeType") { cmd_activeType = valueStr; }
			if (key.name == "cmd_activeObject") { cmd_activeObject = valueStr; }
			// Unit-specific
			if (cmd_activeType == "UNIT")
			{
				if (key.name == "size_x") { database.getUnit(cmd_activeObject).size.x = valueInt; }
				else if (key.name == "size_y") { database.getUnit(cmd_activeObject).size.y = valueInt; }
				else if (key.name == "center_x") { database.getUnit(cmd_activeObject).center.x = valueInt; }
				else if (key.name == "center_y") { database.getUnit(cmd_activeObject).center.y = valueInt; }
				else if (key.name == "shadow_x") { database.getUnit(cmd_activeObject).shadowOffset.x = valueInt; }
				else if (key.name == "shadow_y") { database.getUnit(cmd_activeObject).shadowOffset.y = valueInt; }
				else if (key.name == "interact_distance") { database.getUnit(cmd_activeObject).interactDistance = valueInt; }
				else if (key.name == "collision_distance") { database.getUnit(cmd_activeObject).collisionDistance = valueInt; }
			}
			// End of parsing
		}
		file.close();
		return true;
	}
	else { console << "[cDatabaseExternal::parseFile] Can't read the file " << fileList[index] << endl; }
	return false;
}

void cDatabase::loadExternal()
{
	ext.getFileList();

	for (int i = 0; i < (int)ext.fileList.size(); i++)
	{
		ext.parseFile(i);
	}
}

bool cDatabaseExternal::getNextKey(ifstream *file, cDatabaseKey *key)
{
	char buffer[256];
	string buf = buffer;

	do
	{
		file->getline(buffer, 256);
		buf = buffer;
		// Ignore comments and categories
		if (buf.substr(0, 2) != "//" && buf.substr(0, 1) != "[" && buf.length() > 0)
		{
			int search = buf.find(" = ");
			// Double space
			if (search != -1)
			{
				key->name = buf.substr(0, search);
				key->value = buf.substr(search + 3);
				return true;
			}
			else
			{
				search = buf.find(" =");
				// Single space before equality
				if (search != -1)
				{
					key->name = buf.substr(0, search);
					key->value = buf.substr(search + 2);
					return true;
				}
				else
				{
					search = buf.find("= ");
					// Single space after equality
					if (search != -1)
					{
						key->name = buf.substr(0, search);
						key->value = buf.substr(search + 2);
						return true;
					}
					else
					{
						search = buf.find("=");
						// No spaces
						if (search != -1)
						{
							key->name = buf.substr(0, search);
							key->value = buf.substr(search + 1);
							return true;
						}
					}
				}
			}
		}
	} while (!file->eof());

	return false;
}