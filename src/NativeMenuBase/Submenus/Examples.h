// Licensed under the MIT License.

#pragma once
#include "../UI/Menu.hpp"


//
// Also see EntryMenu.h/.cpp
//


class CExamplesSubmenu : public CSubmenu
{
public:
	CExamplesSubmenu()
	{
		Header = "EXAMPLES";
		SubHeader = "Examples";
		ID = eSubmenuID::Examples;
	}
	void Init() override;
};
inline CExamplesSubmenu g_ExamplesSubmenu{};


class CExamplesSubmenu_Time : public CSubmenu
{
public:
	CExamplesSubmenu_Time()
	{
		Header = "TIME";
		SubHeader = "Change Time";
		ID = eSubmenuID::Examples_Time;
	}
	void Init() override;
	void OnEnter() override;
	void OnExit() override;
	void OnTick() override;

public:
	static bool bPauseTime;

	void SetTime();
	void PauseTime();
};
//inline CExamplesSubmenu_Time g_ExamplesSubmenu_Time{}; // or g_TimeSubmenu, etc, Up to you


class CExamplesSubmenu_Weather : public CSubmenu
{
public:
	CExamplesSubmenu_Weather()
	{
		Header = "WEATHER";
		SubHeader = "Change Weather";
		ID = eSubmenuID::Examples_Weather;
		NumberOfVisibleOptions = 12; // Recommended max
	}
	void Init() override;

public:
	void SetWeather(unsigned weatherType);
};
//inline CExamplesSubmenu_Weather g_ExamplesSubmenu_Weather{}; // or g_WeatherSubmenu, etc, Up to you
