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
void ExampleBtn1(bool, int)
{
	ShowSubtitle("Example Button 1 Pressed");
}

void ExampleBtn2(bool, int)
{
	ShowSubtitle("Example Button 2 Pressed");
}
#pragma endregion


#pragma region Page 1.1
void ToggleOption0(bool, int index)
{
	// Menu::GetToggleSelection(GetCurrentPageIndex(), 0) will return same value as index
	ShowSubtitle(std::to_string(index));
}

void ToggleOption1(bool, int index)
{
	// Menu::GetToggleSelection(GetCurrentPageIndex(), 1) will return same value as index
	ShowSubtitle(std::to_string(index));
}
#pragma endregion


#pragma region Page 2.1
void ChangeWeather(bool, int index)
{
	Hash weatherType;
	std::string weatherName = Menu::GetTextAtCurrentSelection();

	// Set weatherType using switch case
	switch (index)
	{
		case 0:
			weatherType = MISC::GET_HASH_KEY("HIGHPRESSURE");
			break;
		case 1:
			weatherType = MISC::GET_HASH_KEY("RAIN");
			break;
		case 2:
			weatherType = MISC::GET_HASH_KEY("SNOW");
			break;
		case 3:
			weatherType = MISC::GET_HASH_KEY("MISTY");
			break;
		case 4:
			weatherType = MISC::GET_HASH_KEY("FOG");
			break;
		case 5:
			weatherType = MISC::GET_HASH_KEY("SUNNY");
			break;
		case 6:
			weatherType = MISC::GET_HASH_KEY("CLOUDS");
			break;
		case 7:
			weatherType = MISC::GET_HASH_KEY("OVERCAST");
			break;
		case 8:
			weatherType = MISC::GET_HASH_KEY("THUNDERSTORM");
			break;
		case 9:
			weatherType = MISC::GET_HASH_KEY("HURRICANE");
			break;
		case 10:
			weatherType = MISC::GET_HASH_KEY("THUNDER");
			break;
		case 11:
			weatherType = MISC::GET_HASH_KEY("SHOWER");
			break;
		case 12:
			weatherType = MISC::GET_HASH_KEY("BLIZZARD");
			break;
		case 13:
			weatherType = MISC::GET_HASH_KEY("SNOWLIGHT");
			break;
		case 14:
			weatherType = MISC::GET_HASH_KEY("WHITEOUT");
			break;
		case 15:
			weatherType = MISC::GET_HASH_KEY("HAIL");
			break;
		case 16:
			weatherType = MISC::GET_HASH_KEY("SLEET");
			break;
		case 17:
			weatherType = MISC::GET_HASH_KEY("DRIZZLE");
			break;
		case 18:
			weatherType = MISC::GET_HASH_KEY("SANDSTORM");
			break;
		case 19:
			weatherType = MISC::GET_HASH_KEY("OVERCASTDARK");
			break;
		case 20:
			weatherType = MISC::GET_HASH_KEY("GROUNDBLIZZARD");
			break;
		default:
			weatherType = MISC::GET_HASH_KEY("SUNNY");
			break;
	}

	MISC::SET_WEATHER_TYPE(weatherType, true, true, false, 0.0f, false);
	ShowSubtitle("Changed weather to: " + weatherName);
}
#pragma endregion


bool func_exists(double pageIndex, int selectedIndex)
{
	if (functions[pageIndex][selectedIndex]) {
		return true;
	} else {
		ShowSubtitle("~COLOR_RED~FUNCTION DOES NOT EXIST AT THIS INDEX (" + std::to_string(pageIndex).substr(0, 4) + ", " + std::to_string(selectedIndex) + ")~s~");
		return false;
	}
}


void init(double pageIndex, int selectedIndex, bool toggle, int index)
{
	// Page 1
	functions[1.0][1] = &ExampleBtn1;
	functions[1.0][2] = &ExampleBtn2;

	// Page 1.1
	functions[1.1][0] = &ToggleOption0;
	functions[1.1][1] = &ToggleOption1;

	for (int i = 0; i <= 20; i++) {
		// Page 2.1
		functions[2.1][i] = &ChangeWeather;
	}

	// Continue to assign functions like I did above where [pageIndex][index]
	// !! I couldn't figure out a better way to do this. This needs to be improved !!

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
