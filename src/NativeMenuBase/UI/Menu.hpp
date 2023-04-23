// Licensed under the MIT License.

#pragma once
#include "Submenu.hpp"
#include "SubmenuIDs.h"
#include "../script.h"
#include "../common.hpp"

#define BUILD_1311_COMPATIBLE 0 // If true, the menu will be compatible with game version <= 1311.12
#define SUBMENU g_Menu->AddSubmenu

class CNativeMenu
{
private:
	std::unordered_map<int, Submenu*> g_SubmenusMap;
public:
	Submenu* CurrentSubmenu = nullptr;							// Current Submenu
	eSubmenuID CurrentSubmenuID = eSubmenuID::Submenu_Invalid;	// Current Submenu ID
	int SelectionIndex = 0;										// Current Selected Option Index

	// ctor
	CNativeMenu()
	{
		this->RegisterUIPrompts();
	}

	void AddSubmenu(const std::string& header, const std::string& subHeader, eSubmenuID id, int numVisibleOptions, std::function<void(Submenu*)> submenuFunc)
	{
		// Have to use "new" here because "std::vector<std::unique_ptr<Option>> mOptions{};"
		// complains that the objects are being copied. Now we have to use pointers.
		// Would rather just store a copy in g_SubmenusMap, and if we need to get the submenu, return a pointer to it.

		Submenu* submenu = new Submenu(header, subHeader, id, numVisibleOptions, submenuFunc);
		g_SubmenusMap[id] = submenu;
	}


	// Go to a submenu
	// 
	// Note: If you're skipping nested submenus by calling this, it might be a good idea to
	// call this function on those submenus right before the one you need to go to so data gets properly set
	void GoToSubmenu(eSubmenuID id)
	{
		bool setRememberedSelection = false;

		if (!g_SubmenusMap.contains(id)) {
			PRINT_ERROR("Submenu ID ",(int)id," doesn't exist");
			return;
		}

		// This should ONLY be the case when the menu is first created
		if (CurrentSubmenu == nullptr) {
			CurrentSubmenu = g_SubmenusMap[id];
			return;
		}

		if (id > CurrentSubmenu->ID) {
			// went forward
			m_PreviousSubmenus.push_back(CurrentSubmenu->ID);
			m_LastSubmenuSelections[CurrentSubmenu->ID] = SelectionIndex;
		}
		else if (id < CurrentSubmenu->ID) {
			// went back
			m_PreviousSubmenus.pop_back();
			m_LastSubmenuSelections.erase(CurrentSubmenu->ID);
			setRememberedSelection = true;
		}

		CurrentSubmenu = g_SubmenusMap[id];

		if (setRememberedSelection) {
			SelectionIndex = m_LastSubmenuSelections[CurrentSubmenu->ID];
		}
		else {
			SelectionIndex = 0;
		}
	}

	// Gets a submenu via its ID
	Submenu* GetSubmenu(eSubmenuID id)
	{
		if (g_SubmenusMap.contains(id)) {
			return g_SubmenusMap[id];
		}

		PRINT_ERROR("Submenu ID ",(int)id," doesn't exist, returning null");
		return nullptr;
	}

	// Gets the currently selected option in the current submenu
	Option* GetSelectedOption()
	{
		Option* option = g_SubmenusMap[CurrentSubmenu->ID]->GetOption(SelectionIndex);
		if (option == nullptr) {
			PRINT_ERROR("Failed to find option in submenu ", (int)CurrentSubmenu->ID, " with index ", SelectionIndex, ", returning null");
		}
		return option;
	}

	// Checks if a submenu exists
	bool DoesSubmenuExist(eSubmenuID id)
	{
		return g_SubmenusMap.contains(id);
	}

	// Delete a submenu
	void DeleteSubmenu(Submenu* pSubmenu)
	{
		if (pSubmenu == nullptr) {
			PRINT_ERROR("Failed to delete submenu, pSubmenu is null");
			return;
		}

		if (DoesSubmenuExist(pSubmenu->ID)) {
			pSubmenu->Clear();
			delete g_SubmenusMap[pSubmenu->ID];
			g_SubmenusMap.erase(pSubmenu->ID);
		}
	}

	// Checks if the menu is currently open
	bool IsOpen() const
	{
		return m_IsOpen;
	}

	/// <summary>
	/// Set if the menu should be open or closed
	/// </summary>
	/// <param name="bEnabled">- Open/Closed</param>
	/// <param name="bPlaySounds">- Play open and close sounds</param>
	void SetEnabled(bool bEnabled, bool bPlaySounds = true);

	// Renders the menu
	void Update();

private:
	// Misc Variables

	bool m_IsOpen = false;
	// Contains submenu IDs you were previously at to properly backtrack
	std::vector<eSubmenuID> m_PreviousSubmenus = {};
	// Contains the last selection you were at in a sub menu
	std::unordered_map<eSubmenuID, int> m_LastSubmenuSelections = {};

	// Input

	int m_ControlIndex = INPUT_GROUP_KEYBOARD;
	bool m_OpenKeyPressed = false;
	bool m_BackKeyPressed = false;
	bool m_EnterKeyPressed = false;
	bool m_UpKeyPressed = false;
	bool m_DownKeyPressed = false;
	bool m_LeftKeyPressed = false;
	bool m_RightKeyPressed = false;
	bool m_LMBPressed = false;

	// Prompts

	Prompt m_SelectPrompt = NULL;
	Prompt m_BackPrompt = NULL;
	bool m_SelectPromptCompleted = false;
	bool m_BackPromptCompleted = false;

	// Functions

	void CheckInput();
	void HandleInput();
	void DisableCommonInputs();
	void RegisterUIPrompts();

	void HandleEnterPressed();
	void HandleBackPressed();
	void HandleUpKeyPressed();
	void HandleDownKeyPressed();
	void HandleLeftKeyPressed();
	void HandleRightKeyPressed();
	void HandleMouseInput(); // TODO
};

inline std::unique_ptr<CNativeMenu> g_Menu;
