// Licensed under the MIT License.

#pragma once
#include "SubmenuInclude.h"

class CExamplesSubmenu
{
public:
	static bool bPauseTime;

public:
	void Init();

public:
	
	void PauseTime();
	void SetTime();
	void SetWeather(Hash weatherType);
};

inline CExamplesSubmenu* g_ExamplesSubmenu{};
