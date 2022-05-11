// Licensed under the MIT License.

#include "..\keyboard.h"
#include "..\pages.h"
#include "script.h"
#include "menu.h"

/*
####################################################################
# RDR2 Native UI Menu Base - Old Name: RDR2 UI Library
# Made by TuffyTown (Halen84)
# Uses in-game sprites to make similar menus to how they are in-game
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# ==================================================================
# GitHub: https://github.com/Halen84/RDR2-Native-Menu-Base
# License: MIT
####################################################################
*/


// If you have any improvements you would like to make, feel free to make a pull request via GitHub.
// If you find any bugs with this library, please make an issue report or pull request if you know how to fix it.
// Sorry for the semi-shit codebase. I'm not xXCPPMaster6969Xx over here.
// Be sure to check for updates for this library and Natives DB!


 #pragma region Help & Info
/*
	- To create new pages and options, see pages.cpp
	- To execute code when an option is pressed/toggled, see functions.cpp (any pages.cpp)
	- menu.cpp is basically the core UI functionality. Edit at your own risk.
	- script.cpp (this file) is also core. Handles key presses, indexing, drawing, and some other things.
	- Might be a good idea to "using namespace Menu;" in your files to speed up your coding slightly. Kinda ruins the point of namespaces but I don't care.
	- The "lib" folder is useful if you want to import external libraries to use. You can just throw them in there.
	- Ignore the "xyz is ambiguous" errors! It's just an intellisense error.
*/
#pragma endregion


using namespace Menu;

Prompt selectPrompt;
Prompt backPrompt;

double previousPageIndex = 0.0;


// Keeps track of what selection you were on for a page
std::unordered_map<double, int> map_selectedIndexes;
//std::vector<void(*)()> temp_vec_pages;


void playSoundFrontend(const char* name, const char* soundset)
{
	AUDIO::_STOP_SOUND_WITH_NAME(name, soundset);
	AUDIO::PLAY_SOUND_FRONTEND(name, soundset, true, 0);
}

bool isControllerComboPressed(Hash input1, Hash input2)
{
	if (!PAD::_IS_USING_KEYBOARD(0)) {
		return PAD::IS_CONTROL_PRESSED(0, input1) && (m_isClosed == true ? PAD::IS_CONTROL_JUST_PRESSED(0, input2) : PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, input2));
	}

	return false;
}


void updateMenu()
{
	// Draw the menu and its textures
	Draw::DrawMenuTextures();


	// Drawing pages
	if (m_pageIndex == 0.0) {
		// The very first page
		__EntryPage__();
	}
	else {
		// Every other page
		std::unordered_map<double, void(*)()> structure = getPageStructures();
		if (structure.contains(m_pageIndex)) {
			structure[m_pageIndex](); // Draw the page
		}
	}
	

	// Option Counter Text
	if (m_numOptionsInThisPage >= m_visibleOptionsInThisPage) {
		Draw::DrawCSSText(std::to_string(m_selectionIndex + 1) + " of " + std::to_string(m_numOptionsInThisPage), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (m_visibleOptionsInThisPage * INCREMENT), 0, 0);
	} else {
		Draw::DrawCSSText(std::to_string(m_selectionIndex + 1) + " of " + std::to_string(m_numOptionsInThisPage), Font::Hapna, 0x90, 0x90, 0x90, 230, Alignment::Right, 20, 529.0f, 242.0f + (m_numOptionsInThisPage * INCREMENT), 0, 0);
	}


	// Navigation
	if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(selectPrompt, 0)) {
		playSoundFrontend("SELECT", "HUD_SHOP_SOUNDSET"); // TODO: Should I make this play vector switch sound if option is vector?
		bool bIsPage = Util::IsOptionAPage(m_pageIndex, m_selectionIndex);
		bool bIsVector = Util::IsOptionAVector(m_pageIndex, m_selectionIndex);

		if (bIsPage && !bIsVector && m_pageIndex != 0.0) {
			// Note: There is still a bug with pages insides of pages.
			// It should still "mostly" work, but just be aware you can't put too many pages inside of pages if that makes sense

			resetNextAssignedIndex();
			previousPageIndex = m_pageIndex;
			// TODO: This whole Page Indexing system is like the most fucking broken thing ever and needs to be reworked
			m_pageIndex += (m_selectionIndex / 10.0) + 0.1;
			m_selectionIndex = 0;
		}
		else if (bIsPage && !bIsVector && m_pageIndex == 0.0) {
			resetNextAssignedIndex();
			m_pageIndex = m_selectionIndex + 1.0;
			m_selectionIndex = 0;
		}
		
		if (!bIsPage && !bIsVector) {
			Util::ExecuteOptionFunc();
		}
		else if (!bIsPage && bIsVector) {
			// Enter has been pressed, but this option is a vector. Go right
			Util::ExecuteVectorFunc(false, true);
		}
	}

	if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(backPrompt, 0)) {
		playSoundFrontend("BACK", "HUD_SHOP_SOUNDSET");
		if (m_pageIndex == 0.0) {
			Menu::SetMenuEnabled(false);
			m_pageIndex = 0.0;
		} else {
			if ((int)(m_pageIndex * 10) % 10 == 0) {
				m_pageIndex = 0.0;
				m_selectionIndex = map_selectedIndexes[m_pageIndex];
				return;
			}
			m_pageIndex = previousPageIndex;
			m_selectionIndex = map_selectedIndexes[m_pageIndex];
		}
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_GAME_MENU_UP)) {
		if (IsKeyDownLong(VK_SHIFT)) {
			m_selectionIndex -= 10;
			if (m_selectionIndex < 0) {
				m_selectionIndex = 0;
			}
		} else {
			//m_selectionIndex -= (Util::IsIndexASeparator(m_pageIndex, m_selectionIndex - 1) ? 2 : 1);
			m_selectionIndex -= 1;
		}

		if (m_selectionIndex < 0) {
			m_selectionIndex = (m_numOptionsInThisPage - 1);
		}
		playSoundFrontend("NAV_UP", "Ledger_Sounds");
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_GAME_MENU_DOWN)) {
		if (IsKeyDownLong(VK_SHIFT)) {
			m_selectionIndex += 10;
			if (m_selectionIndex > m_numOptionsInThisPage) {
				m_selectionIndex = m_numOptionsInThisPage - 1;
			}
		} else {
			//m_selectionIndex += (Util::IsIndexASeparator(m_pageIndex, m_selectionIndex + 1) ? 2 : 1);
			m_selectionIndex += 1;
		}

		if (m_selectionIndex > (m_numOptionsInThisPage - 1)) {
			m_selectionIndex = 0;
		}
		playSoundFrontend("NAV_DOWN", "Ledger_Sounds");
	}
	
	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_GAME_MENU_LEFT)) {
		if (Util::IsOptionAVector(m_pageIndex, m_selectionIndex)) {
			playSoundFrontend("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
			Util::ExecuteVectorFunc(true, false);
		}
	}

	if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_GAME_MENU_RIGHT)) {
		if (Util::IsOptionAVector(m_pageIndex, m_selectionIndex)) {
			playSoundFrontend("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");
			Util::ExecuteVectorFunc(false, true);
		}
	}

	Draw::DrawSelectionBox();
	map_selectedIndexes[m_pageIndex] = m_selectionIndex;
}


void main()
{
	const int TOGGLE_KEY = VK_NUMPAD9;
	const Hash CONTROLLER_INPUT_1 = INPUT_COVER; // RB (R1)
	const Hash CONTROLLER_INPUT_2 = INPUT_CONTEXT_A; // A (X)
	//bool bFirstTimeActivation = true;

	RegisterUIPrompt(selectPrompt, INPUT_GAME_MENU_ACCEPT, MISC::VAR_STRING(10, "LITERAL_STRING", "Select"));
	RegisterUIPrompt(backPrompt, INPUT_GAME_MENU_CANCEL, MISC::VAR_STRING(10, "LITERAL_STRING", "Back"));

	while (true)
	{
		// Press TOGGLE_KEY or (INPUT_1 + INPUT_2) for controller to open menu
		if (IsKeyJustUp(TOGGLE_KEY) || isControllerComboPressed(CONTROLLER_INPUT_1, CONTROLLER_INPUT_2)) {
			Menu::SetMenuEnabled(!m_isOpen);
			/*if (bFirstTimeActivation) {
				bFirstTimeActivation = false;
				PrintSubtitle("First time opening the menu this game boot");
			}*/
		}

		if (m_isOpen) {
			updateMenu();
			//HUD::HIDE_HUD_AND_RADAR_THIS_FRAME(); // Debating if I should add this instead of calling DISPLAY_RADAR?

			// Disable some common inputs while the menu is open so there's no weirdness
			if (!PAD::_IS_USING_KEYBOARD(0)) {
				*getGlobalPtr(1900383 + 316) = 2; // Disables horse whistling this frame. Not sure if this is safe, but it works.
				PAD::DISABLE_CONTROL_ACTION(0, INPUT_WHISTLE, false);
				PAD::DISABLE_CONTROL_ACTION(0, INPUT_WHISTLE_HORSEBACK, false);
			}

			PAD::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_RADAR_MODE, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_REVEAL_HUD, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_PLAYER_MENU, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_OPEN_JOURNAL, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_OPEN_SATCHEL_MENU, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_OPEN_CRAFTING_MENU, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_OPEN_EMOTE_WHEEL, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_EXPAND_RADAR, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_INTERACTION_MENU, false);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_HUD_SPECIAL, false);
		}

		HUD::_UI_PROMPT_SET_VISIBLE(selectPrompt, m_isOpen);
		HUD::_UI_PROMPT_SET_ENABLED(selectPrompt, m_isOpen);
		HUD::_UI_PROMPT_SET_VISIBLE(backPrompt, m_isOpen);
		HUD::_UI_PROMPT_SET_ENABLED(backPrompt, m_isOpen);

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
