// Licensed under the MIT License.

#include "script.h"
#include "menu.h"


 #pragma region Unordered_Maps
// std::unordered_map is an optimization over std::map in this case

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


double page = 0.0; // Used for setting Menu::m_numOptionsInThisPage
std::string textAtThisSelection = "";


//--------------------------------
// Namespace for private functions
// -------------------------------
namespace menuPrivate
{
	using namespace Menu;
	int nextAssignedIndex = 0;

	void assign_index(const std::string& text)
	{
		if (!map_pageData[m_pageIndex].contains(text) && !map_pageData[m_pageIndex][text]) {
			map_pageData[m_pageIndex][text] = nextAssignedIndex;
			nextAssignedIndex += 1;
		}
	}

	void setFunction(double pageIndex, int index, void(*func)())
	{
		if (!map_optionFunctions[pageIndex][index]) {
			map_optionFunctions[pageIndex][index] = func;
		}
	}

	// wrapper function
	void drawFooter(const std::string& text, int optionIndex)
	{
		if (optionIndex == m_selectionIndex) {
			Header::SetFooter(text);
		}
	}

	void drawOption(const std::string& text, int index, bool bIsToggleOption = false, bool bIsVectorOption = false, bool bIsPageOption = false, bool bIsSeparator = false)
	{
		// Get number of options in the page via highest index
		if (m_pageIndex != page) { m_numOptionsInThisPage = 1; }
		if (index >= m_numOptionsInThisPage) { m_numOptionsInThisPage = index + 1; }
		page = m_pageIndex;

		// Only draw background if index is in range of selectionIndex
		if (m_selectionIndex <= m_visibleOptionsInThisPage-1 && index <= m_visibleOptionsInThisPage-1) {
			if (!bIsSeparator) {
				Draw::DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + (index * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
				Draw::DrawFormattedText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + (index * INCREMENT));
			}
		} else if ((index > (m_selectionIndex - m_visibleOptionsInThisPage)) && index <= m_selectionIndex) {
			if (!bIsSeparator) {
				Draw::DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + ((index - (m_selectionIndex - (m_visibleOptionsInThisPage-1))) * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
				Draw::DrawFormattedText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + ((index - (m_selectionIndex - (m_visibleOptionsInThisPage-1))) * INCREMENT));
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

		if (map_isOptionAVector[m_pageIndex].find(index) == map_isOptionAVector[m_pageIndex].end()) {
			map_isOptionAVector[m_pageIndex][index] = bIsVectorOption;
		}
		if (map_isOptionAPage[m_pageIndex].find(index) == map_isOptionAPage[m_pageIndex].end()) {
			map_isOptionAPage[m_pageIndex][index] = bIsPageOption;
		}
		if (map_isOptionAToggle[m_pageIndex].find(index) == map_isOptionAToggle[m_pageIndex].end()) {
			map_isOptionAToggle[m_pageIndex][index] = bIsToggleOption;
		}
		/*if (map_isSeparator[m_pageIndex].find(index) == map_isSeparator[m_pageIndex].end()) {
			map_isSeparator[m_pageIndex][index] = bIsSeparator;
		}*/

		if (m_selectionIndex == index) {
			textAtThisSelection = text;
		}

		if (bIsVectorOption && !bIsPageOption) {
			std::string curText = "";
			int toggleIndex = 0;

			if (!map_vectorSelectionIndex[m_pageIndex].empty()) {
				toggleIndex = map_vectorSelectionIndex[m_pageIndex][index];
			}
			if (!map_vectorTextOptions[m_pageIndex][index].empty()) {
				curText = map_vectorTextOptions[m_pageIndex][index][toggleIndex];
			}

			if (m_selectionIndex <= m_visibleOptionsInThisPage-1 && index <= m_visibleOptionsInThisPage-1) {
				if (m_selectionIndex == index) {
					// Only draw arrows if we are selecting [index]
					Draw::DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + curText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
				}
				else {
					Draw::DrawFormattedText(curText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
				}
			}
			else if ((index > (m_selectionIndex - m_visibleOptionsInThisPage)) && index <= m_selectionIndex) {
				Draw::DrawFormattedText(curText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + ((index - (m_selectionIndex - (m_visibleOptionsInThisPage-1))) * INCREMENT));
			}
		}
	}

	void addOptionsToVector(int index, std::vector<std::string> options)
	{
		if (map_isOptionAVector[Menu::m_pageIndex][index]) {
			if (map_vectorTextOptions[Menu::m_pageIndex][index].empty()) { // TODO: Instead of this, check if text already exists
				map_vectorTextOptions[Menu::m_pageIndex][index] = options;
			}
		}
	}

	void addOptionsToVector(int index, int numberOfOptions, const std::string& startText, const std::string& endText)
	{
		if (map_isOptionAVector[Menu::m_pageIndex][index]) {
			if (map_vectorTextOptions[Menu::m_pageIndex][index].empty()) { // TODO: Instead of this, check if text already exists
				for (int i = 0; i < numberOfOptions; i++) {
					// If you want text to start at 1, then add +1 to (i)
					map_vectorTextOptions[Menu::m_pageIndex][index].push_back(startText + std::to_string(i) + endText);
				}
			}
		}
	}

	// I didn't want to import a ~200KB format lib, so I just made my own dumbed down version
	template<typename... Args>
	void format(std::string& str, const Args&... args)
	{
		std::vector<std::string> vec_args = { args... };
		for (int i = 0; i < (int)vec_args.size(); i++) {
			int pos = str.find("%s");
			if (pos != std::string::npos) {
				str.replace(pos, 2, vec_args[i]);
			}
		}
	}
}


//--------------------------------
// Draw Namespace
// -------------------------------
namespace Draw
{
	void DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth, int letterSpacing)
	{
		const std::vector<std::string> fontList = { "util", "catalog5", "body1", "body", "Debug_REG", "catalog4", "chalk", "catalog1", "ledger", "title", "wantedPostersGeneric", "gtaCash", "gamername", "handwritten"};
		std::string _font = fontList[static_cast<int>(font)];

		float x = (posX / SCREEN_WIDTH);
		float y = (posY / SCREEN_HEIGHT);
		if (align == Alignment::Right) { x = 0.0f; }
		if (align == Alignment::Center) { x = -1.0f + (x * 2.0f); }

#if BUILD_1311_COMPATIBLE
		HUD::_SET_TEXT_COLOR(red, green, blue, alpha);
#else
		UIDEBUG::_BG_SET_TEXT_COLOR(red, green, blue, alpha);
#endif

		std::string _rightMargin = (align == Alignment::Right ? std::to_string((int)(SCREEN_WIDTH)-(int)(posX)) : (wrapWidth != 0 ? std::to_string((int)(SCREEN_WIDTH) - ((int)(posX) + wrapWidth)) : "0"));
		std::string _align = (align == Alignment::Left ? "Left" : align == Alignment::Right ? "Right" : "Center");
		std::string _letterSpacing = std::to_string(letterSpacing);
		std::string _size = std::to_string(textSize);

		std::string format = "<TEXTFORMAT RIGHTMARGIN = '%s'><P ALIGN = '%s'><FONT FACE = '$%s' LETTERSPACING = '%s' SIZE = '%s'>~s~%s</FONT></P><TEXTFORMAT>";
		menuPrivate::format(format, _rightMargin, _align, _font, _letterSpacing, _size, text);

#if BUILD_1311_COMPATIBLE
		HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(format.c_str()))), x, y);
#else
		UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(format.c_str()))), x, y);
#endif
	}

	void DrawMenuTextures()
	{
		// Background
		Draw::DrawSprite("generic_textures", "inkroller_1a", BG_X_OFFSET, BG_Y_OFFSET, BG_WIDTH, BG_HEIGHT, 0.0, 0, 0, 0, 230, false);
		// Header
		Draw::DrawSprite("generic_textures", "menu_header_1a", TOP_HEADER_X_POS, TOP_HEADER_Y_POS, TOP_HEADER_WIDTH, TOP_HEADER_HEIGHT, 0, 255, 255, 255, 255, false);
		// Footer
		GRAPHICS::DRAW_SPRITE("generic_textures", "menu_bar", 0.16, 0.9, 0.23, 0.001, 0, 255, 255, 255, 175, false); // Doesn't draw right with Draw::DrawSprite for some reason
		// TODO: Scroller textures
		/*
		- menu_textures
		scroller_arrow_bottom
		scroller_arrow_top
		scroller_left_bottom
		scroller_left_top
		scroller_line_down
		scroller_line_up
		scroller_right_bottom
		scroller_right_top
		*/
	}

	void DrawSelectionBox()
	{
		// TODO: Micro adjust these

		// Couldn't add the corner textures because they stretch out and just don't look right
		// Left, Right, Top, Bottom
		if (Menu::m_selectionIndex >= Menu::m_visibleOptionsInThisPage-1) {
			Draw::DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + ((Menu::m_visibleOptionsInThisPage-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + ((Menu::m_visibleOptionsInThisPage-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + ((Menu::m_visibleOptionsInThisPage-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + ((Menu::m_visibleOptionsInThisPage-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
		} else {
			Draw::DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (Menu::m_selectionIndex * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (Menu::m_selectionIndex * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (Menu::m_selectionIndex * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
			Draw::DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (Menu::m_selectionIndex * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
		}
	}
}


//--------------------------------
// Menu Namespace
// -------------------------------
namespace Menu
{
	// See menu.h for more info about this namespace and its functions

	int m_selectionIndex = 0;
	double m_pageIndex = 0.0;
	int m_visibleOptionsInThisPage = DEFAULT;
	int m_numOptionsInThisPage = 0;
	bool m_isOpen = false;
	bool m_isClosed = true;


	namespace New
	{
		
		void PageOption(const string& optionText, const string& footerText, double pageIndex, void(*pageStructureFunction)())
		{
			/*
			double _pageIndex = 0.0;
			if (m_pageIndex == 0.0) {
				_pageIndex = m_selectionIndex + 1.0;
			} else {
				// idk WHY this doesnt work, but it should. might be how i assign the func?
				// Trying to automate page index...
				_pageIndex = m_pageIndex + ((m_selectionIndex + 1.0) / 10);
			}
			*/
			
			if (!map_pageStructures.contains(pageIndex)) {
				map_pageStructures[pageIndex] = pageStructureFunction;
			}

			menuPrivate::assign_index(optionText);
			int index = map_pageData[m_pageIndex][optionText];
			menuPrivate::drawOption(optionText, index, false, false, true);
			menuPrivate::drawFooter(footerText, index);
		}

		void RegularOption(const string& optionText, const string& footerText, void(*funcToExecute)())
		{
			menuPrivate::assign_index(optionText);
			int index = map_pageData[m_pageIndex][optionText];
			menuPrivate::drawOption(optionText, index);
			menuPrivate::setFunction(m_pageIndex, index, funcToExecute);
			menuPrivate::drawFooter(footerText, index);
		}

		void ToggleOption(const string& optionText, const string& footerText, bool* bLoopToggle, bool bUseCheckmark, void(*funcToExecute)())
		{
			menuPrivate::assign_index(optionText);
			int index = map_pageData[m_pageIndex][optionText];
			menuPrivate::drawOption(optionText, index, true);
			menuPrivate::setFunction(m_pageIndex, index, funcToExecute);
			menuPrivate::drawFooter(footerText, index);

			if (PAD::IS_CONTROL_JUST_RELEASED(0, INPUT_GAME_MENU_ACCEPT) && m_selectionIndex == index) {
				// I decided to just do this here because it's just easier...
				*bLoopToggle = !*bLoopToggle;
			}

			if (bUseCheckmark && *bLoopToggle) {
				if (m_selectionIndex <= m_visibleOptionsInThisPage-1 && index <= m_visibleOptionsInThisPage-1) {
					Draw::DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 250 + (index * INCREMENT));
				}
				else if ((index > (m_selectionIndex - m_visibleOptionsInThisPage)) && index <= m_selectionIndex) {
					Draw::DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 250 + ((index - (m_selectionIndex - (m_visibleOptionsInThisPage-1))) * INCREMENT));
				}
			}
		}

		void VectorOption(const string& optionText, const string& footerText, std::vector<string> vectorOptions, void(*funcToExecute)())
		{
			menuPrivate::assign_index(optionText);
			int index = map_pageData[m_pageIndex][optionText];
			menuPrivate::drawOption(optionText, index, false, true);
			menuPrivate::setFunction(m_pageIndex, index, funcToExecute);
			menuPrivate::addOptionsToVector(index, vectorOptions);
			menuPrivate::drawFooter(footerText, index);
		}

		void VectorOption(const string& optionText, const string& footerText, int numberOfOptions, const string& startText, const string& endText, void(*funcToExecute)())
		{
			menuPrivate::assign_index(optionText);
			int index = map_pageData[m_pageIndex][optionText];
			menuPrivate::drawOption(optionText, index, false, true);
			menuPrivate::setFunction(m_pageIndex, index, funcToExecute);
			menuPrivate::addOptionsToVector(index, numberOfOptions, startText, endText);
			menuPrivate::drawFooter(footerText, index);
		}
	}


	namespace Util
	{
		void SetVectorSelection(double pageIndex, int vectorIndex, int newPos)
		{
			if (IsOptionAVector(pageIndex, vectorIndex)) {
				map_vectorSelectionIndex[pageIndex][vectorIndex] = newPos;
			}
		}

		void SetTextForVectorAtPos(const string& newText, double pageIndex, int vectorIndex, int pos)
		{
			if (IsOptionAVector(pageIndex, vectorIndex)) {
				map_vectorTextOptions[pageIndex][vectorIndex][pos] = newText;
			}
		}

		void SetOptionIndex(double pageIndex, const string& optionText, int newIndex)
		{
			if (map_pageData[pageIndex].contains(optionText)) {
				map_pageData[pageIndex][optionText] = newIndex;
			}
		}

		void SetOptionIndex(double pageIndex, int optionIndex, int newIndex)
		{
			for (auto it = map_pageData[pageIndex].begin(); it != map_pageData[pageIndex].end(); it++) {
				if (it->second == optionIndex) {
					map_pageData[pageIndex][it->first] = newIndex;
					break;
				}
			}
		}

		string GetTextAtCurrentSelection(bool bGetTextFromVector)
		{
			if (bGetTextFromVector) {
				int pos = GetVectorSelection(m_pageIndex, m_selectionIndex);
				return map_vectorTextOptions[m_pageIndex][m_selectionIndex][pos];
			}

			return textAtThisSelection;
		}

		int GetVectorSelection(double pageIndex, int vectorIndex)
		{
			if (IsOptionAVector(pageIndex, vectorIndex)) {
				return map_vectorSelectionIndex[pageIndex][vectorIndex];
			}

			return 0; // Always return 0 because option isn't a vector
		}

		void ExecuteOptionFunc()
		{
			// Enter has been pressed
			if (map_optionFunctions[m_pageIndex][m_selectionIndex]) {
				map_optionFunctions[m_pageIndex][m_selectionIndex]();
			} else {
#if PRINT_FUNC_NOT_FOUND
				PrintSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(m_pageIndex).substr(0,4) + " - " + std::to_string(m_selectionIndex) + ")~s~");
#endif // PRINT_FUNC_NOT_FOUND
			}
		}

		void ExecuteVectorFunc(bool left, bool right)
		{
			if (left) {
				map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] -= 1;
				if (map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] < 0) {
					map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] = (int)map_vectorTextOptions[m_pageIndex][m_selectionIndex].size() - 1;
				}
			} else if (right) {
				map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] += 1;
				if (map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] > map_vectorTextOptions[m_pageIndex][m_selectionIndex].size() - 1) {
					map_vectorSelectionIndex[m_pageIndex][m_selectionIndex] = 0;
				}
			}

			// Vector has been changed
			if (map_optionFunctions[m_pageIndex][m_selectionIndex]) {
				map_optionFunctions[m_pageIndex][m_selectionIndex]();
			} else {
#if PRINT_FUNC_NOT_FOUND
				PrintSubtitle("~COLOR_RED~No function was assigned at this index (" + std::to_string(m_pageIndex).substr(0,4) + " - " + std::to_string(m_selectionIndex) + ")~s~");
#endif // PRINT_FUNC_NOT_FOUND
			}
		}

		bool IsOptionAPage(double pageIndex, int index)
		{
			if (map_isOptionAPage[pageIndex].find(index) != map_isOptionAPage[pageIndex].end()) {
				return map_isOptionAPage[pageIndex][index];
			}

			return false;
		}

		bool IsOptionAVector(double pageIndex, int index)
		{
			if (map_isOptionAVector[pageIndex].find(index) != map_isOptionAVector[pageIndex].end()) {
				return map_isOptionAVector[pageIndex][index];
			}

			return false;
		}

		bool IsOptionAToggle(double pageIndex, int index)
		{
			if (map_isOptionAToggle[pageIndex].find(index) != map_isOptionAToggle[pageIndex].end()) {
				return map_isOptionAToggle[pageIndex][index];
			}

			return false;
		}

		bool IsIndexASeparator(double pageIndex, int index)
		{
			return map_isSeparator[pageIndex][index];
		}
	}


	namespace Header
	{
		void SetHeader(const std::string& text, int fontSize, float yPos) {
			Draw::DrawFormattedText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, fontSize, BG_X_OFFSET + (BG_WIDTH * 0.5f), yPos);
		}

		void SetSubHeader(const std::string& text) {
			Draw::DrawFormattedText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 23, BG_X_OFFSET + (BG_WIDTH * 0.5f), 172);
		}

		void SetFooter(const std::string& text) {
			Draw::DrawFormattedText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 19, BG_X_OFFSET + (BG_WIDTH * 0.5f), 980);
		}
	}


	void SetMenuEnabled(bool bEnabled)
	{
		m_isOpen = bEnabled;
		m_isClosed = !bEnabled;

		if (bEnabled) {
			AUDIO::_STOP_SOUND_WITH_NAME("MENU_ENTER", "HUD_PLAYER_MENU");
			AUDIO::PLAY_SOUND_FRONTEND("MENU_ENTER", "HUD_PLAYER_MENU", true, 0);
			MAP::DISPLAY_RADAR(false);
		} else {
			AUDIO::_STOP_SOUND_WITH_NAME("MENU_CLOSE", "HUD_PLAYER_MENU");
			AUDIO::PLAY_SOUND_FRONTEND("MENU_CLOSE", "HUD_PLAYER_MENU", true, 0);
			MAP::DISPLAY_RADAR(true);
		}
	}
}


//--------------------------------
// No Namespace
// -------------------------------

void RegisterUIPrompt(Prompt& prompt, Hash control, const char* promptText)
{
	prompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
	HUD::_UI_PROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UI_PROMPT_SET_PRIORITY(prompt, 2); // PP_HIGH
	HUD::_UI_PROMPT_SET_TEXT(prompt, promptText);
	HUD::_UI_PROMPT_SET_STANDARD_MODE(prompt, TRUE);
	HUD::_UI_PROMPT_REGISTER_END(prompt);

	HUD::_UI_PROMPT_SET_VISIBLE(prompt, false);
	HUD::_UI_PROMPT_SET_ENABLED(prompt, false);
}

void PrintSubtitle(const char* str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str);
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
void PrintSubtitle(const std::string& str) { PrintSubtitle(str.c_str()); }
void PrintSubtitle(const int val) { PrintSubtitle(std::to_string(val).c_str()); }

std::unordered_map<double, void(*)()> getPageStructures() { return map_pageStructures; }
void resetNextAssignedIndex() { menuPrivate::nextAssignedIndex = 0; }
