#pragma once
#include "common_submenu.h"

namespace Menu
{
	// Or something like CExamplesSubmenu

	class CExampleFuncs
	{
	public:
		static bool bPauseTime;
		void PauseTime();

		void SetTime();
		void SetWeather(Hash weatherType);
	};

	inline CExampleFuncs* g_ExampleFuncs{};
}
