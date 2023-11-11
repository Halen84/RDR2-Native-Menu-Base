// Licensed under the MIT License.

#include "script.h"
#include "UI/Menu.hpp"
#include "UI/UIUtil.h"
#include "Submenus/Examples.h"

/*
#########################################################################
# RDR2 Native UI Menu Base
# Made by TuffyTown (Halen84)
# Menu base that is used to make similar menus to how they are in-game
# =======================================================================
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# GitHub Link: https://github.com/Halen84/RDR2-Native-Menu-Base
# License: MIT
#########################################################################
*/

/*

- script.cpp contains the main loop
- console.h contains useful macros that print things to the included debug console
- The "Submenus" folder is where you can create your submenus
- This folder also contains examples of how you might set submenus up.
- To change keybinds, see UI/Menu.cpp in CNativeMenu::CheckInput()
- UI/Options/ contains all option types
- UI/Menu.hpp is the CNativeMenu singleton class, which contains g_Menu global
- UI/Menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/Submenu.h is the base CSubmenu class, meant to be inherited from by your custom submenus. See Submenus/Examples.h and Submenus/EntryMenu.h
- UI/Drawing.cpp draws all text, textures, options, etc
- Note: EntryMenu.h/.cpp is the very first submenu

*/


void RegisterAdditionalSubmenus()
{
	// See EntryMenu.h on what this means/does
	CSubmenu::RegisterWithoutSingleton<CExamplesSubmenu_Time>();
	CSubmenu::RegisterWithoutSingleton<CExamplesSubmenu_Weather>();
}


void main()
{
	RegisterAdditionalSubmenus();

	// Creation of g_Menu calls CNativeMenu ctor, which automatically registers all submenus
	g_Menu = std::make_unique<CNativeMenu>();

	if (!UIUtil::GetScreenDimensions())
	{
		PRINT_WARN("Failed to get Red Dead Redemption 2 game window dimensions. The UI may be sized incorrectly.");
	}

	while (true)
	{
		g_Menu->Update();

		WAIT(0);
	}
}

#pragma warning(disable:28159)
void WaitAndDraw(unsigned ms)
{
	DWORD time = GetTickCount() + ms;
	bool waited = false;
	while (GetTickCount() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if (g_Menu) {
			g_Menu->Update();
		}
	}
}

void ScriptMain()
{
	main();
}
#pragma warning(default:28159)
