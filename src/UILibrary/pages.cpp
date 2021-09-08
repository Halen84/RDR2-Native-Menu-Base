// Licensed under the MIT License.

#include <string>
#include <map>
#include "script.h"
#include "menu.h"
#include "pages.h"
#include "menuitemfunctions.h"

int optionsInThisPage;

void DrawPage(double pageIndex)
{
	if (pageIndex == 0.0) {
		SetHeader("HEADER");
		SetSubHeader("Sub Header");
		DrawListOption("Page 1", 0);
		DrawListOption("Page 2", 1);
		DrawListOption("Page 3", 2);
		DrawListOption("Page 4", 3);
		DrawListOption("Page 5", 4);
		DrawListOption("Page 6", 5);
		DrawListOption("Page 7", 6);
		DrawListOption("Page 8", 7);
		optionsInThisPage = 8;
	}

	else if (pageIndex == 1.0) {
		SetHeader("PAGE 1 HEADER");
		SetSubHeader("Page 1 Sub Header");
		DrawListOption("Page 1 - Option 1", 0);
		DrawListOption("Page 1 - Option 2", 1);
		DrawListOption("Page 1 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 1.1) {
		SetHeader("PAGE 1.1 HEADER");
		SetSubHeader("Page 1.1 Sub Header");
		// Remember, DrawListOption and DrawToggleOption are different!
		DrawToggleOption("Page 1.1 - Toggle Option 1", 0);
		DrawToggleOption("Page 1.1 - Toggle Option 2", 1);
		DrawToggleOption("Page 1.1 - Toggle Option 3", 2);
		DrawToggleOption("Page 1.1 - Toggle Option 4", 3);
		DrawToggleOption("Page 1.1 - Toggle Option 5", 4);
		optionsInThisPage = 5;
	}

	else if (pageIndex == 2.0) {
		SetHeader("PAGE 2 HEADER");
		SetSubHeader("Page 2 Sub Header");
		DrawListOption("Page 2 - Option 1", 0);
		DrawListOption("Page 2 - Option 2", 1);
		DrawListOption("Page 2 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 3.0) {
		SetHeader("PAGE 3 HEADER");
		SetSubHeader("Page 3 Sub Header");
		DrawListOption("Page 3 - Option 1", 0);
		DrawListOption("Page 3 - Option 2", 1);
		DrawListOption("Page 3 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 4.0) {
		SetHeader("PAGE 4 HEADER");
		SetSubHeader("Page 4 Sub Header");
		DrawListOption("Page 4 - Option 1", 0);
		DrawListOption("Page 4 - Option 2", 1);
		DrawListOption("Page 4 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 5.0) {
		SetHeader("PAGE 5 HEADER");
		SetSubHeader("Page 5 Sub Header");
		DrawListOption("Page 5 - Option 1", 0);
		DrawListOption("Page 5 - Option 2", 1);
		DrawListOption("Page 5 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 6.0) {
		SetHeader("PAGE 6 HEADER");
		SetSubHeader("Page 6 Sub Header");
		DrawListOption("Page 6 - Option 1", 0);
		DrawListOption("Page 6 - Option 2", 1);
		DrawListOption("Page 6 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 7.0) {
		SetHeader("PAGE 7 HEADER");
		SetSubHeader("Page 7 Sub Header");
		DrawListOption("Page 7 - Option 1", 0);
		DrawListOption("Page 7 - Option 2", 1);
		DrawListOption("Page 7 - Option 3", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 8.0) {
		SetHeader("PAGE 8 HEADER");
		SetSubHeader("Page 8 Sub Header");
		DrawListOption("Page 8 - Option 1", 0);
		DrawListOption("Page 8 - Option 2", 1);
		DrawListOption("Page 8 - Option 3", 2);
		optionsInThisPage = 3;
	}
}


void OnSelect()
{
	CallFunction(GetCurrentPageIndex(), GetCurrentSelectedIndex(), false);
}


 #pragma region Toggle Option States

std::map<double, std::map<int, bool>> toggleStates;

std::map<double, std::map<int, std::string>> textStates
{
	{1.1, {
		{0, "Off"}, {1, "Off"}, {2, "Off"}, {3, "Off"}, {4, "Off"}
	}},
};

 #pragma endregion


void OnToggle()
{
	double pageIndex = GetCurrentPageIndex();
	int selectedIndex = GetCurrentSelectedIndex();
	
	// If it doesn't exist in the hashmap, add it
	toggleStates[pageIndex][selectedIndex] = GetTextState(pageIndex, selectedIndex) == "On" ? true : false;
	// Reverse state
	toggleStates[pageIndex][selectedIndex] = !toggleStates[pageIndex][selectedIndex];

	textStates[pageIndex][selectedIndex] = toggleStates[pageIndex][selectedIndex] == true ? "On" : "Off";
	CallFunction(pageIndex, selectedIndex, toggleStates[pageIndex][selectedIndex]);
}


bool DoesOptionHavePage(double pageIndex, int selectedIndex)
{
	if (pageIndex == 1.0) {
		// If you want an option to have a page, then put it's index in these conditions like so
		// Be sure to add that page in DrawPage
		if (selectedIndex == 0 /* || selectedIndex == 1 */) {
			return true;
		}
	}
	// Continue doing else if's

	return false;
}


bool DoesOptionHaveToggle(double pageIndex, int selectedIndex)
{
	// Same as DoesOptionHavePage
	if (pageIndex == 1.1) {
		if (selectedIndex >= 0 && selectedIndex <= 4) {
			return true;
		}
	}

	return false;
}


std::string GetTextState(double pageIndex, int selectedIndex) { return textStates[pageIndex][selectedIndex]; }
int GetNumOptionsInCurrentPage() { return optionsInThisPage; }
