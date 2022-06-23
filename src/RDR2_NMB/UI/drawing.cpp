// Licensed under the MIT License.

#include "drawing.h"
#include "util.h"
//#include <format>

const std::vector<const char*> FontList = { "body", "body1", "body2", "body3", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk", "creditNames", "creditTitles",
"Debug_BOLD", "Debug_REG", "FixedWidthNumbers", "Font2", "Font2_cond_NOT_GAMERNAME", "Font5", "Font5_limited", "gtaCash", "gamername", "handwritten", "ledger", "RockstarTAG",
"SOCIAL_CLUB_COND_BOLD", "SOCIAL_CLUB_COND_REG", "title", "title1", "title2", "util", "wantedPostersGeneric" };


// I didn't want to import the ~200KB std::format lib, so I just made my own dumbed down version
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

void correctFontSize(int* fontSize)
{
	/*
	This is from my personal testing. 
	
	Let's say fontSize is 22. To properly display font on a 4k (3840) screen,
	we need to increase fontSize by 10. 22 + 10 = 32. 3840/120 = 32.

	Let's say fontSize is 22. To properly display font on a 1280 screen,
	we need to decrease fontSize by 6. 22 - 6 = 16. 1280/80 = 16.

	- Decimals get truncated on integer conversion.
	- This is far from being good, but it works for now.
	- This should work on all screen sizes. 1920x1080 is unaffected buy this.
	*/

	int size = -1;
	if (Menu::Util::g_windowHandle != NULL) {
		if (Menu::Util::g_screenWidth > 1920) {
			size = Menu::Util::g_screenWidth / 120;
		}
		else if (Menu::Util::g_screenWidth < 1920) {
			size = Menu::Util::g_screenWidth / 80;
		}
	}

	if (size != -1) {
		*fontSize = size;
	}
}

// Used in DrawOption()
void DrawFooter(const std::string& text)
{
	Menu::Drawing::DrawFormattedText(text, Font::Body, 255, 255, 255, 255, Alignment::Center, 18, BG_X_OFFSET + (BG_WIDTH * 0.5f), 980);
}


namespace Menu
{
	void Drawing::DrawHeader(const std::string& text, int size, float yPos)
	{
		Drawing::DrawFormattedText(text, Font::Title, 255, 255, 255, 255, Alignment::Center, size, BG_X_OFFSET + (BG_WIDTH * 0.5f), yPos);
	}


	void Drawing::DrawSubHeader(const std::string& text)
	{
		Drawing::DrawFormattedText(text, Font::Title, 255, 255, 255, 255, Alignment::Center, 23, BG_X_OFFSET + (BG_WIDTH * 0.5f), 172);
	}


	void Drawing::DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth, int letterSpacing)
	{
		correctFontSize(&textSize); // See comment inside function
		
		std::string _font = FontList[static_cast<int>(font)];

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

		std::string formatText = "<TEXTFORMAT RIGHTMARGIN = '%s'><P ALIGN = '%s'><FONT FACE = '$%s' LETTERSPACING = '%s' SIZE = '%s'>~s~%s</FONT></P><TEXTFORMAT>";
		// BASELINE, BLOCKINDENT, CLASS, HEIGHT, WIDTH, HREF, HSPACE, VSPACE, ID, INDENT, KERNING, LEADING, LEFTMARGIN, SRC, TABSTOPS, TARGET, COLOR

		format(formatText, _rightMargin, _align, _font, _letterSpacing, _size, text);

#if BUILD_1311_COMPATIBLE
		HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(formatText.c_str()))), x, y);
#else
		UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(formatText.c_str()))), x, y);
#endif
	}


	void Drawing::DrawOption(Option* option)
	{
		int selection = g_NativeMenu->m_SelectionIndex;
		int visibleOptions = g_NativeMenu->CurrentSubmenu->m_NumVisibleOptions;
		int index = option->m_Index;

		if (option->m_IsSubMenuOption && (option->m_IsBoolOption || option->m_IsVectorOption || option->m_IsRegularOption || option->m_IsPageBreak)) {
			option->m_IsSubMenuOption = false;
		}
		if (option->m_IsPageBreak && (option->m_IsBoolOption || option->m_IsVectorOption || option->m_IsRegularOption || option->m_IsSubMenuOption)) {
			option->m_IsBoolOption = false; option->m_IsVectorOption = false; option->m_IsRegularOption = false; option->m_IsSubMenuOption = false;
		}

		// Draw background
		if (selection <= visibleOptions-1 && index <= visibleOptions-1) {
			if (!option->m_IsPageBreak) {
				DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + (index * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
				DrawFormattedText(option->GetText(), Font::Body, 255, 255, 255, 255, Alignment::Left, 22, 98, 254 + (index * INCREMENT));
			} else {
				DrawFormattedText("", Font::Body, 255, 255, 255, 255, Alignment::Center, 22, BG_X_OFFSET + (BG_WIDTH * 0.5f), 254 + (index * INCREMENT));
			}
		} else if ((index > (selection - visibleOptions)) && index <= selection) {
			if (!option->m_IsPageBreak) {
				DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + ((index - (selection - (visibleOptions-1))) * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 50, 50, 50, 110, true);
				DrawFormattedText(option->GetText(), Font::Body, 255, 255, 255, 255, Alignment::Left, 22, 98, 254 + ((index - (selection - (visibleOptions - 1))) * INCREMENT));
			} else {
				DrawFormattedText("", Font::Body, 255, 255, 255, 255, Alignment::Center, 22, BG_X_OFFSET + (BG_WIDTH * 0.5f), 254 + ((index - (selection - (visibleOptions - 1))) * INCREMENT));
			}
		}

		// Draw footer
		if (selection == index && !option->m_IsPageBreak) {
			//g_NativeMenu->m_sTextAtCurrSelection = option->GetText();
			DrawFooter(option->GetFooter());
		}

		// Draw vector arrows and text
		if (option->m_IsVectorOption) {
			if (selection <= visibleOptions-1 && index <= visibleOptions-1) {
				if (selection == index)
					DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->GetText(true) + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
				else
					DrawFormattedText(option->GetText(true), Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
			}
			else if ((index > (selection - visibleOptions)) && index <= selection) {
				if (selection == index)
					DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->GetText(true) + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 254 + ((index - (selection - (visibleOptions-1))) * INCREMENT));
				else
					DrawFormattedText(option->GetText(true), Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 254 + ((index - (selection - (visibleOptions-1))) * INCREMENT));
			}
		}

		// Draw checkmark
		if (option->m_IsBoolOption) {
			if (option->GetBoolPtr() != nullptr && *option->GetBoolPtr() == true) {
				if (selection <= visibleOptions-1 && index <= visibleOptions-1) {
					DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 250 + (index * INCREMENT));
				}
				else if ((index > (selection - visibleOptions)) && index <= selection) {
					DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Body, 255, 255, 255, 255, Alignment::Right, 22, 520, 250 + ((index - (selection - (visibleOptions-1))) * INCREMENT));
				}
			}
		}
	}


	void Drawing::DrawMenuTextures()
	{
		int numOptions = g_NativeMenu->CurrentSubmenu->m_NumOptions;
		int visOptions = g_NativeMenu->CurrentSubmenu->m_NumVisibleOptions;
		int selection = g_NativeMenu->m_SelectionIndex;

		// Background
		DrawSprite("generic_textures", "inkroller_1a", BG_X_OFFSET, BG_Y_OFFSET, BG_WIDTH, BG_HEIGHT, 0.0, 0, 0, 0, 230, false);
		
		// Header
		DrawSprite("generic_textures", "menu_header_1a", TOP_HEADER_X_POS, TOP_HEADER_Y_POS, TOP_HEADER_WIDTH, TOP_HEADER_HEIGHT, 0, 255, 255, 255, 255, false);
		
		// Footer - TODO: Find more accurate texture
		GRAPHICS::DRAW_SPRITE("generic_textures", "menu_bar", 0.16, 0.9, 0.23, 0.0015, 0, 255, 255, 255, 175, false); // Doesn't draw right with Drawing::DrawSprite for some reason
		
		// Scroller Sprites

		// Top
		DrawSprite("menu_textures", "scroller_left_top", 90, 215, (TOP_HEADER_WIDTH / 2.0f) - 14.0f, 25, 0, 255, 255, 255, 255, false);
		DrawSprite("menu_textures", "scroller_right_top", 322, 215, (TOP_HEADER_WIDTH / 2.0f) - 14.0f, 25, 0, 255, 255, 255, 255, false);
		if (selection >= visOptions)
			DrawSprite("menu_textures", "scroller_arrow_top", 310, 227.5f, 25, 25, 0, 255, 255, 255, 255, true);
		else
			DrawSprite("menu_textures", "scroller_line_up", 310, 227.5f, 25, 25, 0, 255, 255, 255, 255, true);

		// Bottom
		if (numOptions <= visOptions) {
			DrawSprite("menu_textures", "scroller_left_bottom", 90, 244 + (numOptions * INCREMENT), (TOP_HEADER_WIDTH / 2.0f) - 14.0f, 25, 0, 255, 255, 255, 255, false);
			DrawSprite("menu_textures", "scroller_right_bottom", 319, 244 + (numOptions * INCREMENT), (TOP_HEADER_WIDTH / 2.0f) - 10.0f, 25, 0, 255, 255, 255, 255, false);
			DrawSprite("menu_textures", "scroller_line_down", 308.5f, 256.5f + (numOptions * INCREMENT), 22, 25, 0, 255, 255, 255, 255, true); // ???
		}
		else {
			DrawSprite("menu_textures", "scroller_left_bottom", 90, 244 + (visOptions * INCREMENT), (TOP_HEADER_WIDTH / 2.0f) - 14.0f, 25, 0, 255, 255, 255, 255, false);
			DrawSprite("menu_textures", "scroller_right_bottom", 322, 244 + (visOptions * INCREMENT), (TOP_HEADER_WIDTH / 2.0f) - 14.0f, 25, 0, 255, 255, 255, 255, false);
			if (selection == numOptions - 1) // We're at the very last option, don't draw the arrow
				DrawSprite("menu_textures", "scroller_line_down", 310, 256.5f + (visOptions * INCREMENT), 25, 25, 0, 255, 255, 255, 255, true);
			else
				DrawSprite("menu_textures", "scroller_arrow_bottom", 310, 256.5f + (visOptions * INCREMENT), 25, 25, 0, 255, 255, 255, 255, true);
		}
		
	}


	void Drawing::DrawSelectionBox()
	{
		// TODO: Micro adjust these

		// Couldn't add the corner textures because they get stretched out and
		// just don't look right, so the lines are sized to be touching each other.

		int selection = g_NativeMenu->m_SelectionIndex;
		int visibleOptions = g_NativeMenu->CurrentSubmenu->m_NumVisibleOptions;

		// Left, Right, Top, Bottom
		if (selection >= visibleOptions-1) {
			DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + ((visibleOptions-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + ((visibleOptions-1) * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + ((visibleOptions-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + ((visibleOptions-1) * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
		} else {
			DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (selection * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (selection * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (selection * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
			DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (selection * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
		}
	}
}
