#include "..\..\inc\main.h"
#include "UI/menu.hpp" // ALLOCATE_CONSOLE
#include "script.h"
#include "keyboard.h"
#include <iostream>

#ifdef ALLOCATE_CONSOLE
#if ALLOCATE_CONSOLE
void AllocateConsole(const char* title)
{
	AllocConsole();
	SetConsoleTitleA(title);

	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCout, "CONOUT$", "w", stderr);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
}
#endif
#endif

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
#if ALLOCATE_CONSOLE
		AllocateConsole("NativeMenuBaseDebugConsole");
#endif
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
#if ALLOCATE_CONSOLE
		FreeConsole();
#endif
		break;
	}

	return TRUE;
}
