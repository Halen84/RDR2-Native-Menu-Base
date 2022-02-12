// Licensed under the MIT License.

#include "script.h"
#include "menu.h"
#include <string>
#include <vector>
#include <unordered_map>

 #pragma region Unordered Maps
// std::unordered_map is a micro optimization over std::map in this case

// Keeps track of text options for each vector
std::unordered_map<double, std::unordered_map<int, std::vector<std::string>>> map_vectorTextOptions;

// Keeps track of what selection you are on for vector options
std::unordered_map<double, std::unordered_map<int, int>> map_vectorSelectionIndex;

std::unordered_map<double, std::unordered_map<int, bool>> map_isOptionAPage;
std::unordered_map<double, std::unordered_map<int, bool>> map_isOptionAToggle;
std::unordered_map<double, std::unordered_map<int, bool>> map_isOptionAVector;
std::unordered_map<double, std::unordered_map<int, bool>> map_isSeparator;

// Holds functions that will be called for an option
std::unordered_map<double, std::unordered_map<int, void(*)()>> map_optionFunctions;

// Hold functions for drawn pages
std::unordered_map<double, void(*)()> map_pageStructures;

// Contains values for what's in a page and where
std::unordered_map<double, std::unordered_map<std::string, int>> map_pageData;

#pragma endregion

int optionsInThisPage = 0;
int visibleOptions = 8; // How many options are visible at a time. Recommended max is 12.
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


// Not a public function anymore
void drawOption(std::string text, int index, bool bIsToggleOption = false, bool bIsVectorOption = false, bool bIsPageOption = false, bool bIsSeparator = false)
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	// Get number of options in the page via highest index
	if (pageIndex != page) { optionsInThisPage = 1; }
	if (index >= optionsInThisPage) { optionsInThisPage = index + 1; }
	page = pageIndex;

	// Only draw background if index is in range of selectedIndex
	if (selectedIndex <= visibleOptions-1 && index <= visibleOptions-1) {
		if (!bIsSeparator) {
			Draw::DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + (index * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
			Draw::DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + (index * INCREMENT));
		}
	} else if ((index > (selectedIndex - visibleOptions)) && index <= selectedIndex) {
		if (!bIsSeparator) {
			Draw::DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + ((index - (selectedIndex - (visibleOptions-1))) * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
			Draw::DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + ((index - (selectedIndex - (visibleOptions-1))) * INCREMENT));
		}	
	}

	// bIsToggleOption and bIsVectorOption can't contain a page, default to false
	if (bIsPageOption && (bIsToggleOption || bIsVectorOption)) {
		bIsPageOption = false;
	}
	// bIsSeparator can not contain anything, default all to false
	if (bIsSeparator && (bIsToggleOption || bIsVectorOption || bIsPageOption)) {
		bIsToggleOption = false; bIsVectorOption = false; bIsPageOption = false;
	}

	if (map_isOptionAVector[pageIndex].find(index) == map_isOptionAVector[pageIndex].end()) {
		map_isOptionAVector[pageIndex][index] = bIsVectorOption;
	}
	if (map_isOptionAPage[pageIndex].find(index) == map_isOptionAPage[pageIndex].end()) {
		map_isOptionAPage[pageIndex][index] = bIsPageOption;
	}
	if (map_isOptionAToggle[pageIndex].find(index) == map_isOptionAToggle[pageIndex].end()) {
		map_isOptionAToggle[pageIndex][index] = bIsToggleOption;
	}
	if (map_isSeparator[pageIndex].find(index) == map_isSeparator[pageIndex].end()) {
		map_isSeparator[pageIndex][index] = bIsSeparator;
	}

	if (selectedIndex == index) {
		textAtThisSelection = text;
	}

	if (bIsVectorOption && !bIsPageOption) {
		std::string curText = "";
		int toggleIndex = 0;

		if (!map_vectorSelectionIndex[pageIndex].empty()) {
			toggleIndex = map_vectorSelectionIndex[pageIndex][index];
		}
		if (!map_vectorTextOptions[pageIndex][index].empty()) {
			curText = map_vectorTextOptions[pageIndex][index][toggleIndex];
		}

		if (selectedIndex <= visibleOptions-1 && index <= visibleOptions-1) {
			if (selectedIndex == index) {
				// Only draw arrows if we are selecting [index]
				Draw::DrawCSSText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + curText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
			}
			else {
				Draw::DrawCSSText(curText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
			}
		}
		else if ((index > (selectedIndex - visibleOptions)) && index <= selectedIndex) {
			Draw::DrawCSSText(curText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + ((index - (selectedIndex - (visibleOptions-1))) * INCREMENT));
		}
	}
}


void Draw::DrawSelectionBox()
{
	// TODO: Micro adjust these

	int index = *GetCurrentSelectedIndex();

	// Left, Right, Top, Bottom
	if (index >= visibleOptions-1) {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + ((visibleOptions-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + ((visibleOptions-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + ((visibleOptions-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + ((visibleOptions-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
	} else {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
	}
}


// Just a wrapper function. Not public
void drawFooter(std::string text, int optionIndex)
{
	if (optionIndex == *GetCurrentSelectedIndex()) {
		Header::SetFooter(text);
	}
}


////////////////////
// MENU NAMESPACE //
//////////////////// 


void Menu::SetVectorSelection(double pageIndex, int vectorIndex, int newPos)
{
	if (IsOptionAVector(pageIndex, vectorIndex)) {
		map_vectorSelectionIndex[pageIndex][vectorIndex] = newPos;
	}
}


void Menu::SetTextForVectorAtPos(std::string newText, double pageIndex, int vectorIndex, int pos)
{
	if (IsOptionAVector(pageIndex, vectorIndex)) {
		map_vectorTextOptions[pageIndex][vectorIndex][pos] = newText;
	}
}


// Not a public function anymore
void setFunction(double pageIndex, int index, void(*func)())
{
	if (!map_optionFunctions[pageIndex][index]) {
		map_optionFunctions[pageIndex][index] = func;
	}
}


std::string Menu::GetTextAtCurrentSelection(bool bGetTextFromVector)
{
	double pageIndex = *GetCurrentPageIndex();
	int index = *GetCurrentSelectedIndex();

	if (bGetTextFromVector) {
		int pos = Menu::GetVectorSelection(pageIndex, index);
		return map_vectorTextOptions[pageIndex][index][pos];
	}

	return textAtThisSelection;
}


int Menu::GetVectorSelection(double pageIndex, int vectorIndex)
{
	if (IsOptionAVector(pageIndex, vectorIndex)) {
		return map_vectorSelectionIndex[pageIndex][vectorIndex];
	}

	return 0; // Always return 0 because option isn't a vector
}


void Menu::OnSelect()
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	// Enter has been pressed
	if (map_optionFunctions[pageIndex][selectedIndex]) {
		map_optionFunctions[pageIndex][selectedIndex]();
	} else {
		PrintSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(pageIndex).substr(0,4) + " - " + std::to_string(selectedIndex) + ")~s~");
	}
}


void Menu::OnVectorSwitch(bool left, bool right)
{
	double pageIndex = *GetCurrentPageIndex();
	int selectedIndex = *GetCurrentSelectedIndex();

	if (left) {
		map_vectorSelectionIndex[pageIndex][selectedIndex] -= 1;
		if (map_vectorSelectionIndex[pageIndex][selectedIndex] < 0) {
			map_vectorSelectionIndex[pageIndex][selectedIndex] = (int)map_vectorTextOptions[pageIndex][selectedIndex].size() - 1;
		}
	} else if (right) {
		map_vectorSelectionIndex[pageIndex][selectedIndex] += 1;
		if (map_vectorSelectionIndex[pageIndex][selectedIndex] > map_vectorTextOptions[pageIndex][selectedIndex].size() - 1) {
			map_vectorSelectionIndex[pageIndex][selectedIndex] = 0;
		}
	}

	// Toggle has been changed
	if (map_optionFunctions[pageIndex][selectedIndex]) {
		map_optionFunctions[pageIndex][selectedIndex]();
	} else {
		PrintSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(pageIndex).substr(0,4) + " - " + std::to_string(selectedIndex) + ")~s~");
	}
}


bool Menu::IsOptionAPage(double pageIndex, int index)
{
	if (map_isOptionAPage[pageIndex].find(index) != map_isOptionAPage[pageIndex].end()) {
		return map_isOptionAPage[pageIndex][index];
	}

	return false;
}


bool Menu::IsOptionAVector(double pageIndex, int index)
{
	if (map_isOptionAVector[pageIndex].find(index) != map_isOptionAVector[pageIndex].end()) {
		return map_isOptionAVector[pageIndex][index];
	}

	return false;
}


bool Menu::IsOptionAToggle(double pageIndex, int index)
{
	if (map_isOptionAToggle[pageIndex].find(index) != map_isOptionAToggle[pageIndex].end()) {
		return map_isOptionAToggle[pageIndex][index];
	}

	return false;
}


bool Menu::IsIndexASeparator(double pageIndex, int index)
{
	return map_isSeparator[pageIndex][index];
}


int Menu::GetNumOptionsInCurrentPage() { return optionsInThisPage; }
int Menu::GetMaxVisibleOptions() { return visibleOptions; }
void Menu::SetMaxVisibleOptions(int num) { visibleOptions = num; }


// Not a public function anymore
void addOptionsToVector(int index, std::vector<std::string> options)
{
	double pageIndex = *GetCurrentPageIndex();
	if (map_isOptionAVector[pageIndex][index]) {
		if (map_vectorTextOptions[pageIndex][index].empty()) { // TODO: Instead of this, check if text already exists
			map_vectorTextOptions[pageIndex][index] = options;
		}
	}
}


// Not a public function anymore
void addOptionsToVector(int index, int numberOfOptions, std::string startText, std::string endText)
{
	double pageIndex = *GetCurrentPageIndex();
	if (map_isOptionAVector[pageIndex][index]) {
		if (map_vectorTextOptions[pageIndex][index].empty()) { // TODO: Instead of this, check if text already exists
			for (int i = 0; i < numberOfOptions; i++) {
				// If you want text to start at 1, then add +1 to (i)
				map_vectorTextOptions[pageIndex][index].push_back(startText + std::to_string(i) + endText);
			}
		}
	}
}


//////////////////////
// HEADER NAMESPACE //
//////////////////////


void Header::SetHeader(std::string text, int fontSize, float yPos) {
	Draw::DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, fontSize, BG_X_OFFSET + (BG_WIDTH * 0.5f), yPos);
}


void Header::SetSubHeader(std::string text) {
	Draw::DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 23, BG_X_OFFSET + (BG_WIDTH * 0.5f), 172);
}


void Header::SetFooter(std::string text) {
	Draw::DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 19, BG_X_OFFSET + (BG_WIDTH * 0.5f), 980);
}


///////////////////////////
// MENU CREATE NAMESPACE //
///////////////////////////


int nextAssignedIndex = 0;

// Not a public function
void assign_index(std::string text)
{
	if (text == "__RESERVED" || !map_pageData[*GetCurrentPageIndex()].contains(text) && !map_pageData[*GetCurrentPageIndex()][text]) {
		map_pageData[*GetCurrentPageIndex()][text] = nextAssignedIndex;
		nextAssignedIndex += 1;
	}
}

std::unordered_map<double, void(*)()> getPageStructures() { return map_pageStructures; }
void resetNextAssignedIndex() { nextAssignedIndex = 0; }

namespace MenuCreate
{
	void PageOption(std::string optionText, std::string optionFooter, double pageIndex, void(*pageStructureFunction)())
	{
		// TODO: AUTOMATE pageIndex!!!!
		// I already tried to automate this, but I got pissed off because I couldn't figure it out. Sorry

		if (!map_pageStructures[pageIndex]) {
			map_pageStructures[pageIndex] = pageStructureFunction;
		}
		

		assign_index(optionText);
		int index = map_pageData[*GetCurrentPageIndex()][optionText];
		drawOption(optionText, index, false, false, true);
		drawFooter(optionFooter, index);
	}

	void RegularOption(std::string optionText, std::string optionFooter, void(*funcToExecute)())
	{
		assign_index(optionText);
		int index = map_pageData[*GetCurrentPageIndex()][optionText];
		drawOption(optionText, index);
		setFunction(*GetCurrentPageIndex(), index, funcToExecute);
		drawFooter(optionFooter, index);
	}
	
	void ToggleOption(std::string optionText, std::string optionFooter, bool* bToggle, bool bUseCheckmark, void(*funcToExecute)())
	{
		assign_index(optionText);
		int index = map_pageData[*GetCurrentPageIndex()][optionText];
		drawOption(optionText, index, true);
		setFunction(*GetCurrentPageIndex(), index, funcToExecute);
		drawFooter(optionFooter, index);

		int selectedIndex = *GetCurrentSelectedIndex();
		if (PAD::IS_CONTROL_JUST_RELEASED(0, MISC::GET_HASH_KEY("INPUT_GAME_MENU_ACCEPT")) && selectedIndex == index) {
			// I decided to just do this here because it's just easier...
			*bToggle = !*bToggle;
		}
		
		if (bUseCheckmark && *bToggle) {
			if (selectedIndex <= visibleOptions-1 && index <= visibleOptions-1) {
				Draw::DrawCSSText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 250 + (index * INCREMENT));
			}
			else if ((index > (selectedIndex - visibleOptions)) && index <= selectedIndex) {
				Draw::DrawCSSText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 250 + ((index - (selectedIndex - (visibleOptions-1))) * INCREMENT));
			}
		}
	}

	void VectorOption(std::string optionText, std::string optionFooter, std::vector<std::string> vectorOptions, void(*funcToExecute)())
	{
		assign_index(optionText);
		int index = map_pageData[*GetCurrentPageIndex()][optionText];
		drawOption(optionText, index, false, true);
		setFunction(*GetCurrentPageIndex(), index, funcToExecute);
		addOptionsToVector(index, vectorOptions);
		drawFooter(optionFooter, index);
	}

	void VectorOption(std::string optionText, std::string optionFooter, int numberOfOptions, std::string startText, std::string endText, void(*funcToExecute)())
	{
		assign_index(optionText);
		int index = map_pageData[*GetCurrentPageIndex()][optionText];
		drawOption(optionText, index, false, true);
		setFunction(*GetCurrentPageIndex(), index, funcToExecute);
		addOptionsToVector(index, numberOfOptions, startText, endText);
		drawFooter(optionFooter, index);
	}

	// TODO: Make option counter ignore this
	void Separator()
	{
		assign_index("__RESERVED");
		int index = map_pageData[*GetCurrentPageIndex()]["__RESERVED"];
		drawOption("__RESERVED", index, false, false, false, true);
	}
}


// NO NAMESPACE


void RegisterUIPrompt(Prompt& prompt, Hash control, const char* promptText)
{
	prompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
	HUD::_UI_PROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UI_PROMPT_SET_PRIORITY(prompt, 2); // PP_HIGH
	HUD::_UI_PROMPT_SET_TEXT(prompt, promptText);
	HUD::_UI_PROMPT_SET_STANDARD_MODE(prompt, 1);
	HUD::_UI_PROMPT_REGISTER_END(prompt);

	HUD::_UI_PROMPT_SET_VISIBLE(prompt, false);
	HUD::_UI_PROMPT_SET_ENABLED(prompt, false);
}

void PrintSubtitle(const std::string& str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
