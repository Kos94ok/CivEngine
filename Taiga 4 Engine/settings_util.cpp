
#include "main.h"
#include "settings.h"

bool cSettings::getNextKey(ifstream *file, cSettingsKey *key)
{
	char buffer[256];
	string buf = buffer;

	do
	{
		file->getline(buffer, 256);
		buf = buffer;
		// Ignore comments and categories
		if (buf.substr(0, 2) != "//" && buf.substr(0, 1) != "[")
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
	}
	while (!file->eof());

	return false;
}