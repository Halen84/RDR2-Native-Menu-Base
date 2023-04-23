// Licensed under the MIT License.

#pragma once
#include "Menu.hpp"
#include "../script.h"
#include "../common.hpp"

const float SCREEN_WIDTH	= static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
const float SCREEN_HEIGHT	= static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

namespace Drawing
{
	void DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth = 0, int letterSpacing = 0);
	void DrawOption(Option* option);
	void DrawMenu();

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
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(0); // Draw behind in-game UI
		GRAPHICS::DRAW_SPRITE(textureDict, textureName, screenX, screenY, width, height, heading, red, green, blue, alpha, false);
	}
}
