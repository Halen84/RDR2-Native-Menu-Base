// Licensed under the MIT License.

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

enum class Font : int {
	Arial,
	Cabrito,
	DroidSerif,
	Hapna,
	Helvetica,
	Catalogue,
	Chalk,
	Gothica,
	Ledger,
	Lino,
	Turri,
	Redemption,
	Gamertag,
	DreamerScript,
};

enum class Alignment : int {
	Left,
	Center,
	Right
};

const float SCREEN_WIDTH = 1920.0f;
const float SCREEN_HEIGHT = 1080.0f;

const float BG_X_OFFSET = 25.0f;
const float BG_Y_OFFSET = 25.0f;
const float BG_WIDTH    = 576.0f; 
const float BG_HEIGHT   = 1026.0f;

const float TOP_HEADER_WIDTH  = 442.0f; 
const float TOP_HEADER_HEIGHT = 108.0f;
const float TOP_HEADER_X_POS = BG_X_OFFSET + ((BG_WIDTH * 0.5f) - (TOP_HEADER_WIDTH * 0.5f));
const float TOP_HEADER_Y_POS = BG_Y_OFFSET + ((BG_WIDTH - TOP_HEADER_WIDTH) * 0.2f);

const float FOOTER_LINE_WIDTH  = 422.0f; 
const float FOOTER_LINE_HEIGHT = 2.0f;
const float FOOTER_LINE_X_POS = BG_X_OFFSET + ((BG_WIDTH * 0.5f) - (FOOTER_LINE_WIDTH * 0.5f));
const float FOOTER_LINE_Y_POS = BG_Y_OFFSET + (BG_HEIGHT - ((BG_WIDTH - FOOTER_LINE_WIDTH) * 0.5f));

// Y Increment for sprite and text position
const float INCREMENT = (SCREEN_HEIGHT * 0.051f); // const float INCREMENT = 55;

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

void ScriptMain();

int GetCurrentSelectedIndex();
double GetCurrentPageIndex();
