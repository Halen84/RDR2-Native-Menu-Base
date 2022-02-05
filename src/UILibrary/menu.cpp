// Licensed under the MIT License.

#include <string>
#include "script.h"
#include "menu.h"
#include <vector>
#include <map>

 #pragma region Hash Maps

// Keeps track of text options for each toggle
std::map<double, std::map<int, std::vector<std::string>>> toggleTextOptions;

// Keeps track of what selection you are on for toggle options
std::map<double, std::map<int, int>> toggleSelectionIndex;

// Holds values for whether an option has a page
std::map<double, std::map<int, bool>> map_doesOptionHavePage;

// Holds values for whether an option has a toggle
std::map<double, std::map<int, bool>> map_doesOptionHaveToggle;

// Holds values for what function will be called for an option
std::map<double, std::map<int, void(*)()>> map_optionFunctions;

#pragma endregion

int optionsInThisPage = 0;
double page = 0.0;
std::string textAtThisSelection = "";


////////////////////
// DRAW NAMESPACE //
//////////////////// 


void Draw::DrawCSSText(std::string text, Font font, int R, int G, int B, int A, Alignment align, int textSize, float X, float Y, int wrapWidth, int letterSpacing)
{
	std::vector<std::string> fontList = { "util", "catalog5", "body1", "body", "Debug_REG", "catalog4", "chalk", "catalog1", "ledger", "title", "wantedPostersGeneric", "gtaCash", "gamername", "handwritten"};
	std::string _font = fontList[static_cast<int>(font)];

	float x = (X / SCREEN_WIDTH);
	float y = (Y / SCREEN_HEIGHT);
	if (align == Alignment::Right) { x = 0.0f; }
	if (align == Alignment::Center) { x = -1.0f + (x * 2.0f); }
	
	UIDEBUG::_BG_SET_TEXT_COLOR(R, G, B, A);

	// Use this native instead of UIDEBUG:: if you want your menu to be compatible with game version <= 1311.12
	//HUD::_SET_TEXT_COLOR(R, G, B, A);

	std::string cap = "<TEXTFORMAT RIGHTMARGIN ='" + (align == Alignment::Right ? std::to_string(static_cast<int>(SCREEN_WIDTH) - static_cast<int>(X)) : (wrapWidth != 0 ? std::to_string(static_cast<int>(SCREEN_WIDTH) - (static_cast<int>(X) + wrapWidth)) : "0")) 
		+ "'><P ALIGN='" + (align == Alignment::Left ? "left" : align == Alignment::Right ? "right" : "center") +  "'><font face='$" + _font + "' letterspacing ='" + std::to_string(letterSpacing) + "' size='" + std::to_string(textSize) + "'>~s~" + text + "</font></P><TEXTFORMAT>";

	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(cap.c_str()))), x, y);

	// Use this native instead of UIDEBUG:: if you want your menu to be compatible with game version <= 1311.12
	//HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(cap.c_str()))), x, y);
}


void Draw::DrawOption(std::string text, int index, bool bHasPage, bool bIsToggleOption)
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	// Get number of options in the page via highest index
	if (pageIndex != page) { optionsInThisPage = 1; }
	if (index >= optionsInThisPage) { optionsInThisPage = index + 1; }
	page = pageIndex;


	// Only draw background if index is in range of selectedIndex
	if (selectedIndex <= 7 && index <= 7) {
		DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + (index * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 32, 32, 32, 200, true);
		DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + (index * INCREMENT));
	} else if ((index > (selectedIndex - 8)) && index <= selectedIndex) {
		DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + ((index - (selectedIndex - 7)) * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 32, 32, 32, 200, true);
		DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + ((index - (selectedIndex - 7)) * INCREMENT));
	}


	if (map_doesOptionHaveToggle[pageIndex].find(index) == map_doesOptionHaveToggle[pageIndex].end()) {
		map_doesOptionHaveToggle[pageIndex][index] = bIsToggleOption;
	}
	if (map_doesOptionHavePage[pageIndex].find(index) == map_doesOptionHavePage[pageIndex].end()) {
		map_doesOptionHavePage[pageIndex][index] = bHasPage;
	}

	if (selectedIndex == index) {
		textAtThisSelection = text;
	}

	// bIsToggleOption and bHasPage cannot both be true. Nothing will show up.
	if (bIsToggleOption && !bHasPage) {
		std::string curText = "";
		int toggleIndex = 0;

		// Make sure it's not empty, then set values accordingly
		if (!toggleSelectionIndex[pageIndex].empty()) {
			toggleIndex = toggleSelectionIndex[pageIndex][index];
		}
		if (!toggleTextOptions[pageIndex][index].empty()) {
			curText = toggleTextOptions[pageIndex][index][toggleIndex];
		}
		
		// Draw arrows if we are selecting [index]
		if (selectedIndex == index) {
			DrawCSSText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + curText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
		} else {
			DrawCSSText(curText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
		}
	}
}


void Draw::DrawSelectionBox()
{
	// TODO: Make sure next or previous index exists. If not, skip over it.
	// TODO: Micro adjust these and fix it up a little

	int index = *GetCurrentSelectedIndex();

	// Left, Right, Top, Bottom
	if (index >= 7) {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (7 * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (7 * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (7 * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (7 * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
	} else {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
	}
}


////////////////////
// MENU NAMESPACE //
//////////////////// 


void Menu::AddOptionsToToggle(int index, std::vector<std::string> options)
{
	// TODO: Make it so you can add more options without this call.
	// (you can if you manually add it via hashmap)

	double pageIndex = *GetCurrentPageIndex();
	if (DoesOptionHaveToggle(pageIndex, index)) {
		if (toggleTextOptions[pageIndex][index].empty()) {
			toggleTextOptions[pageIndex][index] = options;
		}
	}
}


void Menu::AddOptionsToToggle(int index, int numberOfOptions, std::string baseText)
{
	double pageIndex = *GetCurrentPageIndex();
	if (DoesOptionHaveToggle(pageIndex, index)) {
		if (toggleTextOptions[pageIndex][index].empty()) {
			for (int i = 0; i < numberOfOptions; i++) {
				// If you want text to start at 1, then add +1 to (i)
				toggleTextOptions[pageIndex][index].push_back(baseText + std::to_string(i));
			}
		}
	}
}


void Menu::SetToggleSelection(double pageIndex, int toggleIndex, int newPos)
{
	if (DoesOptionHaveToggle(pageIndex, toggleIndex)) {
		toggleSelectionIndex[pageIndex][toggleIndex] = newPos;
	}
}


void Menu::SetTextAtPos(std::string newText, double pageIndex, int toggleIndex, int pos)
{
	if (DoesOptionHaveToggle(pageIndex, toggleIndex)) {
		toggleTextOptions[pageIndex][toggleIndex][pos] = newText;
	}
}


// TODO: template<typename... Args>
void Menu::SetFunction(double pageIndex, int index, void(*func)(/*Args... args*/), int numberOfOptionsThatUseThisFunction)
{
	if (!map_optionFunctions[pageIndex][index]) {
		map_optionFunctions[pageIndex][index] = func;
		if (numberOfOptionsThatUseThisFunction > 1) {
			for (int i = 0; i < numberOfOptionsThatUseThisFunction + 1; i++) {
				map_optionFunctions[pageIndex][index + i] = func;
			}
		}
	}
}


std::string Menu::GetTextAtCurrentSelection()
{
	return textAtThisSelection;
}


int Menu::GetToggleSelection(double pageIndex, int toggleIndex)
{
	if (DoesOptionHaveToggle(pageIndex, toggleIndex)) {
		return toggleSelectionIndex[pageIndex][toggleIndex];
	}

	return 0; // Always return 0 because toggle doesn't exist
}


void Menu::OnSelect()
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	// Enter has been pressed
	if (map_optionFunctions[pageIndex][selectedIndex]) {
		map_optionFunctions[pageIndex][selectedIndex]();
	} else {
		ShowSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(pageIndex).substr(0,4) + " - " + std::to_string(selectedIndex) + ")~s~");
	}
}


void Menu::OnToggle(bool left, bool right)
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	if (left) {
		toggleSelectionIndex[pageIndex][selectedIndex] -= 1;
		if (toggleSelectionIndex[pageIndex][selectedIndex] < 0) {
			toggleSelectionIndex[pageIndex][selectedIndex] = (int)toggleTextOptions[pageIndex][selectedIndex].size() - 1;
		}
	} else if (right) {
		toggleSelectionIndex[pageIndex][selectedIndex] += 1;
		if (toggleSelectionIndex[pageIndex][selectedIndex] > toggleTextOptions[pageIndex][selectedIndex].size() - 1) {
			toggleSelectionIndex[pageIndex][selectedIndex] = 0;
		}
	}

	// Toggle has been changed
	if (map_optionFunctions[pageIndex][selectedIndex]) {
		map_optionFunctions[pageIndex][selectedIndex]();
	} else {
		ShowSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(pageIndex).substr(0,4) + " - " + std::to_string(selectedIndex) + ")~s~");
	}
}


bool Menu::DoesOptionHavePage(double pageIndex, int index)
{
	if (map_doesOptionHavePage[pageIndex].find(index) != map_doesOptionHavePage[pageIndex].end()) {
		return map_doesOptionHavePage[pageIndex][index];
	}

	return false;
}


bool Menu::DoesOptionHaveToggle(double pageIndex, int index)
{
	if (map_doesOptionHaveToggle[pageIndex].find(index) != map_doesOptionHaveToggle[pageIndex].end()) {
		return map_doesOptionHaveToggle[pageIndex][index];
	}

	return false;
}


int Menu::GetNumOptionsInCurrentPage() { return optionsInThisPage; }


//////////////////////
// HEADER NAMESPACE //
//////////////////////


void Header::SetHeader(std::string text, int fontSize, float yPos) {
	Draw::DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, fontSize,  BG_X_OFFSET + (BG_WIDTH * 0.5f), yPos);
}


void Header::SetSubHeader(std::string text) {
	Draw::DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 23, BG_X_OFFSET + (BG_WIDTH * 0.5f), 172);
}


void Header::SetFooter(std::string text) {
	Draw::DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 20, BG_X_OFFSET + (BG_WIDTH * 0.5f), 975);
}


// NO NAMESPACE

void CreateUIPrompt(Prompt& prompt, Hash control, const char* promptText)
{
	prompt = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_0xF4A5C4509BF923B1(prompt, 0);
	HUD::_UIPROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UIPROMPT_SET_TEXT(prompt, promptText);
	HUD::_UIPROMPT_SET_STANDARD_MODE(prompt, 1);
	HUD::_UIPROMPT_REGISTER_END(prompt);

	HUD::_UIPROMPT_SET_VISIBLE(prompt, false);
	HUD::_UIPROMPT_SET_ENABLED(prompt, false);
}

void ShowSubtitle(const std::string& str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
