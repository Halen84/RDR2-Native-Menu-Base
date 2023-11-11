// Licensed under the MIT License.

#pragma once
#include "../UI/Menu.hpp"

class CEntrySubmenu : public CSubmenu
{
public:
	static bool bBooleanVariable;

public:
	// The constructor can either be implemented here, or in the source file.
	// Make sure the accessibility is public.
	CEntrySubmenu()
	{
		Header = "HEADER";
		SubHeader = "Sub Header";
		ID = eSubmenuID::EntryMenu;
		NumberOfVisibleOptions = 8;
	}

	// Pure virtual function, must be implemented.
	void Init() override;

	// Optional implementation for these functions. See CSubmenu.
	void OnEnter() override;
	void OnExit() override;
	void OnTick() override;
};

// Declaring a singleton variable like this will automatically call the ctor to the class (and Init()).
// If you do not want a singleton, then you can call RegisterWithoutSingleton() in script.cpp.
// If you do create a singleton, then do not call RegisterWithoutSingleton().
inline CEntrySubmenu g_EntrySubmenu{};
