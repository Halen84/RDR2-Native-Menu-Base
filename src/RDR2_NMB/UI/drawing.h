// Licensed under the MIT License.

#pragma once
#include "menu.hpp"
#include "../script.h"
#include "../common.hpp"

const float SCREEN_WIDTH	= 1920.0f;
const float SCREEN_HEIGHT	= 1080.0f;

namespace Menu::Drawing
{
	void DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth = 0, int letterSpacing = 0);
	void DrawOption(Option* option);
	void DrawMenuTextures();
	void DrawSelectionBox();
	void DrawOptionCounter();

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

	void DrawHeader(const std::string& text, int size = 45, float yPos = 79.0f);
	void DrawSubHeader(const std::string& text);
}
