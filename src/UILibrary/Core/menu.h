// Licensed under the MIT License.

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "script.h"

namespace Draw
{
	// Draw CSS styled text
	void DrawCSSText(std::string text, Font font, int R, int G, int B, int A, Alignment align, int textSize, float X, float Y, int wrapWidth = 0, int letterSpacing = 0);

	// Draws a selection box around currently selected item
	void DrawSelectionBox();

	// GRAPHICS::DRAW_SPRITE wrapper
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

namespace Header
{
	void SetHeader(std::string text, int fontSize = 45, float yPos = 79);
	void SetSubHeader(std::string text);
	void SetFooter(std::string text); // Not a header, but I don't really care
}

namespace Menu
{
	void SetVectorSelection(double pageIndex, int vectorIndex, int newPos);
	void SetTextForVectorAtPos(std::string newText, double pageIndex, int vectorIndex, int pos);

	// Set the functon that will be called when this option is pressed/toggled
	//void SetFunction(double pageIndex, int index, void(*func)(), int numberOfOptionsThatUseThisFunction = 1);

	// Get text at current selected index
	std::string GetTextAtCurrentSelection(bool bGetTextFromVector = false);

	int GetVectorSelection(double pageIndex, int vectorIndex);

	void OnSelect();
	void OnVectorSwitch(bool left, bool right);

	bool IsOptionAPage(double pageIndex, int index);
	bool IsOptionAVector(double pageIndex, int index);
	bool IsOptionAToggle(double pageIndex, int index);
	bool IsIndexASeparator(double pageIndex, int index);

	int GetNumOptionsInCurrentPage();
	int GetMaxVisibleOptions();
	void SetMaxVisibleOptions(int num);
}

// Creating Options
namespace MenuCreate
{
	// When pressed, this option will draw a new page via pageStructureFunction parameter.
	void PageOption(std::string optionText, std::string optionFooter, double pageIndex, void(*pageStructureFunction)());

	// Regular plain ol' option.
	// Passed function will execute when option is pressed.
	void RegularOption(std::string optionText, std::string optionFooter, void(*funcToExecute)());

	// An on/off option.
	// Note: bToggle parameter must be an already defined boolean variable.
	// Passed function will execute when option is pressed.
	void ToggleOption(std::string optionText, std::string optionFooter, bool* bToggle, bool bUseCheckmark, void(*funcToExecute)());

	// An option with left and right arrows with multiple options.
	// Passed function will execute when vector option is changed.
	void VectorOption(std::string optionText, std::string optionFooter, std::vector<std::string> vectorOptions, void(*funcToExecute)());
	void VectorOption(std::string optionText, std::string optionFooter, int numberOfOptions, std::string startText, std::string endText, void(*funcToExecute)());

	// Creates an empty spot on the page where the next option would be.
	// Selection box will skip over this spot, but an index will still be assigned.
	// Indexes must account for this function.
	void Separator();
}

void RegisterUIPrompt(Prompt& prompt, Hash control, const char* promptText);
// Objective Hint
void PrintSubtitle(const std::string& str);

std::unordered_map<double, void(*)()> getPageStructures();
void resetNextAssignedIndex();
