// Licensed under the MIT License.

#pragma once
#include "Menu.h"
#include "../script.h"
#include "../common.h"

const float SCREEN_WIDTH	= static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
const float SCREEN_HEIGHT	= static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

#pragma region UI Constants
#define COLOR_PURE_WHITE 255,255,255,255
#define COLOR_WHITE_SMOKE 245,245,245,255
#define COLOR_GREYDARK 93,91,90,255
#define COLOR_RED 204,0,0,255
#define HALF(x)((x) * 0.5f)

struct uiFontSizes {
	static constexpr int Header = 45;
	static constexpr int SubHeader = 22;
	static constexpr int Option = 22;
	static constexpr int OptionCounter = 20;
	static constexpr int Footer = 18;
	static constexpr int Separator = 22;
};
struct uiBackground {
	static constexpr float PositionX = 25.0f;
	static constexpr float PositionY = 25.0f;
	static constexpr float Width = 575.0f;
	static constexpr float Height = 1025.0f;
	static constexpr float MiddleX = PositionX + HALF(Width); // The center X position for the background
	static constexpr float MiddleY = PositionY + HALF(Height); // The center Y position for the background
};
struct uiHeader {
	static constexpr float Width = 445.0f;
	static constexpr float Height = 110.0f;
	static constexpr float PositionX = uiBackground::MiddleX - HALF(Width);
	static constexpr float PositionY = uiBackground::PositionY + ((uiBackground::Width - Width) * 0.2f);
	// What the fuck is this calculation
	static constexpr float TextPositionY = ((PositionY + HALF(Height)) - HALF(uiFontSizes::Header)) - (HALF(uiFontSizes::Header) * 0.1f);
	static constexpr float SubHeaderTextPositionY = (PositionY + Height) + HALF(uiFontSizes::SubHeader);
};
struct uiFooter {
	static constexpr float Width = uiHeader::Width;
	static constexpr float Height = 2.0f;
	static constexpr float PositionX = uiBackground::MiddleX;
	static constexpr float PositionY = HALF(uiBackground::PositionY) + (uiBackground::Height - HALF(uiBackground::Width - Width)) - HALF(Height);
	static constexpr float TextPositionY = (PositionY + Height) + HALF(uiFontSizes::Footer) - 5.0f;
};
struct uiScroller {
	static constexpr float SizeOffset = 15.0f;
	static constexpr float LeftWidth = HALF(uiHeader::Width) - SizeOffset;
	static constexpr float LeftPositionX = (uiBackground::MiddleX - (LeftWidth + SizeOffset));
	static constexpr float RightWidth = LeftWidth;
	static constexpr float RightPositionX = uiBackground::MiddleX + SizeOffset;
	static constexpr float MiddleWidth = SizeOffset * 2.0f;
	static constexpr float MiddlePositionX = uiBackground::MiddleX;
};
struct uiOption {
	static constexpr float Width = uiHeader::Width;
	static constexpr float Height = 50.0f;
	static constexpr float PositionX = uiBackground::MiddleX;
	static constexpr float TextOffset = 8.0f;
	static constexpr float LeftTextPositionX = (PositionX - HALF(Width)) + TextOffset; // X Position of text
	static constexpr float RightTextPositionX = (PositionX + HALF(Width)) - TextOffset; // X Position of Vector text

	static constexpr float PositionYStart = 270.0f; // Position at which the first option will be drawn at
	static constexpr int TextPositionYStart = 275; // Position at which the text of the first option will be drawn at

	static constexpr float SeparatorTextPositionX = uiBackground::MiddleX;
	static constexpr float OptionCounterPositionX = PositionX + HALF(Width);
};
struct uiSelectionBox {
	static constexpr float OffsetUD = 8.0f; // Up/Down Offset
	static constexpr float OffsetLR = 3.0f; // Left/Right Offset

	static constexpr float TopWidth = uiOption::Width + OffsetUD;
	static constexpr float TopPositionX = uiOption::PositionX;
	static constexpr float TopPositionY = (uiOption::PositionYStart - HALF(uiOption::Height)) + 3.0f;

	static constexpr float BottomWidth = TopWidth;
	static constexpr float BottomPositionX = uiOption::PositionX;
	static constexpr float BottomPositionY = (uiOption::PositionYStart + HALF(uiOption::Height)) - 3.0f;

	static constexpr float LeftHeight = uiOption::Height + OffsetLR + 1.0f;
	static constexpr float LeftPositionX = (uiOption::PositionX - HALF(uiOption::Width)) - OffsetLR;

	static constexpr float RightHeight = LeftHeight;
	static constexpr float RightPositionX = (uiOption::PositionX + HALF(uiOption::Width)) + OffsetLR;
};
struct uiCheckbox {
	static constexpr float Width = 30.0f;
	static constexpr float Height = Width;
	static constexpr float PositionX = uiBackground::MiddleX + HALF(uiOption::Width) - uiOption::TextOffset - HALF(Width) - 2.0f;
};
#pragma endregion

class CDrawing
{
private:
	static void DrawBackground();
public:
	static void Text(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth = 0, int letterSpacing = 0);
	static void DrawMenu(const char* header, const char* subheader);
	static void DrawOption(int slot, const char* label, const char* footer, bool hovered, OptionKind kind, void* extradata);
	// GRAPHICS::DRAW_SPRITE Wrapper
	static void DrawSprite(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, bool centered);
};
