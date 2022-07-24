// Licensed under the MIT License.

#pragma once
#include "option.hpp"
#include "submenu.hpp"
#include "../script.h"
#include "../common.hpp"


#define BUILD_1311_COMPATIBLE FALSE // If true, the menu will be compatible with game version <= 1311.12
#define ALLOCATE_CONSOLE FALSE // To be used for debugging purposes
#if ALLOCATE_CONSOLE
	#define LOG(msg)(std::cout << msg << std::endl)
#endif


const float SCREEN_WIDTH = 1920.0f;		// TODO: Maybe convert to use Util::g_screenWidth?
const float SCREEN_HEIGHT = 1080.0f;	// TODO: Maybe convert to use Util::g_screenHeight?

const float BG_X_OFFSET = 25.0f;		// Background X Offset
const float BG_Y_OFFSET = 25.0f;		// Background Y Offset
const float BG_WIDTH    = 576.0f;		// Background Width
const float BG_HEIGHT   = 1026.0f;		// Background Height

const float TOP_HEADER_WIDTH  = 442.0f;	// Header Width
const float TOP_HEADER_HEIGHT = 108.0f;	// Header Height
const float TOP_HEADER_X_POS = BG_X_OFFSET + ((BG_WIDTH * 0.5f) - (TOP_HEADER_WIDTH * 0.5f));	// Header X Offset
const float TOP_HEADER_Y_POS = BG_Y_OFFSET + ((BG_WIDTH - TOP_HEADER_WIDTH) * 0.2f);			// Header Y Offset

//const float FOOTER_LINE_WIDTH  = 422.0f; 
//const float FOOTER_LINE_HEIGHT = 2.0f;
//const float FOOTER_LINE_X_POS = BG_X_OFFSET + ((BG_WIDTH * 0.5f) - (FOOTER_LINE_WIDTH * 0.5f));
//const float FOOTER_LINE_Y_POS = BG_Y_OFFSET + (BG_HEIGHT - ((BG_WIDTH - FOOTER_LINE_WIDTH) * 0.5f));

// Y increment for option and sprite positions
const float INCREMENT = (SCREEN_HEIGHT * 0.051f); // 55


namespace Menu
{
	inline std::unordered_map<int, Submenu> g_SubmenusMap;


	class CNativeMenu
	{
	public:
		Submenu* CurrentSubmenu = nullptr;			// Current Submenu
		int m_CurrentSubmenuID = Submenu_Invalid;	// Current Submenu ID
		int m_SelectionIndex = 0;					// Current Selected Option Index

		void AddSubmenu(const std::string& header, const std::string& subHeader, int id, int numVisibleOptions, std::function<void(Submenu*)> submenuFunc)
		{
			Submenu submenu(header, subHeader, id, numVisibleOptions, submenuFunc);
			g_SubmenusMap[id] = submenu;
		}

		// NOTE: THIS FUNCTION IS A LITTLE BROKEN
		// bSetRememberedSelection: Should be TRUE if going backwards
		void GoToSubmenu(int id, bool bSetRememberedSelection)
		{
			if (!g_SubmenusMap.contains(id)) {
#if ALLOCATE_CONSOLE
				std::cout << "[CNativeMenu::GoToSubmenu] [ERROR]: Submenu '" << id << "' doesn't exist" << "\n";
#endif
				return;
			}

			// This should only be the case when first initializing the menu
			if (CurrentSubmenu == nullptr) {
				CurrentSubmenu = &g_SubmenusMap[id];
				return;
			}

			if (id > CurrentSubmenu->m_ID) {
				// we went forward
				m_PrevSubMenuIds.push_back(CurrentSubmenu->m_ID);
				m_SubMenuLastSelections[CurrentSubmenu->m_ID] = m_SelectionIndex;
			}
			else if (id < CurrentSubmenu->m_ID) {
				// we went back
				m_PrevSubMenuIds.pop_back();
				m_SubMenuLastSelections.erase(CurrentSubmenu->m_ID);
			}

			CurrentSubmenu = &g_SubmenusMap[id];

			if (bSetRememberedSelection)
				m_SelectionIndex = m_SubMenuLastSelections[CurrentSubmenu->m_ID];
			else
				m_SelectionIndex = 0;
		}

		Submenu* GetSubmenu(int id)
		{
			if (g_SubmenusMap.contains(id)) {
				return &g_SubmenusMap[id];
			}

#if ALLOCATE_CONSOLE
			std::cout << "[CNativeMenu::GetSubmenu] [ERROR]: Invalid ID (" << id << "), returning nullptr" << "\n";
#endif
			return nullptr;
		}

		Option* GetSelectedOption()
		{
			return &g_SubmenusMap[CurrentSubmenu->m_ID].m_Options[m_SelectionIndex];
		}

		bool DoesSubmenuExist(int id)
		{
			return g_SubmenusMap.contains(id);
		}

		// Remove submenus entirely
		void RemoveSubmenusAtAndAfterThisID(int id)
		{
			for (auto it = g_SubmenusMap.begin(); it != g_SubmenusMap.end(); it++) {
				if (it->first >= id) {
					g_SubmenusMap.erase(it);
				}
			}
		}

		// Remove a submenu entirely
		void RemoveSubmenu(int id...)
		{
			std::vector<int> toRemove = { id };
			for (int i = 0; i < toRemove.size(); i++) {
				if (DoesSubmenuExist(toRemove[i])) {
					g_SubmenusMap.erase(toRemove[i]);
				}
			}
		}

		// Clear all options from a submenu
		void ClearSubmenu(Submenu* sub...)
		{
			std::vector<Submenu*> toClear = { sub };
			for (int i = 0; i < toClear.size(); i++) {
				if (DoesSubmenuExist(toClear[i]->m_ID)) {
					toClear[i]->Clear();
				}
			}
		}

		// Clear all options from a submenu
		void ClearSubmenu(int id...)
		{
			std::vector<int> toClear = { id };
			for (int i = 0; i < toClear.size(); i++) {
				if (DoesSubmenuExist(toClear[i])) {
					GetSubmenu(toClear[i])->Clear();
				}
			}
		}
		
		bool IsOpen()
		{
			return m_IsOpen;
		}


		void SetEnabled(bool bEnabled, bool bPlaySounds);
		void Update();
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
