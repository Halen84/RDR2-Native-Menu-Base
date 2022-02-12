// Licensed under the MIT License.


#include "..\keyboard.h"
#include "..\pages.h"
#include "script.h"
#include "menu.h"
#include <string>
#include <unordered_map>


/*
####################################################################
# RDR2 Native UI Menu Base - Old Name: RDR2 UI Library
# Made by TuffyTown (Halen84)
# Uses in-game sprites to make similar menus to how they are in-game
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# ==================================================================
# GitHub: https://github.com/Halen84/RDR2-UI-Library
# License: MIT
####################################################################
*/


// If you have any improvements you would like to make, feel free to make a pull request via GitHub.
// If you find any bugs with this library, please make an issue report or pull request if you know how to fix it.
// Be sure to check for updates for this library and Natives DB!


 #pragma region Help & Info
/*
	- To create and draw pages, see \Core\pages.cpp
	- To execute code when an option is pressed/toggled, see functions.cpp
	- menu.cpp is basically the core UI functionality. Edit at your own risk.
	- script.cpp (this file) is also core. Handles key presses, indexing, drawing, and some other things
*/
#pragma endregion


Prompt selectPrompt;
Prompt backPrompt;

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
	// Background, Header, and Footer Textures
	Draw::DrawSprite("generic_textures", "inkroller_1a", BG_X_OFFSET, BG_Y_OFFSET, BG_WIDTH, BG_HEIGHT, 0.0, 0, 0, 0, 230, false);
	Draw::DrawSprite("generic_textures", "menu_header_1a", TOP_HEADER_X_POS, TOP_HEADER_Y_POS, TOP_HEADER_WIDTH, TOP_HEADER_HEIGHT, 0, 255, 255, 255, 255, false);
	GRAPHICS::DRAW_SPRITE("generic_textures", "menu_bar", 0.16, 0.9, 0.23, 0.001, 0, 255, 255, 255, 175, false); // Doesn't draw right with Draw::DrawSprite for some reason
	// TODO - Scroller Sprites
	/*
	- menu_textures
	scroller_arrow_bottom
	scroller_arrow_top
	scroller_left_bottom
	scroller_left_top
	scroller_line_down
	scroller_line_up
	scroller_right_bottom
	scroller_right_top
	*/


	// Drawing pages
	if (pageIndex == 0.0) {
		// The very first page
		__DrawEntryPage__();
	}
	else {
		// Every other page
		std::unordered_map<double, void(*)()> structure = getPageStructures();
		if (structure.contains(pageIndex)) {
			structure[pageIndex](); // Draw the page
		}
	}
	

	// Option Counter Text
	int numOptions = Menu::GetNumOptionsInCurrentPage();
	int maxVisOptions = Menu::GetMaxVisibleOptions();
	if (numOptions >= maxVisOptions) {
		Draw::DrawCSSText(std::to_string(selectedIndex + 1) + " of " + std::to_string(numOptions), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (maxVisOptions * INCREMENT), 0, 0);
	} else {
		Draw::DrawCSSText(std::to_string(selectedIndex + 1) + " of " + std::to_string(numOptions), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (numOptions * INCREMENT), 0, 0);
	}


	// Navigation
	if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(selectPrompt, 0)) {
		play_frontend_sound("SELECT", "HUD_SHOP_SOUNDSET");
		bool bIsPage = Menu::IsOptionAPage(pageIndex, selectedIndex);
		bool bIsVector = Menu::IsOptionAVector(pageIndex, selectedIndex);

		if (bIsPage && !bIsVector && pageIndex != 0.0) {
			// Note: There is still a bug with pages insides of pages.
			// It should still "mostly" work, but just be aware you can't put too many pages inside of pages if that makes sense
			
			// Pages inside of pages are indexed by decimals so thats why its a double
			resetNextAssignedIndex();
			previousPageIndex = pageIndex;
			previousIndex = selectedIndex;
			pageIndex += (selectedIndex / 10.0) + 0.1;
			selectedIndex = 0;
		}
		else if (bIsPage && !bIsVector && pageIndex == 0.0) {
			resetNextAssignedIndex();
			pageIndex = selectedIndex + 1.0;
			previousIndex = selectedIndex;
			selectedIndex = 0;
		}
		
		if (!bIsPage && !bIsVector) {
			Menu::OnSelect();
		}
		else if (!bIsPage && bIsVector) {
			// Enter has been pressed, but this option is a vector. Go right
			Menu::OnVectorSwitch(false, true);
		}
	}

	if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(backPrompt, 0)) {
		play_frontend_sound("BACK", "HUD_SHOP_SOUNDSET");
		selectedIndex = previousIndex; // needs to be fixed...
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

	if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_UP"))) {
		//selectedIndex -= 1;
		selectedIndex -= (Menu::IsIndexASeparator(pageIndex, selectedIndex - 1) ? 2 : 1);
		if (selectedIndex < 0) {
			selectedIndex = (numOptions - 1);
		}
		play_frontend_sound("NAV_UP", "Ledger_Sounds");
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_DOWN"))) {
		//selectedIndex += 1;
		selectedIndex += (Menu::IsIndexASeparator(pageIndex, selectedIndex + 1) ? 2 : 1);
		if (selectedIndex > (numOptions - 1)) {
			selectedIndex = 0;
		}
		play_frontend_sound("NAV_DOWN", "Ledger_Sounds");
	}
	
	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_LEFT"))) {
		if (Menu::IsOptionAVector(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
			Menu::OnVectorSwitch(true, false);
		}
	}

	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_RIGHT"))) {
		if (Menu::IsOptionAVector(pageIndex, selectedIndex)) {
			play_frontend_sound("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");
			Menu::OnVectorSwitch(false, true);
		}
	}

	Draw::DrawSelectionBox();
}


int* GetCurrentSelectedIndex() { return &selectedIndex; }
double* GetCurrentPageIndex() { return &pageIndex; }


void main()
{
	const int TOGGLE_KEY = VK_NUMPAD9;
	const char* CONTROLLER_INPUT_1 = "INPUT_COVER"; // RB (R1)
	const char* CONTROLLER_INPUT_2 = "INPUT_CONTEXT_B"; // B (O)
	bool firstTimeActivation = true;

	RegisterUIPrompt(selectPrompt, MISC::GET_HASH_KEY("INPUT_GAME_MENU_ACCEPT"), MISC::VAR_STRING(10, "LITERAL_STRING", "Select"));
	RegisterUIPrompt(backPrompt, MISC::GET_HASH_KEY("INPUT_GAME_MENU_CANCEL"), MISC::VAR_STRING(10, "LITERAL_STRING", "Back"));

	while (true)
	{
		// Press toggle key or RB + B for controller to open menu
		if (IsKeyJustUp(TOGGLE_KEY) || (PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY(CONTROLLER_INPUT_1)) && PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY(CONTROLLER_INPUT_2)) && !PAD::_IS_USING_KEYBOARD(0))) {
			enabled = !enabled;
			justOpened = enabled;
			justClosed = !justOpened;
			if (firstTimeActivation) {
				firstTimeActivation = false;
				//PrintSubtitle("First time opening menu");
			}
		}

		if (enabled) {
			update();
			if (!PAD::_IS_USING_KEYBOARD(0)) {
				// Disable common DPad inputs
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_WHISTLE"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_WHISTLE_HORSEBACK"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_SELECT_RADAR_MODE"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_REVEAL_HUD"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_PLAYER_MENU"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_OPEN_JOURNAL"), true);
				PAD::DISABLE_CONTROL_ACTION(0, MISC::GET_HASH_KEY("INPUT_OPEN_SATCHEL_MENU"), true);
			}
			if (justOpened) {
				justOpened = false;
				play_frontend_sound("MENU_ENTER", "HUD_PLAYER_MENU");
				MAP::DISPLAY_RADAR(false);
			}
		} else {
			if (justClosed) {
				justClosed = false;
				play_frontend_sound("MENU_CLOSE", "HUD_PLAYER_MENU");
				MAP::DISPLAY_RADAR(true);
			}
		}

		HUD::_UI_PROMPT_SET_VISIBLE(selectPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(selectPrompt, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(backPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(backPrompt, enabled);

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
