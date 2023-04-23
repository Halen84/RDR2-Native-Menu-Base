// Licensed under the MIT License.

#pragma once
#include <windows.h>
#include "../common.hpp"

namespace UIUtil
{
	extern int g_screenWidth;
	extern int g_screenHeight;
	extern HWND g_windowHandle;

	bool GetScreenDimensions();

	void PrintSubtitle(const char* msg);
	void PrintSubtitle(const std::string& msg);
	void PrintSubtitle(const int msg);
	void PrintSubtitle(const float msg);
}
