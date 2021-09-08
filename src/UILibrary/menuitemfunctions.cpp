// Licensed under the MIT License.

#include <map>
#include <functional>
#include <string>
#include "menu.h"
#include "script.h"

bool initialized = false;
std::map<double, std::map<int, std::function<void(bool)>>> functions;

// This is where you will write your functions when an option is toggled/pressed

#pragma region Page 1
void ListOption1_1(bool toggle)
{
	ShowSubtitle("Option 2 Pressed");
}

void ListOption1_2(bool toggle)
{
	ShowSubtitle("Option 3 Pressed");
}
#pragma endregion

#pragma region Page 1.1
void ToggleOption1_0(bool toggle)
{
	ShowSubtitle(toggle == true ? "On" : "Off");
}

void ToggleOption1_1(bool toggle)
{
	ShowSubtitle(toggle == true ? "On" : "Off");
}
#pragma endregion

#pragma region Page 2
void ListOption2_0(bool toggle)
{
	ShowSubtitle("Option 1 Pressed");
}

void ListOption2_1(bool toggle)
{
	ShowSubtitle("Option 2 Pressed");
}

void ListOption2_2(bool toggle)
{
	ShowSubtitle("Option 3 Pressed");
}
#pragma endregion

bool func_exists(double pageIndex, int selectedIndex)
{
	if (functions[pageIndex][selectedIndex]) {
		return true;
	} else {
		ShowSubtitle("FUNCTION DOES NOT EXIST AT THIS INDEX (" + std::to_string(pageIndex).substr(0, 3) + ", " + std::to_string(selectedIndex) + ")");
		return false;
	}
}

void init(double pageIndex, int selectedIndex, bool toggle)
{
	// Page 1 - Option 2
	functions[1.0][1] = &ListOption1_1;
	// Page 1 - Option 3
	functions[1.0][2] = &ListOption1_2;

	// Page 1.1 - Toggle Option 1
	functions[1.1][0] = &ToggleOption1_0;
	functions[1.1][1] = &ToggleOption1_1;

	functions[2.0][0] = &ListOption2_0;
	functions[2.0][1] = &ListOption2_1;
	functions[2.0][2] = &ListOption2_2;

	// Continue to assign functions in the hashmap like I did above
	// Please rename the functions to your liking
	// !! I couldn't figure out a better way to do this !!

	initialized = true;
	if (func_exists(pageIndex, selectedIndex)) {
		functions[pageIndex][selectedIndex](toggle);
	}	
}

void CallFunction(double pageIndex, int selectedIndex, bool toggle)
{
	if (!initialized) {
		init(pageIndex, selectedIndex, toggle);
	} else {
		if (func_exists(pageIndex, selectedIndex)) {
			functions[pageIndex][selectedIndex](toggle);
		}
	}
}
