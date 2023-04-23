#include "..\..\inc\main.h"
#include "console.h"
#include "script.h"
#include "keyboard.h"
#include <iostream>

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
