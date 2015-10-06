
#include "main.h"
#include "item.h"
#include "game.h"
#include "database.h"
#include "math.h"

void cItem::setName(string txt)
{
	displayName = txt;
}

void cItem::addDescrLine(string line)
{
	description[descrLinesUsed++] = line;
	//description.push_back(line);
	//descrLinesUsed += 1;
}

cItem newItem;
int cItemContainer::add(cItem target, int count)
{
	int search = getId(target.type);
	if (search != -1)
	{
		amount[search] += count;
		//sort(SORT_BYNAME);
		return search;
	}
	else if (itemCounter < LIMIT_ITEMS && itemCounter < itemLimit)
	{
		newItem = target;
		newItem.globalId = game.itemGlobalCounter++;
		item.push_back(newItem);
		//item[itemCounter] = target;
		//item[itemCounter].globalId = game.itemGlobalCounter++;
		amount.push_back(count);
		itemCounter += 1;
		//sort(SORT_BYNAME);
		return itemCounter - 1;
	}
	return -1;
}

int cItemContainer::add(std::string type, int count)
{
	return add(database.getItem(type), count);
}

cItem& cItemContainer::get(int id)
{
	id = getId(id);
	if (id != -1) { return item[id]; }
	return database.item[0];
}

cItem& cItemContainer::get(std::string type, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == type) { return item[i]; }
	}
	return database.item[0];
}

bool cItemContainer::remove(int id, int count)
{
	id = getId(id);
	if (id != -1)
	{
		if (count == -1 || amount[id] <= count)
		{
			/*for (int i = id; i < itemCounter - 1; i++)
			{
				item[i] = item[i + 1];
				amount[i] = amount[i + 1];
			}*/
			item.erase(item.begin() + id);
			amount.erase(amount.begin() + id);
			itemCounter -= 1;
		}
		else { amount[id] -= count; }
		return true;
	}
	return false;
}

bool cItemContainer::remove(string type, int count, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == type)
		{
			remove(item[i].globalId, count);
			return true;
		}
	}
	return false;
}

void cItemContainer::clear()
{
	item.clear();
	amount.clear();
	itemCounter = 0;
}

int cItemContainer::getId(int id)
{
	for (int i = 0; i < itemCounter; i++)
	{
		if (item[i].globalId == id && !item[i].hasRef(REF_ITEM_GHOST)) { return i; }
	}
	return -1;
}

int cItemContainer::getId(string id, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == id && !item[i].hasRef(REF_ITEM_GHOST)) { return i; }
	}
	return -1;
}

int cItemContainer::getAmount(string type)
{
	int id = getId(type);
	if (id == -1) { return 0; }
	return amount[id];
}

cItem ex_item[LIMIT_ITEMS];
int ex_amount[LIMIT_ITEMS];
bool ex_ignore[LIMIT_ITEMS];
void cItemContainer::sort(int sortType)
{
	game.access.lock();
	for (int i = 0; i < LIMIT_ITEMS; i++) { ex_ignore[i] = false; }
	int itemsFound = 0, currentBestItem = -1;
	string bestName, name;
	do
	{
		currentBestItem = -1;
		bestName = "zzzzzzzzzzzzzz";
		// Checking all the item names
		for (int i = 0; i < itemCounter; i++)
		{
			if (!ex_ignore[i])
			{
				if (sortType == SORT_BYNAME)
				{
					name = item[i].displayName;
					for (int y = 0; y < (int)min(name.length(), bestName.length()); y++)
					{
						// New name is better
						if ((int)name.c_str()[y] < (int)bestName.c_str()[y])
						{
							bestName = name;
							currentBestItem = i;
							break;
						}
						// Best name is still the best
						else if ((int)name.c_str()[y] > (int)bestName.c_str()[y])
						{
							break;
						}
					}
				}
			}
		}
		if (currentBestItem != -1) {
			ex_item[itemsFound] = item[currentBestItem];
			ex_amount[itemsFound] = amount[currentBestItem];
			ex_ignore[currentBestItem] = true;
			itemsFound += 1;
		}
	} while (currentBestItem != -1);

	for (int i = 0; i < itemCounter; i++)
	{
		item[i] = ex_item[i];
		amount[i] = ex_amount[i];
	}
	game.access.unlock();
}

void cDropContainer::add(std::string type, int count, float chance, int powerLevel)
{
	this->item[itemCounter] = type;
	this->amount[itemCounter] = count;
	this->chance[itemCounter] = chance;
	this->powerLevel[itemCounter] = powerLevel;
	itemCounter += 1;
}

int cDropContainer::flush(int unitId, int usedPowerLevel)
{
	cItemDrop drop;
	std::vector<cItemDrop> dropList;
	for (int i = 0; i < itemCounter; i++)
	{
		if (usedPowerLevel >= powerLevel[i] && math.randf(0.00f, 1.00f) <= chance[i])
		{
			drop.type = item[i];
			drop.count = amount[i];
			dropList.push_back(drop);
		}
	}
	return game.createDrop(vec2f(0.00f, 0.00f), dropList, unitId);
}

int cDropContainer::flush(vec2f pos, int usedPowerLevel)
{
	cItemDrop drop;
	std::vector<cItemDrop> dropList;
	for (int i = 0; i < itemCounter; i++)
	{
		if (usedPowerLevel >= powerLevel[i] && math.randf(0.00f, 1.00f) <= chance[i])
		{
			drop.type = item[i];
			drop.count = amount[i];
			dropList.push_back(drop);
		}
	}
	return game.createDrop(pos, dropList);
}