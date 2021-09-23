// Licensed under the MIT License.

#include <string>
#include "script.h"
#include "pages.h"
#include <vector>

void DrawCSSText(std::string text, Font font, int R, int G, int B, int A, Alignment align, int textSize, float X, float Y, int wrapWidth = 0, int letterSpacing = 0)
{
	std::vector<std::string> fontList = { "util", "catalog5", "body1", "body", "Debug_REG", "catalog4", "chalk", "catalog1", "ledger", "title", "wantedPostersGeneric", "gtaCash", "gamername", "handwritten"};
	std::string _font = fontList[static_cast<int>(font)];

	float x = (X / SCREEN_WIDTH);
	if (align == Alignment::Right) { x = 0.0f; }
	if (align == Alignment::Center) { x = -1.0f + (x * 2.0f); }
	float y = (Y) / SCREEN_HEIGHT;
	
	HUD::_SET_TEXT_COLOR(R, G, B, A);
	
	std::string cap = "<TEXTFORMAT RIGHTMARGIN ='" + (align == Alignment::Right ? std::to_string(static_cast<int>(SCREEN_WIDTH) - static_cast<int>(X)) : (wrapWidth != 0 ? std::to_string(static_cast<int>(SCREEN_WIDTH) - (static_cast<int>(X) + wrapWidth)) : "0")) 
		+ "'><P ALIGN='" + (align == Alignment::Left ? "left" : align == Alignment::Right ? "right" : "center") +  "'><font face='$" + _font + "' letterspacing ='" + std::to_string(letterSpacing) + "' size='" + std::to_string(textSize) + "'>~s~" + text + "</font></P><TEXTFORMAT>";

	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(cap.c_str()))), x, y);
}


void DrawListOption(std::string text, int index)
{
	int selectedIndex = GetCurrentSelectedIndex();

	if (selectedIndex <= 7 && index <= 7) {
		DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + (index * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 32, 32, 32, 200, true);
		DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + (index * INCREMENT));
	} else if ((index > (selectedIndex - 8)) && index <= selectedIndex) {
		DrawSprite("generic_textures", "selection_box_bg_1c", 310, 270 + ((index - (selectedIndex - 7)) * INCREMENT), TOP_HEADER_WIDTH, 52, 0, 32, 32, 32, 200, true);
		DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Left, 22, 98, 254 + ((index - (selectedIndex - 7)) * INCREMENT));
	}
}

// todo: maybe add parameters for default on and off text?
void DrawToggleOption(std::string text, int index)
{
	double pageIndex = GetCurrentPageIndex();

	DrawListOption(text, index);
	std::string toggleText = GetText(index);

	if (GetCurrentSelectedIndex() == index) {
		DrawCSSText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + toggleText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
	} else {
		DrawCSSText(toggleText, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Right, 22, 520, 254 + (index * INCREMENT));
	}
}


void DrawSelectionBox()
{
	int index = GetCurrentSelectedIndex();

	// Left, Right, Top, Bottom
	if (index >= 7) {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (7 * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (7 * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (7 * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 309, 293 + (7 * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22 , 0, 255, 0, 0, 255, true);
	} else {
		DrawSprite("menu_textures", "crafting_highlight_l",  86, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_r", 534, 270 + (index * INCREMENT), 19, 59, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_t", 310, 247 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
		DrawSprite("menu_textures", "crafting_highlight_b", 310, 293 + (index * INCREMENT), TOP_HEADER_WIDTH + 6.0f, 22, 0, 255, 0, 0, 255, true);
	}
}


void CreateControlAction(Prompt& prompt, Hash control, const char* text)
{
	prompt = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_0xF4A5C4509BF923B1(prompt, 0);
	HUD::_UIPROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UIPROMPT_SET_TEXT(prompt, text);
	HUD::_UIPROMPT_SET_STANDARD_MODE(prompt, 1);
	HUD::_UIPROMPT_REGISTER_END(prompt);

	HUD::_UIPROMPT_SET_VISIBLE(prompt, false);
	HUD::_UIPROMPT_SET_ENABLED(prompt, false);
}


void SetHeader(std::string text, int fontSize) {
	DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, fontSize,  BG_X_OFFSET + (BG_WIDTH * 0.5f), 80);
}


void SetSubHeader(std::string text) {
	DrawCSSText(text, Font::Lino, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 23, BG_X_OFFSET + (BG_WIDTH * 0.5f), 80 + 92);
}


void SetFooter(std::string text) {
	DrawCSSText(text, Font::Hapna, 0xff, 0xff, 0xff, 0xff, Alignment::Center, 20, BG_X_OFFSET + (BG_WIDTH * 0.5f), 980);
}


void ShowSubtitle(const std::string& str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
