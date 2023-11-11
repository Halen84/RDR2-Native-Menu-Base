// Licensed under the MIT License.

#include "script.h"
#include "common.hpp"
#include "UI/Menu.hpp"
#include "UI/UIUtil.h"
#include "Submenus/Examples.hpp"

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
HELP & INFO:

- The UI will look and function the best on 1920x1080 displays
- The "Submenus" folder is where you can put your functions for options. Its up to you how you want to organize everything.
- ^^^ You can also create the options in other files as long as you call it in InitializeMenu()
- To change keybinds, see UI/Menu.cpp in CheckInput()
- UI/Options/ contains all option types
- UI/Submenu.hpp is the Submenu class
- UI/Menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/Menu.hpp is the CNativeMenu class
- UI/Drawing.cpp does the texture and text drawing for everything
*/


bool exampleBool = false;
const std::vector<std::string> exampleOptionVector = { "First", "Second", "Third", "Last" };
void InitializeMenu()
{
	g_Menu->AddSubmenu("HEADER", "Sub Header", Submenu_EntryMenu, 8, [](Submenu* sub) 
	{
			sub->AddRegularOption("Regular Option", "Regular Option Example", [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Regular~s~ option function executed");
			});

			sub->AddBoolOption("Bool Option", "Bool Option Example", &exampleBool, [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Bool~s~ option function executed. Boolean is " + std::string(exampleBool ? "True" : "False"));
			});

			sub->AddVectorOption("Vector Option 1", "Vector Option", exampleOptionVector, [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Vector option 1~s~ function executed.");
			});

			sub->AddVectorOption<int>("Vector Option 2", "Vector Option with an initializer list", { 0, 1, 2, 3, 4, 5 }, [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Vector option 2~s~ function executed.");
			});

			sub->AddVectorOption<int>("Vector Option 3", "Vector Option with preset vector index 3", { 0, 1, 2, 3, 4, 5 }, [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Vector option 3~s~ function executed.");
			})->SetVectorIndex(3);

			// 0 based indexing, 0 --> 9 is 10 options
			sub->AddVectorOption("Vector Option 4", "\"Static\" Vector Option", 10, "Start ", " End", [] {
				UIUtil::PrintSubtitle("~COLOR_BLUE~Vector option 4~s~ function executed.");
			});

			// This submenu is created below (Submenu_Examples). This just creates an option for the page
			// When adding a new submenu option, make sure to actually create the submenu
			sub->AddSubmenuOption("Submenu Example", "Submenu Option Example", Submenu_Examples);
	});

	// Initialize this submenu
	g_ExamplesSubmenu->Init();
}


void main()
{
	g_Menu = std::make_unique<CNativeMenu>();

	InitializeMenu(); // Make sure to call InitializeMenu() before calling any other CNativeMenu (g_Menu) function
	g_Menu->GoToSubmenu(Submenu_EntryMenu); // We only need to do this manually ONCE. It's automatic. See comment inside function.

	if (!UIUtil::GetScreenDimensions()) {
		PRINT_WARN("Failed to get Red Dead Redemption 2 game window dimensions. The UI may be sized incorrectly.");
	}

	while (true)
	{
		// This is required. Do not remove.
		// This makes the menu render everything.
		g_Menu->Update();

		// Update the vectors in real time ONLY while we're in the time examples submenu
		// 
		// You could probably put this in a function called CExampleSubmenu::Tick() that gets called every frame
		// so it doesn't have to be put here
		if (g_Menu->GetCurrentSubmenu()->ID == Submenu_Examples_Time) {
			g_Menu->GetCurrentSubmenu()->GetOption(0)->As<VectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_HOURS());
			g_Menu->GetCurrentSubmenu()->GetOption(1)->As<VectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_MINUTES());
			g_Menu->GetCurrentSubmenu()->GetOption(2)->As<VectorOption*>()->SetVectorIndex(CLOCK::GET_CLOCK_SECONDS());
		}

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
#pragma warning(default:28159)

void ScriptMain()
{
	main();
}
