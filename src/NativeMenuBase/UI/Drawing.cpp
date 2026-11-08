// Licensed under the MIT License.

#include "drawing.h"
#include "UIUtil.h"

const float INCREMENT = (SCREEN_HEIGHT * 0.050f); // Y Position increment for option and sprite positions

const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };

// Fix font size based on display size
int _actualScreenWidth = -1;
static int CalculateFontSize(int fontSize)
{
	if (_actualScreenWidth == -1) {
		_actualScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	}
	fontSize = (fontSize * UIUtil::g_screenWidth) / _actualScreenWidth;
	return fontSize;
}

#if 0
static float CalcTextWidth(const std::string& text, int fontSize)
{
	// magic...
	float totalWidth = 0;
	float size_small = fontSize * 0.35f;
	float size_medium = fontSize * 0.65f;
	float size_large = fontSize * 0.85f;
	float size_xlarge = fontSize * 1.0f;

	for (char c : text)
	{
		if (c == 'i' || c == 'l' || c == 'I' || c == 't' || c == 'f' || c == 'j' || c == 'r' || c == '1') {
			totalWidth += size_small;
		}
		else if (c == 'm' || c == 'w') {
			totalWidth += size_large;
		}
		else if (c == 'W' || c == 'M') {
			totalWidth += size_xlarge;
		}
		else if (c == ' ') {
			totalWidth += fontSize * 0.35f; // Space width
		}
		else {
			totalWidth += size_medium;
		}
	}

	return totalWidth;
}
#endif

void CDrawing::Text(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth, int letterSpacing)
{
	textSize = CalculateFontSize(textSize);

	float x = (posX / SCREEN_WIDTH);
	float y = (posY / SCREEN_HEIGHT);
	if (align == Alignment::Right) { x = 0.0f; }
	if (align == Alignment::Center) { x = -1.0f + (x * 2.0f); }

	const char* _font = FontList[(int)font];
	const int _rightMargin = (align == Alignment::Right ? ((int)(SCREEN_WIDTH)-(int)(posX)) : (wrapWidth != 0 ? ((int)(SCREEN_WIDTH) - ((int)(posX) + wrapWidth)) : 0));
	const char* _align = (align == Alignment::Left ? "Left" : align == Alignment::Right ? "Right" : "Center");

	char buffer[512]; // Leave some room for the raw text
	snprintf(buffer, sizeof buffer,
		"<TEXTFORMAT RIGHTMARGIN='%d'><P ALIGN='%s'><FONT FACE='$%s' LETTERSPACING='%d' SIZE='%d'>"
		"~s~%s"
		"</FONT></P></TEXTFORMAT>",
		_rightMargin, _align, _font, letterSpacing, textSize, text.c_str());
	// BASELINE, BLOCKINDENT, CLASS, HEIGHT, WIDTH, HREF, HSPACE, VSPACE, ID, INDENT, KERNING, LEADING, LEFTMARGIN, SRC, TABSTOPS, TARGET, COLOR
	
	UIDEBUG::_BG_SET_TEXT_COLOR(red, green, blue, alpha);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", buffer), x, y);
}

void CDrawing::DrawMenu(const char* header, const char* subheader)
{
	DrawBackground();

	Page& pg = CMenu::GetCurrentPage();
	CDrawing::Text(header, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::Header, uiBackground::MiddleX, uiHeader::TextPositionY);
	CDrawing::Text(subheader, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::SubHeader, uiBackground::MiddleX, uiHeader::SubHeaderTextPositionY);
	CDrawing::Text(std::to_string(pg.Cursor+1) + " of " + std::to_string(pg.OptionCount), Font::Body, 144,144,144,230, Alignment::Right, uiFontSizes::OptionCounter, uiOption::OptionCounterPositionX, 243.0f + (min(pg.OptionCount,8) * INCREMENT), 0, -1);
}

void CDrawing::DrawOption(int slot, const char* label, const char* footer, bool hovered, OptionKind kind, void* extradata)
{
	float y = (slot * (SCREEN_HEIGHT * 0.05f)) + uiOption::PositionYStart;
	float textY = y - (uiOption::Height * 0.5f) + (uiFontSizes::Option * 0.5f);

	DrawSprite("generic_textures", "selection_box_bg_1c", uiOption::PositionX, y, uiOption::Width, uiOption::Height, 0, 50,50,50,110, true);
	CDrawing::Text(label, Font::Body, COLOR_WHITE_SMOKE, Alignment::Left, uiFontSizes::Option, uiOption::LeftTextPositionX, textY);

	if (hovered) {
		DrawSprite("menu_textures", "crafting_highlight_l", uiSelectionBox::LeftPositionX,   y, 19, uiSelectionBox::LeftHeight, 0, COLOR_RED, true);
		DrawSprite("menu_textures", "crafting_highlight_r", uiSelectionBox::RightPositionX,  y, 19, uiSelectionBox::RightHeight, 0, COLOR_RED, true);
		DrawSprite("menu_textures", "crafting_highlight_t", uiSelectionBox::TopPositionX,    y - 22, uiSelectionBox::TopWidth, 22, 0, COLOR_RED, true);
		DrawSprite("menu_textures", "crafting_highlight_b", uiSelectionBox::BottomPositionX, y + 22, uiSelectionBox::BottomWidth, 22, 0, COLOR_RED, true);
		CDrawing::Text(footer, Font::Body, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::Footer, uiBackground::MiddleX, uiFooter::TextPositionY);
	}

	switch (kind)
	{
		case OptionKind::Regular:
		case OptionKind::Submenu:
			break;
		case OptionKind::Checkbox:
			DrawSprite("generic_textures", "tick_box", uiCheckbox::PositionX, y, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
			if (extradata && *(bool*)extradata == true) {
				DrawSprite("generic_textures", "tick", uiCheckbox::PositionX, y, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
			}
			break;
		case OptionKind::List:
			if (extradata) {
				if (hovered) {
					//DrawSprite("menu_textures", "selection_arrow_right", uiOption::RightTextPositionX - 18, (uiOption::TextPositionYStart - 16) + inRangeIncr, 18, 18, 0, 255, 0, 0, 255, false);
					//float textWidth = CalcTextWidth(option->RightText, uiFontSizes::Option);
					//float imageX = (uiOption::RightTextPositionX - textWidth) - (18*2);
					//if (option->RightText.length() == 1) {
					//	imageX -= 8.0f;
					//}
					//DrawSprite("menu_textures", "selection_arrow_left", imageX, (uiOption::TextPositionYStart - 16) + inRangeIncr, 18, 18, 0, 255, 0, 0, 255, false);

					// TODO: can't use img src because it messes up scaling on different resolutions
					CDrawing::Text("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + std::string((const char*)extradata) + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, COLOR_PURE_WHITE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, textY);
				}
				else {
					CDrawing::Text((const char*)extradata, Font::Body, COLOR_PURE_WHITE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, textY);
				}
			}
			break;
	}
}

void CDrawing::DrawSprite(const char* textureDict, const char* textureName, float screenX, float screenY, float width, float height, float heading, int red, int green, int blue, int alpha, bool centered)
{
	screenX /= SCREEN_WIDTH;
	screenY /= SCREEN_HEIGHT;
	width   /= SCREEN_WIDTH;
	height  /= SCREEN_HEIGHT;
	if (!centered) {
		screenX += width * 0.5f;
		screenY += height * 0.5f;
	}
	GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
	GRAPHICS::DRAW_SPRITE(textureDict, textureName, screenX, screenY, width, height, heading, red, green, blue, alpha, false);
}

void CDrawing::DrawBackground()
{
	// Background
	DrawSprite("generic_textures", "inkroller_1a", uiBackground::PositionX, uiBackground::PositionY, uiBackground::Width, uiBackground::Height, 0, 0,0,0,230, false);

	// Header
	DrawSprite("generic_textures", "menu_header_1a", uiHeader::PositionX, uiHeader::PositionY, uiHeader::Width, uiHeader::Height, 0, COLOR_PURE_WHITE, false);

	// Footer
	DrawSprite("generic_textures", "menu_bar", uiFooter::PositionX, uiFooter::PositionY, uiFooter::Width, uiFooter::Height, 0, 255, 255, 255, 175, true);

	//
	// Scroller Sprites
	//

	Page& pg = CMenu::GetCurrentPage();

	int visible    = min(pg.OptionCount, 8);
	bool moreAbove = pg.Scroll > 0;
	bool moreBelow = pg.Scroll + visible < pg.OptionCount;

	// Top
	DrawSprite("menu_textures", "scroller_left_top",  uiScroller::LeftPositionX,  215, uiScroller::LeftWidth,  25, 0, COLOR_PURE_WHITE, false);
	DrawSprite("menu_textures", "scroller_right_top", uiScroller::RightPositionX, 215, uiScroller::RightWidth, 25, 0, COLOR_PURE_WHITE, false);
	DrawSprite("menu_textures", moreAbove ? "scroller_arrow_top" : "scroller_line_up", uiScroller::MiddlePositionX, 227.5f, uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);

	// Bottom
	const float bottomY = visible * INCREMENT;
	DrawSprite("menu_textures", "scroller_left_bottom",  uiScroller::LeftPositionX,  244 + bottomY, uiScroller::LeftWidth,  25, 0, COLOR_PURE_WHITE, false);
	DrawSprite("menu_textures", "scroller_right_bottom", uiScroller::RightPositionX, 244 + bottomY, uiScroller::RightWidth, 25, 0, COLOR_PURE_WHITE, false);
	DrawSprite("menu_textures", moreBelow ? "scroller_arrow_bottom" : "scroller_line_down", uiScroller::MiddlePositionX, 256.5f + bottomY, uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
}
