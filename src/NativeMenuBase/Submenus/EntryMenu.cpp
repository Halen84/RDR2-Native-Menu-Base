// Licensed under the MIT License.

#include "EntryMenu.h"
#include "../UI/UIUtil.h"

bool CEntrySubmenu::bBooleanVariable = false;

void CEntrySubmenu::Init()
{
	// Note: Lambdas do not have to be used for the function parameter, a function reference also works.
	this->AddRegularOption("Regular Option", "Regular Option Footer", [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Regular Option~s~ executed");
	});

	this->AddBoolOption("Bool Option", "Bool Option Footer", &this->bBooleanVariable, [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Bool Option~s~ executed: " + std::string(bBooleanVariable == true ? "True" : "False"));
	});

	this->AddSubmenuOption("Submenu Option", "Submenu Option Footer", eSubmenuID::Examples);

	this->AddEmptyOption("Empty Option");

	this->AddVectorOption<int>("Vector Option 1", "Vector Option with an initializer list", { 0, 1, 2, 3, 4, 5 }, [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Vector Option 1~s~ executed");
	});

	this->AddVectorOption<int>("Vector Option 2", "Vector Option with preset vector index 3", { 0, 1, 2, 3, 4, 5 }, [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Vector Option 2~s~ executed");
	})->SetVectorIndex(3); // The "Add...Option" functions return a pointer to the option it created

	std::vector<const char*> vec = { "const", "char*", "vector" };
	this->AddVectorOption("Vector Option 3", "Vector Option with std::vector variable", vec, [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Vector Option 3~s~ executed");
	});

	this->AddVectorOption("Vector Option 4", "\"Static\" Vector Option", 10, "Start ", " End", [] {
		UIUtil::PrintSubtitle("~COLOR_BLUE~Vector Option 4~s~ executed");
	});
}

void CEntrySubmenu::OnEnter()
{
	// Do stuff when you enter this submenu
}

void CEntrySubmenu::OnExit()
{
	// Do stuff when you exit this submenu
}

void CEntrySubmenu::OnTick()
{
	// Do stuff every frame while currently in this submenu
}
