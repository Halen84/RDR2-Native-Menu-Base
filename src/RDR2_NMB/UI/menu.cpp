// Licensed under the MIT License.

#include "menu.hpp"
#include "drawing.h"
#include "../common.hpp"

using namespace Menu;

void playSoundFrontend(const char* name, const char* soundset)
{
	AUDIO::_STOP_SOUND_WITH_NAME(name, soundset);
	AUDIO::PLAY_SOUND_FRONTEND(name, soundset, true, 0);
}


void CNativeMenu::LoopFunc()
{
	CheckInput();
	HandleInput();
	DisableCommonInputs(); // Uses m_IsOpen
	
	if (m_IsOpen) {
		Drawing::DrawMenuTextures();
		Drawing::DrawSelectionBox();
		Drawing::DrawHeader(m_CurrentSubMenu->m_Header);
		Drawing::DrawSubHeader(m_CurrentSubMenu->m_SubHeader);
		
		for (int i = 0; i < m_CurrentSubMenu->m_NumOptions; i++) {
			Option* option = m_CurrentSubMenu->GetOption(i);
			if (option != nullptr) {
				Drawing::DrawOption(option);
			} else {
#if ALLOCATE_CONSOLE
				std::cout << "[CNativeMenu::LoopFunc] [ERROR]: Option is a nullptr (" << i << ")" << "\n";
#endif
			}
		}

		if (m_CurrentSubMenu->m_NumOptions >= m_CurrentSubMenu->m_NumVisibleOptions) {
			Drawing::DrawFormattedText(std::to_string(m_SelectionIndex + 1) + " of " + std::to_string(m_CurrentSubMenu->m_NumOptions), Font::Body, 144, 144, 144, 230, Alignment::Right, 20, 531.0f, 243.0f + (m_CurrentSubMenu->m_NumVisibleOptions * INCREMENT), 0, -1);
		} else {
			Drawing::DrawFormattedText(std::to_string(m_SelectionIndex + 1) + " of " + std::to_string(m_CurrentSubMenu->m_NumOptions), Font::Body, 144, 144, 144, 230, Alignment::Right, 20, 531.0f, 243.0f + (m_CurrentSubMenu->m_NumOptions * INCREMENT), 0, -1);
		}
	}

	HUD::_UI_PROMPT_SET_VISIBLE(m_SelectPrompt, m_IsOpen);
	HUD::_UI_PROMPT_SET_ENABLED(m_SelectPrompt, m_IsOpen);
	HUD::_UI_PROMPT_SET_VISIBLE(m_BackPrompt, m_IsOpen);
	HUD::_UI_PROMPT_SET_ENABLED(m_BackPrompt, m_IsOpen);
}


void CNativeMenu::SetEnabled(bool bEnabled, bool bPlaySounds)
{
	m_IsOpen = bEnabled;

	if (bPlaySounds) {
		if (m_IsOpen) {
			MAP::DISPLAY_RADAR(false);
			playSoundFrontend("MENU_ENTER", "HUD_PLAYER_MENU");
		} else {
			MAP::DISPLAY_RADAR(true);
			playSoundFrontend("MENU_CLOSE", "HUD_PLAYER_MENU");
		}
	}
}


void CNativeMenu::CheckInput()
{
	m_PadIndex = PAD::_IS_USING_KEYBOARD(0) ? 0 : 2;

	m_OpenKeyPressed	= IsKeyJustUp(VK_F5) || PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) && PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A);
	m_EnterKeyPressed	= PAD::IS_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_ACCEPT);
	m_BackKeyPressed	= PAD::IS_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_CANCEL);
	m_UpKeyPressed		= PAD::IS_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_UP);
	m_DownKeyPressed	= PAD::IS_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_DOWN);
	m_LeftKeyPressed	= PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_LEFT);
	m_RightKeyPressed	= PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_PadIndex, INPUT_GAME_MENU_RIGHT);

	if (HUD::_UI_PROMPT_IS_VALID(m_SelectPrompt))
		m_SelectPromptCompleted = HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(m_SelectPrompt, 0);
	else
		m_SelectPromptCompleted = m_EnterKeyPressed; // Prompt doesn't exist for some reason. Fallback to use raw input.

	if (HUD::_UI_PROMPT_IS_VALID(m_BackPrompt))
		m_BackPromptCompleted = HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(m_BackPrompt, 0);
	else
		m_BackPromptCompleted = m_BackKeyPressed; // Prompt doesn't exist for some reason. Fallback to use raw input.
}


void CNativeMenu::HandleInput()
{
	if (m_OpenKeyPressed) {
		m_IsOpen = !m_IsOpen;
		SetEnabled(m_IsOpen, true);
	}


	if (m_IsOpen) {
		if (m_SelectPromptCompleted) {
			Option* option = GetSelectedOption();
			if (option == nullptr) return;

			if (option->m_IsRegularOption) {
				option->ExecuteFunc();
			}
			else if (option->m_IsBoolOption) {
				if (option->GetBoolPtr() != nullptr) {
					*option->GetBoolPtr() = !*option->GetBoolPtr(); // Switch value
				} else {
#if ALLOCATE_CONSOLE
					std::cout << "[CNativeMenu::HandleInput] [WARNING]: GetBoolPtr at index " << m_SelectionIndex << " is a nullptr" << "\n";
#endif
				}

				option->ExecuteFunc();
			}
			else if (option->m_IsVectorOption) {
				option->ExecuteVectorFunc(false, true);
			}
			else if (option->m_IsSubMenuOption) {
				//g_NativeMenu->GoToSubmenu(option->m_SubMenuID, false);
				m_PrevSubMenuIds.push_back(m_CurrentSubMenu->m_ID);
				m_SubMenuLastSelections[m_CurrentSubMenu->m_ID] = m_SelectionIndex;
				m_CurrentSubMenu = &m_SubMenus[option->m_SubMenuID];
				m_SelectionIndex = 0;
			}
			playSoundFrontend("SELECT", "HUD_SHOP_SOUNDSET");
		}


		if (m_BackPromptCompleted) {
			if (m_CurrentSubMenu->m_ID <= Submenu_EntryMenu) {
				playSoundFrontend("BACK", "HUD_SHOP_SOUNDSET");
				SetEnabled(false, true);
				return;
			}

			if (m_PrevSubMenuIds.size() > 0) {
				//g_NativeMenu->GoToSubmenu(m_PrevSubMenuIds[m_PrevSubMenuIds.size() - 1], true);
				m_CurrentSubMenu = &m_SubMenus[m_PrevSubMenuIds[m_PrevSubMenuIds.size() - 1]];
				m_PrevSubMenuIds.pop_back();
				m_SelectionIndex = m_SubMenuLastSelections[m_CurrentSubMenu->m_ID];
				m_SubMenuLastSelections.erase(m_CurrentSubMenu->m_ID);
			} else {
				m_CurrentSubMenu = &m_SubMenus[Submenu_EntryMenu];
				m_SelectionIndex = m_SubMenuLastSelections[Submenu_EntryMenu];
			}

			playSoundFrontend("BACK", "HUD_SHOP_SOUNDSET");
		}


		if (m_UpKeyPressed) {
			if (GetSelectedOption() == nullptr) return;

			if (IsKeyDownLong(VK_SHIFT)) {
				m_SelectionIndex -= 10;
				if (m_SelectionIndex < 0)
					m_SelectionIndex = 0; // DONT reset to the BOTTOM of the page
			} else {
				m_SelectionIndex--;
			}

			if (GetSelectedOption()->m_IsPageBreak)
				m_SelectionIndex--;

			if (m_SelectionIndex < 0)
				m_SelectionIndex = m_CurrentSubMenu->m_NumOptions - 1;

			playSoundFrontend("NAV_UP", "Ledger_Sounds");
		}


		if (m_DownKeyPressed) {
			if (GetSelectedOption() == nullptr) return;

			if (IsKeyDownLong(VK_SHIFT)) {
				m_SelectionIndex += 10;
				if (m_SelectionIndex > m_CurrentSubMenu->m_NumOptions - 1)
					m_SelectionIndex = m_CurrentSubMenu->m_NumOptions - 1; // DONT reset to the TOP of the page
			} else {
				m_SelectionIndex++;
			}

			if (GetSelectedOption()->m_IsPageBreak)
				m_SelectionIndex++;

			if (m_SelectionIndex > m_CurrentSubMenu->m_NumOptions - 1)
				m_SelectionIndex = 0;

			playSoundFrontend("NAV_DOWN", "Ledger_Sounds");
		}


		if (m_LeftKeyPressed) {
			// TODO: Hold shift to go left 2 selections
			Option* option = GetSelectedOption();
			if (option == nullptr) return;

			if (option->m_IsVectorOption) {
				option->ExecuteVectorFunc(true, false);
				playSoundFrontend("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
			}
		}


		if (m_RightKeyPressed) {
			// TODO: Hold shift to go right 2 selections
			Option* option = GetSelectedOption();
			if (option == nullptr) return;

			if (option->m_IsVectorOption) {
				option->ExecuteVectorFunc(false, true);
				playSoundFrontend("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");
			}
		}
	}
}


void CNativeMenu::DisableCommonInputs()
{
	if (m_IsOpen) {
		if (m_PadIndex == INPUT_GROUP_CONTROLLER) {
			*getGlobalPtr(1900383 + 316) = 2; // Disables horse whistling this frame. Not sure if this is safe, but it works.
			PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_CONTROLLER, INPUT_WHISTLE, false);
			PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_CONTROLLER, INPUT_WHISTLE_HORSEBACK, false);
		}

		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_SELECT_RADAR_MODE, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_REVEAL_HUD, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_PLAYER_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_OPEN_JOURNAL, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_OPEN_SATCHEL_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_OPEN_CRAFTING_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_OPEN_EMOTE_WHEEL, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_EXPAND_RADAR, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_INTERACTION_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_PadIndex, INPUT_HUD_SPECIAL, false);
	}
}


void CNativeMenu::RegisterUIPrompts()
{
	auto reg = [](Prompt &_prompt, const char* text, Hash control) 
	{ 
		_prompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
		HUD::_UI_PROMPT_SET_CONTROL_ACTION(_prompt, control);
		HUD::_UI_PROMPT_SET_PRIORITY(_prompt, 2); // PP_High
		HUD::_UI_PROMPT_SET_TEXT(_prompt, MISC::VAR_STRING(10, "LITERAL_STRING", text));
		HUD::_UI_PROMPT_SET_STANDARD_MODE(_prompt, TRUE);
		// Allows multiple prompts (>2) of the same type to be shown
		HUD::_UI_PROMPT_SET_ATTRIBUTE(_prompt, 34, true);
		HUD::_UI_PROMPT_REGISTER_END(_prompt);

		HUD::_UI_PROMPT_SET_VISIBLE(_prompt, false);
		HUD::_UI_PROMPT_SET_ENABLED(_prompt, false);
	};

	if (m_SelectPrompt == NULL)
		reg(m_SelectPrompt, "Select", INPUT_GAME_MENU_ACCEPT);

	if (m_BackPrompt == NULL)
		reg(m_BackPrompt, "Back", INPUT_GAME_MENU_CANCEL);
}

