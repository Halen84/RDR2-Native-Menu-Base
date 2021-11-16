// Licensed under the MIT License.

#pragma once

#include <string>
#include <vector>
#include "script.h"


namespace Draw
{
	// Draw CSS styled text
	void DrawCSSText(std::string text, Font font, int R, int G, int B, int A, Alignment align, int textSize, float X, float Y, int wrapWidth = 0, int letterSpacing = 0);

	// Draw a new option
	void DrawOption(std::string text, int index, bool bHasPage = false, bool bIsToggleOption = false);

	// Draws a selection box around currently selected item
	void DrawSelectionBox();

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
	void SetHeader(std::string text, int fontSize = 45, float yPos = 80);
	void SetSubHeader(std::string text);
	void SetFooter(std::string text); // Not a header, but I don't really care
}

namespace Menu
{
	// Add options to a toggle with std::vector<string>
	void AddOptionsToToggle(int index, std::vector<std::string> options);
	// Add options to a toggle with static text
	void AddOptionsToToggle(int index, int numberOfOptions, std::string baseText);

	void SetToggleSelection(double pageIndex, int toggleIndex, int newPos);
	void SetTextAtPos(std::string newText, double pageIndex, int toggleIndex, int pos);
	// Get text at current selected index
	std::string GetTextAtCurrentSelection();
	int GetToggleSelection(double pageIndex, int toggleIndex);

	void OnSelect();
	void OnToggle(bool left, bool right);

	void CreateUIPrompt(Prompt& prompt, Hash control, const char* promptText);
	bool DoesOptionHavePage(double pageIndex, int index);
	bool DoesOptionHaveToggle(double pageIndex, int index);
	int GetNumOptionsInCurrentPage();
}


// Objective Hint
void ShowSubtitle(const std::string& str);
