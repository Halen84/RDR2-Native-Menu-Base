#include "util.h"
#include "../script.h"

// TODO: Rewrite this file or something. Seems useless except for PrintSubtitle()

namespace Menu::Util
{
	int g_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int g_screenHeight = 1080;
	HWND g_windowHandle = NULL;

	bool SetResolution()
	{
		g_windowHandle = FindWindowA(0, "Red Dead Redemption 2"); // grcWindow sgaWindow
		RECT windowRect = {};

		if (GetWindowRect(g_windowHandle, &windowRect)) {
			g_screenWidth = windowRect.right - windowRect.left;
			g_screenHeight = windowRect.bottom - windowRect.top;
			return true;
		}

		return false;
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
