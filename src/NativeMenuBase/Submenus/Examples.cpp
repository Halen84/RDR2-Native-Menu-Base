// Licensed under the MIT License.

#include "Examples.h"
#include "../UI/UIUtil.h"

void CExamplesSubmenu::Init()
{
	this->AddSubmenuOption("Change Time", "", eSubmenuID::Examples_Time);
	this->AddSubmenuOption("Change Weather", "", eSubmenuID::Examples_Weather);
}




bool CExamplesSubmenu_Time::bPauseTime = false;
void CExamplesSubmenu_Time::Init()
{
	// Pass in by reference so we can use "this" pointer
	this->AddVectorOption("Hour", "", 24, "", "", [&] { this->SetTime(); });
	this->AddVectorOption("Minute", "", 60, "", "", [&] { this->SetTime(); });
	this->AddVectorOption("Second", "", 60, "", "", [&] { this->SetTime(); });
	this->AddBoolOption("Pause Time", "", &bPauseTime, [&] { this->PauseTime(); });
}
void CExamplesSubmenu_Time::OnEnter()
{
	UIUtil::PrintSubtitle("[OnEnter] Entered Examples_Time");
}
void CExamplesSubmenu_Time::OnExit()
{
	UIUtil::PrintSubtitle("[OnExit] Exited Examples_Time");
}
void CExamplesSubmenu_Time::OnTick()
{
	// This function is being called every frame as long as we are in this submenu

	this->GetOption(0)->As<CVectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_HOURS());
	this->GetOption(1)->As<CVectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_MINUTES());
	this->GetOption(2)->As<CVectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_SECONDS());
}




void CExamplesSubmenu_Weather::Init()
{
	// In a real scenario, you should use enum hashes
	// for the weather types instead of GET_HASH_KEY

	this->AddRegularOption("High Pressure", "",		[&] { this->SetWeather(MISC::GET_HASH_KEY("HIGHPRESSURE")); });
	this->AddRegularOption("Rain", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("RAIN")); });
	this->AddRegularOption("Snow", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("SNOW")); });
	this->AddRegularOption("Misty", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("MISTY")); });
	this->AddRegularOption("Fog", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("FOG")); });
	this->AddRegularOption("Sunny", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("SUNNY")); });
	this->AddRegularOption("Clouds", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("CLOUDS")); });
	this->AddRegularOption("Overcast", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("OVERCAST")); });
	this->AddRegularOption("Thunderstorm", "",		[&] { this->SetWeather(MISC::GET_HASH_KEY("THUNDERSTORM")); });
	this->AddRegularOption("Hurricane", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("HURRICANE")); });
	this->AddRegularOption("Thunder", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("THUNDER")); });
	this->AddRegularOption("Shower", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("SHOWER")); });
	this->AddRegularOption("Blizzard", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("BLIZZARD")); });
	this->AddRegularOption("Snow Light", "",		[&] { this->SetWeather(MISC::GET_HASH_KEY("SNOWLIGHT")); });
	this->AddRegularOption("Whiteout", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("WHITEOUT")); });
	this->AddRegularOption("Hail", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("HAIL")); });
	this->AddRegularOption("Sleet", "",				[&] { this->SetWeather(MISC::GET_HASH_KEY("SLEET")); });
	this->AddRegularOption("Drizzle", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("DRIZZLE")); });
	this->AddRegularOption("Sandstorm", "",			[&] { this->SetWeather(MISC::GET_HASH_KEY("SANDSTORM")); });
	this->AddRegularOption("Overcast Dark", "",		[&] { this->SetWeather(MISC::GET_HASH_KEY("OVERCASTDARK")); });
	this->AddRegularOption("Ground Blizzard", "",	[&] { this->SetWeather(MISC::GET_HASH_KEY("GROUNDBLIZZARD")); });
}

#pragma region Time Submenu

void CExamplesSubmenu_Time::PauseTime()
{
	CLOCK::PAUSE_CLOCK(bPauseTime, 0);
	if (bPauseTime) {
		UIUtil::PrintSubtitle("Paused Time");
	}
	else {
		UIUtil::PrintSubtitle("Unpaused Time");
	}
}

void CExamplesSubmenu_Time::SetTime()
{
	CVectorOption* option = g_Menu->GetSelectedOption()->As<CVectorOption*>();

	int hrs = CLOCK::GET_CLOCK_HOURS();
	int min = CLOCK::GET_CLOCK_MINUTES();
	int sec = CLOCK::GET_CLOCK_SECONDS();

	// Use switch case here because multiple options use this function.
	// Dont do this unless this is the case.
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

#pragma endregion

#pragma region Weather Submenu

void CExamplesSubmenu_Weather::SetWeather(unsigned weatherType)
{
	COption* option = g_Menu->GetSelectedOption();

	MISC::SET_WEATHER_TYPE(weatherType, true, true, false, 0.0f, false);
	UIUtil::PrintSubtitle("Set weather type to ~COLOR_OBJECTIVE~" + option->Text + "~s~");
}

#pragma endregion
