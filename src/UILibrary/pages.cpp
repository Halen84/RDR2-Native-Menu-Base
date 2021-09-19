// Licensed under the MIT License.

#include <string>
#include <map>
#include <vector>
#include "script.h"
#include "menu.h"
#include "pages.h"
#include "menuitemfunctions.h"

// todo: automate this...
int optionsInThisPage;

void DrawPage(double pageIndex)
{
	if (pageIndex == 0.0) {
		SetHeader("HEADER");
		SetSubHeader("Sub Header");
		DrawListOption("Examples", 0);
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
		DrawListOption("Toggle Examples", 0);
		DrawListOption("Example Button", 1);
		DrawListOption("Example Button", 2);
		optionsInThisPage = 3;
	}

	else if (pageIndex == 1.1) {
		SetHeader("PAGE 1.1 HEADER");
		SetSubHeader("Page 1.1 Sub Header");

		// Add options with static text
		DrawToggleOption("Multiple Options", 0);
		AddMultipleOptions("BaseText ", 0, 10);

		// Add options with unique text
		DrawToggleOption("Single Options", 1);
		AddSingleOption("First", 1); AddSingleOption("Second", 1); AddSingleOption("Third", 1);

		// You can also not add any options for just On and Off
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
	CallFunction(GetCurrentPageIndex(), GetCurrentSelectedIndex(), false, -1);
}


 #pragma region Toggle Option Hashmaps
// SORRY FOR CONFUSING AND SIMILAR NAMES

// On and Off booleans
std::map<double, std::map<int, bool>> enabledStates;

// On and Off text states
std::map<double, std::map<int, std::string>> textStates
{
	// If you want something to be on by default...
	{1.1, {
		{2, "On"},
	}},
};

std::map<double, std::map<int, std::vector<std::string>>> toggleTexts;

// Toggle selection
std::map<double, std::map<int, int>> toggleSelection;

 #pragma endregion


std::string get_enabled_text(double pageIndex, int index)
{
	if (!textStates[pageIndex][index].empty()) {
		return textStates[pageIndex][index];
	}
	else {
		textStates[pageIndex][index] = enabledStates[pageIndex][index] == true ? "On" : "Off";
		return textStates[pageIndex][index];
	}
}


void OnToggle(bool left, bool right)
{
	double pageIndex = GetCurrentPageIndex();
	int selectedIndex = GetCurrentSelectedIndex();

	if (toggleTexts[pageIndex][selectedIndex].empty()) { // Using "On" and "Off"
		// If it doesn't exist in the hashmap, add it. Doesn't matter if it already exists.
		enabledStates[pageIndex][selectedIndex] = get_enabled_text(pageIndex, selectedIndex) == "On" ? true : false;

		// Reverse state
		enabledStates[pageIndex][selectedIndex] = !enabledStates[pageIndex][selectedIndex];

		textStates[pageIndex][selectedIndex] = enabledStates[pageIndex][selectedIndex] == true ? "On" : "Off";
		CallFunction(pageIndex, selectedIndex, enabledStates[pageIndex][selectedIndex], -1);
	} else {
		if (right) {
			toggleSelection[pageIndex][selectedIndex] += 1;
			if (toggleSelection[pageIndex][selectedIndex] > toggleTexts[pageIndex][selectedIndex].size() - 1) {
				toggleSelection[pageIndex][selectedIndex] = 0;
			}
		} else if (left) {
			toggleSelection[pageIndex][selectedIndex] -= 1;
			if (toggleSelection[pageIndex][selectedIndex] < 0) {
				toggleSelection[pageIndex][selectedIndex] = (int)toggleTexts[pageIndex][selectedIndex].size() - 1;
			}
		}
		CallFunction(pageIndex, selectedIndex, false, toggleSelection[pageIndex][selectedIndex]);
	}
}


void AddSingleOption(std::string text, int index)
{
	double pageIndex = GetCurrentPageIndex();
	bool match = false;
	// Is this optimal?
	for (std::string& element : toggleTexts[pageIndex][index]) {
		if (element == text) {
			match = true;
			break;
		}
	}
	if (!match) {
		toggleTexts[pageIndex][index].push_back(text);
	}
}


void AddMultipleOptions(std::string baseText, int index, int numberOfOptions)
{
	double pageIndex = GetCurrentPageIndex();
	if (toggleTexts[pageIndex][index].empty()) {
		for (int i = 0; i < numberOfOptions; i++) {
			toggleTexts[pageIndex][index].push_back(baseText + std::to_string(i)); // If you want text to start at 1 then add +1
		}
	}
}


std::string GetText(int index)
{
	double pageIndex = GetCurrentPageIndex();

	if (toggleTexts[pageIndex][index].empty()) {
		return get_enabled_text(pageIndex, index);
	}

	int pos = GetToggleSelectionIndex(pageIndex, index);

	if (pos >= 0 && pos <= toggleTexts[pageIndex][index].size() - 1) {
		return toggleTexts[pageIndex][index].at(pos);
	} else {
		return "INDEX OUT OF RANGE"; // Shouldn't be possible to reach, but just in case
	}
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


int GetToggleSelectionIndex(double pageIndex, int index) { return toggleSelection[pageIndex][index]; }
int GetNumOptionsInCurrentPage() { return optionsInThisPage; }
