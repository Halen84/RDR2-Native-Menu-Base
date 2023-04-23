// Licensed under the MIT License.

#include "UIUtil.h"
#include "../script.h"

// TODO: Remove this file maybe. Seems useless except for PrintSubtitle()

namespace UIUtil
{
	int g_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int g_screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND g_windowHandle = NULL;

	bool FindGameWindow()
	{
		g_windowHandle = FindWindowA(0, "Red Dead Redemption 2"); // sgaWindow
		return g_windowHandle != NULL;
	}

	bool GetScreenDimensions()
	{
		if (!FindGameWindow()) {
			PRINT_ERROR("Failed to find Red Dead Redemption 2 game window");
			return false;
		}

		RECT windowRect;
		if (!GetWindowRect(g_windowHandle, &windowRect)) {
			PRINT_ERROR("GetWindowRect() failed");
			return false;
		}

		g_screenWidth = windowRect.right - windowRect.left;
		g_screenHeight = windowRect.bottom - windowRect.top;

		return true;
	}

	void PrintSubtitle(const char* msg)
	{
		UILOG::_UILOG_SET_CACHED_OBJECTIVE(msg);
		UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
		UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
		UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
	}

	void PrintSubtitle(const std::string& msg) { PrintSubtitle(msg.c_str()); }
	void PrintSubtitle(const int msg) { PrintSubtitle(std::to_string(msg).c_str()); }
	void PrintSubtitle(const float msg) { PrintSubtitle(std::to_string(msg).c_str()); }
}
