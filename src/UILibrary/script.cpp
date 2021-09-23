// Licensed under the MIT License.


#include "script.h"
#include "keyboard.h"
#include "menu.h"
#include "pages.h"
#include "footer.h"
#include <string>


/*
################################################################
# RDR2 UI Library
# Made by TuffyTown (Halen84)
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# ==============================================================
# GitHub: https://github.com/Halen84/RDR2-UI-Library
# License: MIT
################################################################
*/


// I am NOT very good at C++, but I tried my best. So please ignore bad and inconsistent code :(
// If you have any improvements you would like to make, feel free to make a pull request via GitHub.
// If you find any bugs with this library, please make an issue report or pull request if you know how to fix it.
// Be sure to check for updates!


 #pragma region Help & Info
/*
	- To change how pages are drawn, see pages.cpp
	- To change the footer text, see footer.cpp
	- To execute code when an option is pressed/toggled, see menuitemfunctions.cpp
	- menu.cpp is where sprites and text are drawn
	- script.h contains some enums and constants
	- Be sure to change your output directory, as its set to my custom one
*/
#pragma endregion


int selectPrompt;
int backPrompt;

double pageIndex = 0.0; // Current Page
double previousPageIndex = 0.0;

int selectedIndex = 0; // Selected Item
int previousIndex = 0;

bool enabled = false;
bool justOpened = false;
bool justClosed = false;


void play_frontend_sound(const char* name, const char* soundset)
{
	AUDIO::_STOP_SOUND_WITH_NAME(name, soundset);
	AUDIO::PLAY_SOUND_FRONTEND(name, soundset, true, 0);
}


void update()
{
	// Background & Header Texture
	DrawSprite("generic_textures", "inkroller_1a", BG_X_OFFSET, BG_Y_OFFSET, BG_WIDTH, BG_HEIGHT, 0.0, 0, 0, 0, 230, false);
	DrawSprite("generic_textures", "menu_header_1a", TOP_HEADER_X_POS, TOP_HEADER_Y_POS, TOP_HEADER_WIDTH, TOP_HEADER_HEIGHT, 0, 255, 255, 255, 255, false);


	// Drawing Pages
	DrawPage(pageIndex);
	int numOptions = GetNumOptionsInCurrentPage();
	if (numOptions >= 8) {
		DrawCSSText(std::to_string(selectedIndex + 1) + " of " + std::to_string(numOptions), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (8 * 55.0f), 0, -1);
	} else {
		DrawCSSText(std::to_string(selectedIndex + 1) + " of " + std::to_string(numOptions), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (numOptions * 55.0f), 0, -1);
	}


	// Footer
	// Doesn't draw right with new wrapper for some reason
	GRAPHICS::DRAW_SPRITE("generic_textures", "menu_bar", 0.16, 0.9, 0.23, 0.001, 0, 255, 255, 255, 175, false);
	UpdateFooter();


	// Page Handling
	if (HUD::_UIPROMPT_HAS_STANDARD_MODE_COMPLETED(selectPrompt, 0)) {
		play_frontend_sound("SELECT", "HUD_SHOP_SOUNDSET");
		if (pageIndex == 0.0) {
			pageIndex = selectedIndex + 1.0;
			previousIndex = selectedIndex;
			selectedIndex = 0;
		} else {
			if (DoesOptionHavePage(pageIndex, selectedIndex)) {
				// Pages inside of pages are indexed by decimals so thats why its a double
				previousPageIndex = pageIndex;
				pageIndex += (selectedIndex / 10.0) + 0.1;
				// Note: There is still a bug with pages insides of pages currently. Should still "mostly" work however.
			} else if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
				OnToggle(false, true);
			} else {
				OnSelect();
			}
		}
	}

	if (HUD::_UIPROMPT_HAS_STANDARD_MODE_COMPLETED(backPrompt, 0)) {
		play_frontend_sound("BACK", "HUD_SHOP_SOUNDSET");
		selectedIndex = previousIndex;
		if (pageIndex == 0.0) {
			enabled = false;
			justClosed = true;
			pageIndex = 0.0;
		} else {
			if ((int)(pageIndex * 10) % 10 == 0) {
				pageIndex = 0.0;
				return;
			}
			pageIndex = previousPageIndex;
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
	
	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_LEFT"))) {
		if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
			OnToggle(true, false);
		}
	}

	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_RIGHT"))) {
		if (DoesOptionHaveToggle(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");
			OnToggle(false, true);
		}
	}

	DrawSelectionBox();
}


int GetCurrentSelectedIndex() { return selectedIndex; }
double GetCurrentPageIndex() { return pageIndex; }


void main()
{
	const int TOGGLE_KEY = VK_NUMPAD9;
	bool firstTimeActivation = true;

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
				//ShowSubtitle("First time opening menu");
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
