
#pragma once
#include "precomp.h"
#include "item.h"

class cComponent
{
public:
	std::string type;
	int count;
	bool consume;

	cComponent() {
		type = "";
		count = 1;
		consume = true;
	}

	cComponent(std::string item) {
		type = item;
		count = 1;
		consume = true;
	}

	cComponent(std::string item, bool toReturn) {
		type = item;
		count = 1;
		consume = !toReturn;
	}

	cComponent(const char* item) {
		type = item;
		count = 1;
		consume = true;
	}

	cComponent(std::string item, int amount) {
		type = item;
		count = amount;
		consume = true;
	}

	cComponent(std::string item, int amount, bool toReturn) {
		type = item;
		count = amount;
		consume = !toReturn;
	}

	cComponent& operator=(std::string item) {
		type = item;
		count = 1;
		consume = true;
		return *this;
	}
};

class cRecipe
{
public:
	cComponent ingr[LIMIT_COMPONENTS];
	cComponent result;
	int ingrCount;

	int resourceBalance;

	cRecipe() {
		ingrCount = 1;
		resourceBalance = 0;
	}
};

class cCraft
{
public:
	cItemContainer cont;
	cItemContainer resultCont;
	int recipeCounter;
	cRecipe recipe[LIMIT_RECIPES];

	int selectedRecipe;

	cCraft() {
		selectedRecipe = 0;
		recipeCounter = 0;
		cont.itemLimit = LIMIT_COMPONENTS;
		resultCont.itemLimit = 1;
	}

	void addRecipe(cComponent result, int resBalance,  cComponent ingrA,
				cComponent ingrB = "", cComponent ingrC = "", cComponent ingrD = "", cComponent ingrE = "",
				cComponent ingrF = "", cComponent ingrG = "", cComponent ingrH = "", cComponent ingrI = "");
	void checkActiveRecipe();
	void updateCraftAmounts();
	void loadRecipes();
	void getResult(bool craftAll);
	void clearGhostItems();
	int getActiveRecipeId();
	int getActiveRecipeRepeats(bool ignoreResource = false);
	int getActiveRecipeCount();
	bool isItemInRecipe(int recId, std::string type);
};

extern cCraft craft;