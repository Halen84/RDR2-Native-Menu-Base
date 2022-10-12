// Licensed under the MIT License.

#include "script.h"
#include "common.hpp"
#include "UI/menu.hpp"
#include "UI/util.h"
#include "Submenus/examples.hpp"

/*
#########################################################################
# RDR2 Native UI Menu Base
# Made by TuffyTown (Halen84)
# Uses in-game sprites to make UI menus similar to how they are in-game
# =======================================================================
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# GitHub Link: https://github.com/Halen84/RDR2-Native-Menu-Base
# License: MIT
#########################################################################
*/


/*
HELP & INFO:

- The UI will look and function the best on 1920x1080 displays
- The "SubMenus" folder is where you can put your functions for options. Its up to you how you want to organize everything.
- ^^^ You can also create the options in other files as long as you call it in InitializeMenu()
- To change keybinds, see UI/menu.cpp in CheckInput()
- UI/option.hpp is the Option class
- UI/menu.hpp is the CNativeMenu class (g_NativeMenu)
- UI/submenu.hpp is the Submenu class
- UI/menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/drawing.cpp does the texture and text drawing for everything
*/


using namespace Menu;


bool _exampleBool = false;
std::vector<std::string> _exampleOptionVector = { "First", "Second", "Third", "Last" };
void InitializeMenu()
{
	g_NativeMenu->AddSubmenu("HEADER", "Sub Header", Submenu_EntryMenu, DEFAULT, [](Submenu* sub) 
	{
			sub->AddRegularOption("Regular Option", "Regular Option Example", [] {
				Util::PrintSubtitle("~COLOR_BLUE~Regular~s~ option function executed");
			});

			sub->AddBoolOption("Bool Option", "Bool Option Example", &_exampleBool, [] {
				Util::PrintSubtitle("~COLOR_BLUE~Bool~s~ option function executed. Bool is: " + std::string(_exampleBool ? "true" : "false"));
			});

			sub->AddVectorOption<std::string>("Vector Option 1", "Vector Option", _exampleOptionVector, [] {
				Util::PrintSubtitle("~COLOR_BLUE~Vector option 1~s~ function executed. Index: " + std::to_string(g_NativeMenu->GetSelectedOption()->m_VectorIndex));
			});

			// Note: When using const char* vectors, DONT be explicit about the type here.
			// There is a seperate function for it.
			sub->AddVectorOption("Vector Option 2", "Vector Option with an unnamed scope", { "Unnamed", "Scope", "Vector" }, [] {
				Util::PrintSubtitle("~COLOR_BLUE~Vector option 2~s~ function executed.");
			});

			sub->AddVectorOption<int>("Vector Option 3", "Vector Option with an unnamed scope as an integer", { 1, 2, 3, 4, 5 }, [] {
				Util::PrintSubtitle("~COLOR_BLUE~Vector option 3~s~ function executed.");
			});

			// 0 based indexing, 0 --> 9 is 10 options
			sub->AddVectorOption("Vector Option 4", "\"Static\" Vector Option", 10, "", "", [] {
				Util::PrintSubtitle("~COLOR_BLUE~Vector option 4~s~ function executed.");
			});

			// This submenu is created below (Submenu_Examples). This just creates an option for the page
			// When adding a new submenu option, make sure to actually create the submenu
			sub->AddSubmenuOption("Submenu Example", "Submenu Option Example", Submenu_Examples);
	});


	// These calls below would preferably go in examples.cpp, but I put them here for simplicity reasons.


	g_NativeMenu->AddSubmenu("EXAMPLES", "Functional Examples", Submenu_Examples, DEFAULT, [](Submenu* sub)
	{
			sub->AddSubmenuOption("Change Weather", "", Submenu_Examples_Weather);
			sub->AddSubmenuOption("Change Time", "", Submenu_Examples_Time);
	});


	g_NativeMenu->AddSubmenu("EXAMPLES", "Change Weather", Submenu_Examples_Weather, MAX, [](Submenu* sub)
	{
			// In a real scenario, you should use enum hashes
			// for the weather types instead of GET_HASH_KEY

			sub->AddRegularOption("High Pressure", "",		[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("HIGHPRESSURE")); });
			sub->AddRegularOption("Rain", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("RAIN")); });
			sub->AddRegularOption("Snow", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SNOW")); });
			sub->AddRegularOption("Misty", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("MISTY")); });
			sub->AddRegularOption("Fog", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("FOG")); });
			sub->AddRegularOption("Sunny", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SUNNY")); });
			sub->AddRegularOption("Clouds", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("CLOUDS")); });
			sub->AddRegularOption("Overcast", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("OVERCAST")); });
			sub->AddRegularOption("Thunderstorm", "",		[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("THUNDERSTORM")); });
			sub->AddRegularOption("Hurricane", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("HURRICANE")); });
			sub->AddRegularOption("Thunder", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("THUNDER")); });
			sub->AddRegularOption("Shower", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SHOWER")); });
			sub->AddRegularOption("Blizzard", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("BLIZZARD")); });
			sub->AddRegularOption("Snow Light", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SNOWLIGHT")); });
			sub->AddRegularOption("Whiteout", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("WHITEOUT")); });
			sub->AddRegularOption("Hail", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("HAIL")); });
			sub->AddRegularOption("Sleet", "",				[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SLEET")); });
			sub->AddRegularOption("Drizzle", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("DRIZZLE")); });
			sub->AddRegularOption("Sandstorm", "",			[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("SANDSTORM")); });
			sub->AddRegularOption("Overcast Dark", "",		[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("OVERCASTDARK")); });
			sub->AddRegularOption("Ground Blizzard", "",	[] { g_ExampleFuncs->SetWeather(MISC::GET_HASH_KEY("GROUNDBLIZZARD")); });
	});


	// If you were to implement this in examples.cpp, you wouldn't need to use a lambda.
	// You also wouldn't need to use "g_ExampleFuncs->..."
	// But once again, this is just here for simplicity reasons.
	g_NativeMenu->AddSubmenu("EXAMPLES", "Change Time", Submenu_Examples_Time, DEFAULT, [](Submenu* sub)
	{
			sub->AddVectorOption("Hour", "", 24, "", "",	[] { g_ExampleFuncs->SetTime(); });
			sub->AddVectorOption("Minute", "", 60, "", "",	[] { g_ExampleFuncs->SetTime(); });
			sub->AddVectorOption("Second", "", 60, "", "",	[] { g_ExampleFuncs->SetTime(); });
			sub->AddBoolOption("Pause Time", "", &g_ExampleFuncs->bPauseTime, [] { g_ExampleFuncs->PauseTime(); });
	});
}


void main()
{
	g_NativeMenu = std::make_unique<CNativeMenu>();
	g_NativeMenu->RegisterUIPrompts();
	InitializeMenu(); // Make sure to call InitializeMenu() before calling any other CNativeMenu (g_NativeMenu) function
	g_NativeMenu->GoToSubmenu(Submenu_EntryMenu, false); // We only need to do this manually ONCE. It's automatic. See comment inside function.

	if (!Menu::Util::SetResolution()) {
#if ALLOCATE_CONSOLE
		std::cout << "[main] [WARNING]: Failed to find the RDR2 game window" << "\n";
#endif
	}

	while (true)
	{
		g_NativeMenu->Update();

		// Update the vectors in real time while we're in the time examples page
		if (g_NativeMenu->CurrentSubmenu->m_ID == Submenu_Examples_Time && g_NativeMenu->DoesSubmenuExist(Submenu_Examples_Time)) {
			g_NativeMenu->CurrentSubmenu->GetOption(0)->SetVectorIndex(CLOCK::GET_CLOCK_HOURS());
			g_NativeMenu->CurrentSubmenu->GetOption(1)->SetVectorIndex(CLOCK::GET_CLOCK_MINUTES());
			g_NativeMenu->CurrentSubmenu->GetOption(2)->SetVectorIndex(CLOCK::GET_CLOCK_SECONDS());
		}

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
