// Licensed under the MIT License.

#pragma once
#include "option.hpp"
#include "submenu.hpp"
#include "../script.h"
#include "../common.hpp"


#define BUILD_1311_COMPATIBLE FALSE // If true, the menu will be compatible with game version <= 1311.12
#define ALLOCATE_CONSOLE FALSE // To be used for debugging purposes


namespace Menu
{
	inline std::unordered_map<int, Submenu> m_SubMenus;


	class CNativeMenu
	{
	public:
		Submenu* m_CurrentSubMenu = nullptr;	// Current Submenu
		int m_SelectionIndex = 0;				// Current Selected Option Index

		void AddSubmenu(const std::string& header, const std::string& subHeader, int id, int numVisibleOptions, std::function<void(Submenu*)> func)
		{
			Submenu submenu(header, subHeader, id, numVisibleOptions, func);
			m_SubMenus[id] = submenu;
		}

		// NOTE: THIS FUNCTION IS A LITTLE BROKEN
		// bSetRememberedSelection: Should be TRUE if going backwards
		void GoToSubmenu(int id, bool bSetRememberedSelection)
		{
			if (!m_SubMenus.contains(id)) {
#if ALLOCATE_CONSOLE
				std::cout << "[CNativeMenu::GoToSubmenu] [ERROR]: Submenu '" << id << "' doesn't exist" << "\n";
#endif
				return;
			}

			// This should only be the case when first initializing the menu
			if (m_CurrentSubMenu == nullptr) {
				m_CurrentSubMenu = &m_SubMenus[id];
				return;
			}

			if (id > m_CurrentSubMenu->m_ID) {
				// we went forward
				m_PrevSubMenuIds.push_back(m_CurrentSubMenu->m_ID);
				m_SubMenuLastSelections[m_CurrentSubMenu->m_ID] = m_SelectionIndex;
			}
			else if (id < m_CurrentSubMenu->m_ID) {
				// we went back
				m_PrevSubMenuIds.pop_back();
				m_SubMenuLastSelections.erase(m_CurrentSubMenu->m_ID);
			}

			m_CurrentSubMenu = &m_SubMenus[id];

			if (bSetRememberedSelection)
				m_SelectionIndex = m_SubMenuLastSelections[m_CurrentSubMenu->m_ID];
			else
				m_SelectionIndex = 0;
		}

		Submenu* GetSubmenu(int id)
		{
			if (m_SubMenus.contains(id)) {
				return &m_SubMenus[id];
			}

#if ALLOCATE_CONSOLE
			std::cout << "[CNativeMenu::GetSubmenu] [ERROR]: Invalid ID (" << id << "), returning nullptr" << "\n";
#endif
			return nullptr;
		}

		Option* GetSelectedOption()
		{
			return &m_SubMenus[m_CurrentSubMenu->m_ID].m_Options[m_SelectionIndex];
		}

		bool DoesSubMenuExist(int id)
		{
			return m_SubMenus.contains(id);
		}

		void ClearSubmenusAtAndAfterThisID(int id)
		{
			for (auto it = m_SubMenus.begin(); it != m_SubMenus.end(); it++) {
				if (it->first >= id) {
					m_SubMenus.erase(it);
				}
			}
		}


		void SetEnabled(bool bEnabled, bool bPlaySounds);
		void LoopFunc();
		void RegisterUIPrompts();

	private:
		// Misc Variables

		bool m_IsOpen = false;
		// Contains submenu IDs you were previously at to properly backtrack
		std::vector<int> m_PrevSubMenuIds = {};
		// Contains the last selection you were at in a sub menu
		// Submenu ID, Selection Index
		std::unordered_map<int, int> m_SubMenuLastSelections = {};

		// Input

		int m_PadIndex = INPUT_GROUP_KEYBOARD;
		bool m_OpenKeyPressed = false;
		bool m_BackKeyPressed = false;
		bool m_EnterKeyPressed = false;
		bool m_UpKeyPressed = false;
		bool m_DownKeyPressed = false;
		bool m_LeftKeyPressed = false;
		bool m_RightKeyPressed = false;

		// Prompts

		Prompt m_SelectPrompt = NULL;
		Prompt m_BackPrompt = NULL;
		bool m_SelectPromptCompleted = false;
		bool m_BackPromptCompleted = false;

		// Func declarations

		void CheckInput();
		void HandleInput();
		void DisableCommonInputs();
	};

	
	inline std::unique_ptr<CNativeMenu> g_NativeMenu;
}
