#pragma once
#include "../script.h"
#include "../common.hpp"
#include "../UI/menu.hpp"

namespace Menu
{
	class CExampleFuncs
	{
	public:
		static bool bPauseTime;
		void PauseTime();

		void SetTime();
		void SetWeather();
	};

	inline CExampleFuncs* g_ExampleFuncs{};
}
