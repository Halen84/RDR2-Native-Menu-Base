// Licensed under the MIT License.


#include "menu.h"
#include "script.h"


// This is where you will write your functions when an option is toggled/pressed
// Remember to add your new functions to function.h!


#pragma region Page 1
void ExampleBtn1()
{
	ShowSubtitle("Example Button 1 Pressed");
}

void ExampleBtn2()
{
	ShowSubtitle("Example Button 2 Pressed");
}
#pragma endregion


#pragma region Page 1.1
void ToggleOption0()
{
	int pos = Menu::GetToggleSelection(*GetCurrentPageIndex(), 0);
	ShowSubtitle(std::to_string(pos) + " - Toggle Option Vector");
}

void ToggleOption1()
{
	int pos = Menu::GetToggleSelection(*GetCurrentPageIndex(), 1);
	ShowSubtitle(std::to_string(pos) + " - Toggle Option Static");
}
#pragma endregion


#pragma region Page 2.1
void ChangeWeather()
{
	int index = *GetCurrentSelectedIndex();
	Hash weatherType;
	std::string weatherName = Menu::GetTextAtCurrentSelection();

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

void ChangeTime()
{
	int selectedIndex = *GetCurrentSelectedIndex();
	int pos = Menu::GetToggleSelection(*GetCurrentPageIndex(), selectedIndex);

	int hrs = CLOCK::GET_CLOCK_HOURS();
	int min = CLOCK::GET_CLOCK_MINUTES();
	int sec = CLOCK::GET_CLOCK_SECONDS();

	switch (selectedIndex)
	{
		case 0:
			CLOCK::SET_CLOCK_TIME(pos, min, sec); // Change hour
			break;
		case 1:
			CLOCK::SET_CLOCK_TIME(hrs, pos, sec); // Change minute
			break;
		case 2:
			CLOCK::SET_CLOCK_TIME(hrs, min, pos); // Change second
			break;
		default:
			break;
	}
}
#pragma endregion
