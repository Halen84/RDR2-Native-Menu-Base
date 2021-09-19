// Licensed under the MIT License.

#include <map>
#include <functional>
#include <string>
#include "menu.h"
#include "script.h"

bool initialized = false;
// TODO: VARIABLE NUMBER OF ARGS FOR MORE FUNCTIONALITY
std::map<double, std::map<int, std::function<void(bool, int)>>> functions;

// This is where you will write your functions when an option is toggled/pressed

#pragma region Page 1
// Parameters like this means they're unused
void ListOption1_1(bool, int)
{
	ShowSubtitle("Option 2 Pressed");
}

void ListOption1_2(bool, int)
{
	ShowSubtitle("Option 3 Pressed");
}
#pragma endregion

#pragma region Page 1.1
void ToggleOption1_0(bool, int index)
{
	ShowSubtitle(std::to_string(index));
}

void ToggleOption1_1(bool, int index)
{
	ShowSubtitle(std::to_string(index));
}

void ToggleOption1_2(bool toggle, int)
{
	ShowSubtitle(toggle == true ? "On" : "Off");
}
#pragma endregion

#pragma region Page 2
void ListOption2_0(bool, int)
{
	ShowSubtitle("Option 1 Pressed");
}

void ListOption2_1(bool, int)
{
	ShowSubtitle("Option 2 Pressed");
}

void ListOption2_2(bool, int)
{
	ShowSubtitle("Option 3 Pressed");
}
#pragma endregion


bool func_exists(double pageIndex, int selectedIndex)
{
	if (functions[pageIndex][selectedIndex]) {
		return true;
	} else {
		ShowSubtitle("FUNCTION DOES NOT EXIST AT THIS INDEX (" + std::to_string(pageIndex).substr(0, 4) + ", " + std::to_string(selectedIndex) + ")");
		return false;
	}
}


void init(double pageIndex, int selectedIndex, bool toggle, int index)
{
	// Page 1 - Option 2
	functions[1.0][1] = &ListOption1_1;
	// Page 1 - Option 3
	functions[1.0][2] = &ListOption1_2;

	// Page 1.1 - Toggle Option 1
	functions[1.1][0] = &ToggleOption1_0;
	functions[1.1][1] = &ToggleOption1_1;
	functions[1.1][2] = &ToggleOption1_2;

	functions[2.0][0] = &ListOption2_0;
	functions[2.0][1] = &ListOption2_1;
	functions[2.0][2] = &ListOption2_2;

	// Continue to assign functions in the hashmap like I did above
	// Please rename the functions to your liking
	// !! I couldn't figure out a better way to do this !!

	initialized = true;
	if (func_exists(pageIndex, selectedIndex)) {
		functions[pageIndex][selectedIndex](toggle, index);
	}	
}


void CallFunction(double pageIndex, int selectedIndex, bool toggle, int index)
{
	if (!initialized) {
		init(pageIndex, selectedIndex, toggle, index);
	} else {
		if (func_exists(pageIndex, selectedIndex)) {
			functions[pageIndex][selectedIndex](toggle, index);
		}
	}
}
