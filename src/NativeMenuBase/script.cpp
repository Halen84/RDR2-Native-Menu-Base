// Licensed under the MIT License.

#include "script.h"
#include "common.h"
#include "UI/Menu.h"
#include "UI/UIUtil.h"

/*
#########################################################################
# RDR2 Native UI Menu Base
# Branch: immediate
# Made by TuffyTown (Halen84)
# Menu base that is used to make similar menus to how they are in-game
# =======================================================================
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# GitHub Link: https://github.com/Halen84/RDR2-Native-Menu-Base
# License: MIT
#########################################################################
*/

static void Update()
{
	// Since this is the root menu, no label or footer is displayed. Leave blank.
	if (CMenu::BeginSubmenu("", "", "HEADER", "SUB HEADER", SubmenuId::Root))
	{
		if (CMenu::Option("Option", "Basic option example")) {
			UIUtil::PrintSubtitle("Option pressed");
		}

		static const char* items[] = { "1", "2", "3", "4", "5" };
		static int currentItem = 0;
		if (CMenu::ListOption("List Option", "List option example", &currentItem, items, _countof(items))) {
			UIUtil::PrintSubtitle("List option pressed");
		}

		CMenu::Separator("Separator");

		static bool toggle = false;
		if (CMenu::CheckboxOption("Checkbox", "Checkbox option example", &toggle)){
			UIUtil::PrintSubtitle("Checkbox pressed");
		}

		if (CMenu::BeginSubmenu("Child Submenu", "Click me", "SUBMENU1", "SUBMENU1", SubmenuId::Submenu1))
		{
			if (CMenu::Option("Option 1", "Basic option example")) {
				UIUtil::PrintSubtitle("Option 1 pressed");
			}

			if (CMenu::Option("Option 2", "Basic option example")) {
				UIUtil::PrintSubtitle("Option 2 pressed");
			}

			CMenu::EndSubmenu();
		}

		CMenu::EndSubmenu();
	}
}

void main()
{
	if (!UIUtil::GetScreenDimensions()) {
		PRINT_WARN("Failed to get RDR2 game window dimensions. The UI may be sized incorrectly.");
	}

	while (true)
	{
		CMenu::BeginFrame();
		Update();
		CMenu::EndFrame();

		WAIT(0);
	}
}

void ScriptMain()
{
	main();
}
