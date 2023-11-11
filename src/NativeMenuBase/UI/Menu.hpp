// Licensed under the MIT License.

#pragma once
#include "../script.h"
#include "Submenu.hpp"
#include "SubmenuIDs.h"

#define BUILD_1311_COMPATIBLE 1 // If true, the menu will be compatible with game version <= 1311.12

class CNativeMenu
{
private:
	std::unordered_map<eSubmenuID, CSubmenu*> m_SubmenusMap{};

	CSubmenu* m_CurrentSubmenu = nullptr;                // Current Submenu
	eSubmenuID m_CurrentSubmenuID = eSubmenuID::Invalid; // Current Submenu ID
	int m_SelectionIndex = 0;                            // Current Selected Option Index
public:

	CNativeMenu()
	{
		PRINT_INFO("Registering ", __submenus.size(), " submenus");
		for (int i = 0; i < __submenus.size(); i++) {
			CSubmenu* sub = static_cast<CSubmenu*>(__submenus[i]);
			this->RegisterSubmenu(sub, true);
		}
		PRINT_INFO("Successfully registered ", m_SubmenusMap.size(), " submenus");

		// We no longer need this
		__submenus.clear();
		__submenus.shrink_to_fit();

		this->RegisterUIPrompts();
		this->GoToSubmenu(eSubmenuID::EntryMenu);
	}

	/// <summary>
	/// Register a new submenu
	/// </summary>
	/// <param name="pSubmenu">- The submenu to register</param>
	/// <param name="bInitialize">- Automatically initialize the submenu</param>
	void RegisterSubmenu(CSubmenu* pSubmenu, bool bInitialize)
	{
		if (pSubmenu == nullptr) {
			PRINT_ERROR("Failed to register submenu, pSubmenu is null");
			return;
		}

		if (m_SubmenusMap.contains(pSubmenu->ID)) {
			PRINT_WARN("Submenu with ID ", (int)pSubmenu->ID, " has already been registered");
			return;
		}

		m_SubmenusMap[pSubmenu->ID] = pSubmenu;
		if (bInitialize) {
			pSubmenu->Init();
		}
	}

	// Go to a submenu
	// 
	// Note: If you're skipping nested submenus by calling this, it might be a good idea to
	// call this function on those submenus right before the one you need to go to so data gets properly set
	void GoToSubmenu(eSubmenuID id)
	{
		bool setRememberedSelection = false;

		if (!m_SubmenusMap.contains(id)) {
			PRINT_ERROR("Submenu ID ",(int)id," doesn't exist");
			return;
		}

		// This should ONLY be the case when the menu is first created
		if (m_CurrentSubmenu == nullptr) {
			m_CurrentSubmenu = m_SubmenusMap[id];
			return;
		}

		if (id > m_CurrentSubmenu->ID) {
			// went forward
			m_PreviousSubmenus.push_back(m_CurrentSubmenu->ID);
			m_LastSubmenuSelections[m_CurrentSubmenu->ID] = m_SelectionIndex;
		}
		else if (id < m_CurrentSubmenu->ID) {
			// went back
			m_PreviousSubmenus.pop_back();
			m_LastSubmenuSelections.erase(m_CurrentSubmenu->ID);
			setRememberedSelection = true;
		}

		m_CurrentSubmenu->OnExit();
		m_CurrentSubmenu = m_SubmenusMap[id];
		m_CurrentSubmenu->OnEnter();

		if (setRememberedSelection) {
			m_SelectionIndex = m_LastSubmenuSelections[m_CurrentSubmenu->ID];
		}
		else {
			m_SelectionIndex = 0;
		}
	}

	// Run all submenu tick functions.
	// By default, this is not called. If you want your tick functions to always run,
	// uncomment this line above the if statement in CNativeMenu::Update() in Menu.cpp
	// 
	// RunTickFunctions();
	//
	// Comment out "m_CurrentSubmenu->OnTick();" in Menu.cpp if you intend on calling this.
	void RunTickFunctions()
	{
		for (auto it = m_SubmenusMap.begin(); it != m_SubmenusMap.end(); it++)
		{
			it->second->OnTick();
		}
	}

	// Gets a submenu from its ID
	CSubmenu* GetSubmenu(eSubmenuID id)
	{
		if (m_SubmenusMap.contains(id)) {
			return m_SubmenusMap[id];
		}

		PRINT_ERROR("Submenu ID ",(int)id," doesn't exist, returning null");
		return nullptr;
	}

	// Gets the currently selected option in the current submenu
	COption* GetSelectedOption()
	{
		COption* option = m_SubmenusMap[m_CurrentSubmenu->ID]->GetOption(m_SelectionIndex);
		if (option == nullptr) {
			PRINT_ERROR("Failed to find option in submenu ", (int)m_CurrentSubmenu->ID, " with index ", m_SelectionIndex, ", returning null");
		}
		return option;
	}

	// Returns a pointer to the current submenu
	inline CSubmenu* GetCurrentSubmenu()
	{
		return m_CurrentSubmenu;
	}

	// Get the current selection index
	inline int GetSelectionIndex()
	{
		return m_SelectionIndex;
	}

	// Set the current selection index
	void SetSelectionIndex(int newIndex)
	{
		if (newIndex > m_CurrentSubmenu->GetNumberOfOptions() - 1)
		{
			newIndex = m_CurrentSubmenu->GetNumberOfOptions() - 1;
		}
		m_SelectionIndex = newIndex;
	}

	// Checks if a submenu exists
	inline bool DoesSubmenuExist(eSubmenuID id)
	{
		return m_SubmenusMap.contains(id);
	}

	// Delete a submenu
	void DeleteSubmenu(CSubmenu* pSubmenu)
	{
		if (pSubmenu == nullptr) {
			PRINT_ERROR("Failed to delete submenu, pSubmenu is null");
			return;
		}

		if (this->DoesSubmenuExist(pSubmenu->ID)) {
			pSubmenu->Clear();
			m_SubmenusMap.erase(pSubmenu->ID);
		}
	}

	// Checks if the menu is currently open
	inline bool IsOpen() const
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
