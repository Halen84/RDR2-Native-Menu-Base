// Licensed under the MIT License.


#include "script.h"
#include "keyboard.h"
#include "menu.h"
#include "pages.h"
#include "footer.h"
#include <string>


/*
######################################################
# RDR2 UI Library
# Made by TuffyTown
# Credits: Alexander Blade - ScriptHook SDK
# ====================================================
# GitHub:
# License: MIT
######################################################
*/


// If you have any improvements you would like to make, feel free to make a pull request via GitHub.
// If you find any bugs with this library, please make an issue report or pull request if you know how to fix it.


 #pragma region INFO
/*
	- If you want to have more than 8 page options (scrolling), you'll have to implement it yourself because I couldn't figure it out.
	  If you figure this out, please make a pull request via GitHub and I'll gladly implement it. See menu.cpp

	- Sorry for not having this feature and or any other missed features.

	- My current theory is to get perfect ui, scrolling, etc, I would have to use DATABINDING namespace. Lol idk.
	  Ive looked through the decompiled scripts and it looks like it would work based on the context, but I have no idea how to use the functions.
	  There is no good documentation on them!

	- I probably should've used OOP

	- I am NOT very good at C++, but I tried my best. So please ignore bad and inconsistent code :(
*/
#pragma endregion

 #pragma region Help
/*
	- To change how pages are drawn, see pages.cpp
	- To change the footer text, see footer.cpp
	- To execute code when an option is pressed/toggled, see menuitemfunctions.cpp
	- menu.cpp is where sprites are drawn
	- If you would like to implement scrolling, see menu.cpp (DrawListOption)
*/
#pragma endregion


int selectPrompt;
int backPrompt;

double pageIndex = 0.0; // Current Page
int selectedIndex = 0; // Selected Item

bool enabled = false;
bool justOpened = false;
bool justClosed = false;


void draw_text_element(std::string text, float posX, float posY, float scaleX, float scaleY)
{
	UIDEBUG::_BG_SET_TEXT_SCALE(scaleX, scaleY);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", _strdup(text.c_str())), posX, posY);
}


void play_frontend_sound(const char* name, const char* soundset)
{
	AUDIO::_STOP_SOUND_WITH_NAME(name, soundset);
	AUDIO::PLAY_SOUND_FRONTEND(name, soundset, true, 0);
}


void update()
{
	// Background & Header Texture
	GRAPHICS::DRAW_SPRITE("generic_textures", "inkroller_1a", 0.16, 0.5, 0.3, 0.95, 0.0, 0, 0, 0, 230, false);
	GRAPHICS::DRAW_SPRITE("generic_textures", "menu_header_1a", 0.16, 0.1, 0.23, 0.1, 0, 255, 255, 255, 255, false);
	

	// Drawing Pages
	DrawPage(pageIndex);
	int numOptions = GetNumOptionsInCurrentPage();
	// todo: find color closer to ingame color
	DrawCSSText(std::to_string(selectedIndex + 1) + " of " + std::to_string(numOptions), "body", "c0c0c0", "CENTER", 0, 20, -0.515, 0.187f + (numOptions * 0.053f));


	// Footer
	GRAPHICS::DRAW_SPRITE("generic_textures", "menu_bar", 0.16, 0.9, 0.22, 0.001, 0, 255, 255, 255, 175, false);
	UpdateFooter();


	// Page Handling
	if (HUD::_UIPROMPT_HAS_STANDARD_MODE_COMPLETED(selectPrompt, 0)) {
		play_frontend_sound("SELECT", "HUD_SHOP_SOUNDSET");
		if (pageIndex == 0.0) {
			pageIndex = selectedIndex + 1.0;
			selectedIndex = 0;
		} else {
			if (DoesOptionHavePage(pageIndex, selectedIndex)) {
				// Pages inside of pages are indexed by decimals so thats why its a double
				pageIndex += 0.1;
			} else if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
				OnToggle();
			} else {
				OnSelect();
			}
		}
	}

	if (HUD::_UIPROMPT_HAS_STANDARD_MODE_COMPLETED(backPrompt, 0)) {
		play_frontend_sound("BACK", "HUD_SHOP_SOUNDSET");
		selectedIndex = 0;
		if (pageIndex == 0.0) {
			enabled = false;
			justClosed = true;
			pageIndex = 0.0;
		} else {
			pageIndex -= 0.1;
			// Since we subtract 0.1, we will go back too much if we are on page x.0 and end up on page x.9 if that makes sense
			// So just check the if the tenths place is 9
			if ((int)(pageIndex * 10) % 10 == 9) {
				pageIndex = 0.0;
			}
		}
	}


	// Navigation & Input
	if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_UP"))) {
		selectedIndex -= 1;
		if (selectedIndex < 0) {
			selectedIndex = (numOptions - 1);
		}
		play_frontend_sound("NAV_UP", "Ledger_Sounds");
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_DOWN"))) {
		selectedIndex += 1;
		if (selectedIndex > (numOptions - 1)) {
			selectedIndex = 0;
		}
		play_frontend_sound("NAV_DOWN", "Ledger_Sounds");
	}
	
	// Not sure why left & right only work this way
	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_LEFT"))) {
		if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
			OnToggle();
		}
	}

	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_RIGHT"))) {
		if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");
			OnToggle();
		}
	}

	DrawSelectionBox();
}


// Underutilized lol
int GetCurrentSelectedIndex() { return selectedIndex; }
double GetCurrentPageIndex() { return pageIndex; }


void main()
{
	const int TOGGLE_KEY = VK_NUMPAD9;
	bool firstTimeActivation = true;

	// Didn't care to look for text hash, so I made them myself
	CreateControlAction(selectPrompt, MISC::GET_HASH_KEY("INPUT_GAME_MENU_ACCEPT"), MISC::VAR_STRING(10, "LITERAL_STRING", "Select"));
	CreateControlAction(backPrompt, MISC::GET_HASH_KEY("INPUT_GAME_MENU_CANCEL"), MISC::VAR_STRING(10, "LITERAL_STRING", "Back"));

	while (true)
	{
		if (IsKeyJustUp(TOGGLE_KEY)) {
			enabled = !enabled;
			justOpened = enabled;
			justClosed = !justOpened;

			MAP::DISPLAY_RADAR(!enabled);
			if (firstTimeActivation) {
				firstTimeActivation = false;
				//ShowSubtitle("Made by TuffyTown");
			}
		}

		if (enabled) {
			update();
			if (justOpened) {
				justOpened = false;
				play_frontend_sound("MENU_ENTER", "HUD_PLAYER_MENU");
			}
		} else {
			if (justClosed) {
				justClosed = false;
				play_frontend_sound("MENU_CLOSE", "HUD_PLAYER_MENU");
			}
		}

		HUD::_UIPROMPT_SET_VISIBLE(selectPrompt, enabled);
		HUD::_UIPROMPT_SET_ENABLED(selectPrompt, enabled);
		HUD::_UIPROMPT_SET_VISIBLE(backPrompt, enabled);
		HUD::_UIPROMPT_SET_ENABLED(backPrompt, enabled);

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
