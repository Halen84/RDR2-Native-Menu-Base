// Licensed under the MIT License.

#include "Examples.hpp"

bool CExamplesSubmenu::bPauseTime = false;

void CExamplesSubmenu::Init()
{
	// Here, we will create the submenus

	g_Menu->AddSubmenu("EXAMPLES", "Function Examples", Submenu_Examples, 8, [](Submenu* sub)
	{
		sub->AddSubmenuOption("Change Weather", "", Submenu_Examples_Weather);
		sub->AddSubmenuOption("Change Time", "", Submenu_Examples_Time);
	});


	g_Menu->AddSubmenu("EXAMPLES", "Change Weather", Submenu_Examples_Weather, 12, [](Submenu* sub)
	{
		// In a real scenario, you should use enum hashes
		// for the weather types instead of GET_HASH_KEY

		sub->AddRegularOption("High Pressure", "",		[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("HIGHPRESSURE")); });
		sub->AddRegularOption("Rain", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("RAIN")); });
		sub->AddRegularOption("Snow", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SNOW")); });
		sub->AddRegularOption("Misty", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("MISTY")); });
		sub->AddRegularOption("Fog", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("FOG")); });
		sub->AddRegularOption("Sunny", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SUNNY")); });
		sub->AddRegularOption("Clouds", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("CLOUDS")); });
		sub->AddRegularOption("Overcast", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("OVERCAST")); });
		sub->AddRegularOption("Thunderstorm", "",		[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("THUNDERSTORM")); });
		sub->AddRegularOption("Hurricane", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("HURRICANE")); });
		sub->AddRegularOption("Thunder", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("THUNDER")); });
		sub->AddRegularOption("Shower", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SHOWER")); });
		sub->AddRegularOption("Blizzard", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("BLIZZARD")); });
		sub->AddRegularOption("Snow Light", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SNOWLIGHT")); });
		sub->AddRegularOption("Whiteout", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("WHITEOUT")); });
		sub->AddRegularOption("Hail", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("HAIL")); });
		sub->AddRegularOption("Sleet", "",				[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SLEET")); });
		sub->AddRegularOption("Drizzle", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("DRIZZLE")); });
		sub->AddRegularOption("Sandstorm", "",			[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("SANDSTORM")); });
		sub->AddRegularOption("Overcast Dark", "",		[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("OVERCASTDARK")); });
		sub->AddRegularOption("Ground Blizzard", "",	[] { g_ExamplesSubmenu->SetWeather(MISC::GET_HASH_KEY("GROUNDBLIZZARD")); });
	});


	// If you were to implement this in examples.cpp, you wouldn't need to use a lambda.
	// But once again, this is just here for simplicity reasons.
	g_Menu->AddSubmenu("EXAMPLES", "Change Time", Submenu_Examples_Time, 8, [](Submenu* sub)
	{
		sub->AddVectorOption("Hour", "", 24, "", "",	[] { g_ExamplesSubmenu->SetTime(); });
		sub->AddVectorOption("Minute", "", 60, "", "",	[] { g_ExamplesSubmenu->SetTime(); });
		sub->AddVectorOption("Second", "", 60, "", "",	[] { g_ExamplesSubmenu->SetTime(); });
		sub->AddBoolOption("Pause Time", "", &bPauseTime, [] { g_ExamplesSubmenu->PauseTime(); });
	});
}


void CExamplesSubmenu::PauseTime()
{
	CLOCK::PAUSE_CLOCK(bPauseTime, 0);
	if (bPauseTime)
		UIUtil::PrintSubtitle("Time paused");
	else
		UIUtil::PrintSubtitle("Time unpaused");
}


void CExamplesSubmenu::SetTime()
{
	// We know that GetSelectedOption() is a vector option, so convert it to that.
	// Need to convert to access VectorOption specific functions. e.g GetVectorIndex()
	VectorOption* option = g_Menu->GetSelectedOption()->As<VectorOption*>();

	int hrs = CLOCK::GET_CLOCK_HOURS();
	int min = CLOCK::GET_CLOCK_MINUTES();
	int sec = CLOCK::GET_CLOCK_SECONDS();

	// Use switch case here because multiple options in the submenu use this function
	switch (option->Index)
	{
		case 0:
			CLOCK::SET_CLOCK_TIME(option->GetVectorIndex(), min, sec); // Change hour
			break;
		case 1:
			CLOCK::SET_CLOCK_TIME(hrs, option->GetVectorIndex(), sec); // Change minute
			break;
		case 2:
			CLOCK::SET_CLOCK_TIME(hrs, min, option->GetVectorIndex()); // Change second
			break;
		default:
			break;
	}
}


void CExamplesSubmenu::SetWeather(Hash weatherType)
{
	Option* option = g_Menu->GetSelectedOption();

	MISC::SET_WEATHER_TYPE(weatherType, true, true, false, 0.0f, false);
	UIUtil::PrintSubtitle("Set weather type to ~COLOR_OBJECTIVE~" + option->Text + "~s~");
}
