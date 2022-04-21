// Licensed under the MIT License.

#pragma once

#include "..\common.hpp"
#include "script.h"

namespace Draw
{
	// Draw CSS styled text
	void DrawCSSText(std::string text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth = 0, int letterSpacing = 0);

	// Draws background, header, footer, and scroller textures
	void DrawMenuTextures();

	// Draws a selection box around the currently selected item
	void DrawSelectionBox();

	// GRAPHICS::DRAW_SPRITE Wrapper
	inline void DrawSprite(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, BOOL centered)
	{
		screenX /= SCREEN_WIDTH;
		screenY /= SCREEN_HEIGHT;
		width   /= SCREEN_WIDTH;
		height  /= SCREEN_HEIGHT;
		if (!centered) {
			screenX += width * 0.5f;
			screenY += height * 0.5f;
		}
		GRAPHICS::DRAW_SPRITE(textureDict, textureName, screenX, screenY, width, height, heading, red, green, blue, alpha, false);
	}
}


namespace Menu
{
	using std::string;

	extern int m_selectionIndex; // Selected option
	extern double m_pageIndex; // Current page
	extern int m_visibleOptionsInThisPage; // Sets how many options should be visible for a page. Recommended max is 12
	extern int m_numOptionsInThisPage; // Returns the number of options in the current page. DO NOT edit this variable
	extern bool m_isOpen; // Returns whether the menu is open or not. DO NOT edit this variable directly, use Menu::SetMenuEnabled
	extern bool m_isClosed; // Returns whether the menu is closed or not. DO NOT edit this variable directly, use Menu::SetMenuEnabled


	// Used for creating new menu options
	namespace New
	{
		// Create an option for a new page via pageStructureFunction parameter.
		void PageOption(string optionText, string footerText, double pageIndex, void(*pageStructureFunction)());

		// Regular plain ol' option.
		// Passed function will execute when option is pressed.
		void RegularOption(string optionText, string footerText, void(*funcToExecute)());

		// An on/off (true/false) toggle option.
		// Note: bLoopToggle parameter must be an already defined boolean variable.
		// Passed function will execute when option is pressed.
		void ToggleOption(string optionText, string footerText, bool* bLoopToggle, bool bUseCheckmark, void(*funcToExecute)());

		// An option with left and right arrows with multiple options.
		// Passed function will execute when vector option is changed.
		void VectorOption(string optionText, string footerText, std::vector<string> vectorOptions, void(*funcToExecute)());
		void VectorOption(string optionText, string footerText, int numberOfOptions, string startText, string endText, void(*funcToExecute)());
	}


	// Contains many useful functions related to options
	namespace Util
	{
		// Sets what selection a vector will be at
		void SetVectorSelection(double pageIndex, int vectorIndex, int newPos);
		// Sets the text for a vector at a certain position
		void SetTextForVectorAtPos(string newText, double pageIndex, int vectorIndex, int pos);
		// Sets an option to a new index (different spot on the page)
		void SetOptionIndex(double pageIndex, string optionText, int newIndex);

		// Gets text at current selection
		string GetTextAtCurrentSelection(bool bGetTextFromVector = false);
		int GetVectorSelection(double pageIndex, int vectorIndex);

		void ExecuteOptionFunc();
		void ExecuteVectorFunc(bool left, bool right);

		bool IsOptionAPage(double pageIndex, int index);
		bool IsOptionAVector(double pageIndex, int index);
		bool IsOptionAToggle(double pageIndex, int index);
		//bool IsIndexASeparator(double pageIndex, int index);
	}


	// Contains functions related to the menu header, sub header, and footer
	namespace Header
	{
		void SetHeader(string text, int fontSize = 45, float yPos = 79);
		void SetSubHeader(string text);
		void SetFooter(string text); // Not a header, but I don't really care
	}

	// Toggle menu on/off
	void SetMenuEnabled(bool bEnabled);
}

void RegisterUIPrompt(Prompt& prompt, Hash control, const char* promptText);
// Objective Hint
void PrintSubtitle(const char* str);
void PrintSubtitle(const std::string& str);
void PrintSubtitle(const int val);

std::unordered_map<double, void(*)()> getPageStructures();
void resetNextAssignedIndex();
