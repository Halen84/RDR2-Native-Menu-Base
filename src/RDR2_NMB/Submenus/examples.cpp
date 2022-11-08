#include "examples.hpp"
#include "../UI/util.h"

namespace Menu
{
	bool CExampleFuncs::bPauseTime = false;
	void CExampleFuncs::PauseTime()
	{
		CLOCK::PAUSE_CLOCK(bPauseTime, 0);
		if (bPauseTime)
			Util::PrintSubtitle("Time paused");
		else
			Util::PrintSubtitle("Time unpaused");
	}

	void CExampleFuncs::SetTime()
	{
		Option* option = g_Menu->GetSelectedOption();

		int hrs = CLOCK::GET_CLOCK_HOURS();
		int min = CLOCK::GET_CLOCK_MINUTES();
		int sec = CLOCK::GET_CLOCK_SECONDS();

		// Use switch case here because multiple options use this function
		switch (option->m_Index)
		{
		case 0:
			CLOCK::SET_CLOCK_TIME(option->m_VectorIndex, min, sec); // Change hour
			break;
		case 1:
			CLOCK::SET_CLOCK_TIME(hrs, option->m_VectorIndex, sec); // Change minute
			break;
		case 2:
			CLOCK::SET_CLOCK_TIME(hrs, min, option->m_VectorIndex); // Change second
			break;
		default:
			break;
		}
	}

	void CExampleFuncs::SetWeather(Hash weatherType)
	{
		Option* option = g_Menu->GetSelectedOption();

		MISC::SET_WEATHER_TYPE(weatherType, true, true, false, 0.0f, false);
		Util::PrintSubtitle("Set weather type to ~COLOR_OBJECTIVE~" + option->GetText() + "~s~");
	}
}
