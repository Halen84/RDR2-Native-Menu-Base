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


void CNativeMenu::Update()
{
	CheckInput();
	HandleInput();
	DisableCommonInputs(); // Uses m_IsOpen
	
	if (m_IsOpen) {
		Drawing::DrawMenuTextures();
		Drawing::DrawSelectionBox();
		Drawing::DrawHeader(CurrentSubmenu->m_Header);
		Drawing::DrawSubHeader(CurrentSubmenu->m_SubHeader);
		
		for (int i = 0; i < CurrentSubmenu->m_NumOptions; i++) {
			Option* option = CurrentSubmenu->GetOption(i);
			if (option != nullptr) {
				Drawing::DrawOption(option);
			} else {
#if ALLOCATE_CONSOLE
				std::cout << "[CNativeMenu::Update] [ERROR]: Option is a nullptr at index " << i << "\n";
#endif
			}
		}

		Drawing::DrawOptionCounter();
	}

	if (CurrentSubmenu != nullptr) {
		m_CurrentSubmenuID = CurrentSubmenu->m_ID;
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
		if (m_IsOpen)
			playSoundFrontend("MENU_ENTER", "HUD_PLAYER_MENU");
		else
			playSoundFrontend("MENU_CLOSE", "HUD_PLAYER_MENU");
	}

	if (m_IsOpen)
		MAP::DISPLAY_RADAR(false);
	else
		MAP::DISPLAY_RADAR(true);
}


void CNativeMenu::CheckInput()
{
	m_ControlIndex = PAD::IS_USING_KEYBOARD_AND_MOUSE(0) ? 0 : 2;

	m_OpenKeyPressed	= IsKeyJustUp(VK_F5) || PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) && PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A);
	m_EnterKeyPressed	= PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_ACCEPT);
	m_BackKeyPressed	= PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_CANCEL);
	m_UpKeyPressed		= PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_UP);
	m_DownKeyPressed	= PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_DOWN);
	m_LeftKeyPressed	= PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_LEFT);
	m_RightKeyPressed	= PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_RIGHT);

	if (HUD::_UI_PROMPT_IS_VALID(m_SelectPrompt) && HUD::_UI_PROMPT_IS_ACTIVE(m_SelectPrompt))
		m_SelectPromptCompleted = HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(m_SelectPrompt, 0);
	else
		m_SelectPromptCompleted = m_EnterKeyPressed; // Fallback to use raw input.

	if (HUD::_UI_PROMPT_IS_VALID(m_BackPrompt) && HUD::_UI_PROMPT_IS_ACTIVE(m_SelectPrompt))
		m_BackPromptCompleted = HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(m_BackPrompt, 0);
	else
		m_BackPromptCompleted = m_BackKeyPressed; // Fallback to use raw input.
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
					*option->GetBoolPtr() = !*option->GetBoolPtr(); // Flip
				} else {
#if ALLOCATE_CONSOLE
					std::cout << "[CNativeMenu::HandleInput] [WARNING]: GetBoolPtr at option index " << m_SelectionIndex << " is a nullptr" << "\n";
#endif
				}

				option->ExecuteFunc();
			}
			else if (option->m_IsVectorOption) {
				option->ExecuteVectorFunc(false, true);
			}
			else if (option->m_IsSubmenuOption) {
				if (DoesSubmenuExist(option->m_SubmenuID)) {
					//g_NativeMenu->GoToSubmenu(option->m_SubmenuID, false);
					m_PrevSubmenuIds.push_back(CurrentSubmenu->m_ID);
					m_SubmenuLastSelections[CurrentSubmenu->m_ID] = m_SelectionIndex;
					CurrentSubmenu = &g_SubmenusMap[option->m_SubmenuID];
					m_SelectionIndex = 0;
				}
				else {
#if ALLOCATE_CONSOLE
					std::cout << "[CNativeMenu::HandleInput] [ERROR]: Submenu " << option->m_SubmenuID << " doesn't exist." << "\n";
#endif
				}
			}
			playSoundFrontend("SELECT", "HUD_SHOP_SOUNDSET");
		}


		if (m_BackPromptCompleted) {
			if (CurrentSubmenu->m_ID <= Submenu_EntryMenu) {
				playSoundFrontend("BACK", "HUD_SHOP_SOUNDSET");
				SetEnabled(false, true);
				return;
			}

			if (m_PrevSubmenuIds.size() > 0) {
				//g_NativeMenu->GoToSubmenu(m_PrevSubmenuIds[m_PrevSubmenuIds.size() - 1], true);
				CurrentSubmenu = &g_SubmenusMap[m_PrevSubmenuIds[m_PrevSubmenuIds.size() - 1]];
				m_PrevSubmenuIds.pop_back();
				m_SelectionIndex = m_SubmenuLastSelections[CurrentSubmenu->m_ID];
				m_SubmenuLastSelections.erase(CurrentSubmenu->m_ID);
			} else {
				CurrentSubmenu = &g_SubmenusMap[Submenu_EntryMenu];
				m_SelectionIndex = m_SubmenuLastSelections[Submenu_EntryMenu];
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
				m_SelectionIndex = CurrentSubmenu->m_NumOptions - 1;

			playSoundFrontend("NAV_UP", "Ledger_Sounds");
		}


		if (m_DownKeyPressed) {
			if (GetSelectedOption() == nullptr) return;

			if (IsKeyDownLong(VK_SHIFT)) {
				m_SelectionIndex += 10;
				if (m_SelectionIndex > CurrentSubmenu->m_NumOptions - 1)
					m_SelectionIndex = CurrentSubmenu->m_NumOptions - 1; // DONT reset to the TOP of the page
			} else {
				m_SelectionIndex++;
			}

			if (GetSelectedOption()->m_IsPageBreak)
				m_SelectionIndex++;

			if (m_SelectionIndex > CurrentSubmenu->m_NumOptions - 1)
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
		if (m_ControlIndex == INPUT_GROUP_CONTROLLER) {
			*getGlobalPtr(1900383 + 316) = 2; // Disables horse whistling this frame. Not sure if this is safe, but it works.
			PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_CONTROLLER, INPUT_WHISTLE, false);
			PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_CONTROLLER, INPUT_WHISTLE_HORSEBACK, false);
		}

		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_SELECT_RADAR_MODE, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_REVEAL_HUD, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_PLAYER_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_JOURNAL, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_CRAFTING_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_EMOTE_WHEEL, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_EXPAND_RADAR, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_INTERACTION_MENU, false);
		PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_HUD_SPECIAL, false);
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
		// Allows multiple prompts of the same type to be shown (?)
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

