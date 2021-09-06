// Licensed under the MIT License.

#include <string>
#include "script.h"
#include "pages.h"

// Y Increment for sprite and text position
const float INCREMENT = 0.053f;

void DrawCSSText(std::string text, std::string font, std::string color, std::string align, int leading, int textSize, float X, float Y)
{
	// Note: Right align is cursed.

	font = "'$" + font + "'";
	color = "'#" + color + "'";
	align = "'" + align + "'";
	std::string _leading = "'" + std::to_string(leading) + "'";
	std::string size = "'" + std::to_string(textSize) + "'";

	// https://www.rdr2mods.com/forums/topic/649-changing-draw_text-font/
	std::string textTemplate = "<TEXTFORMAT LEADING=" + _leading + "><FONT FACE=" + font + " COLOR=" + color + "><P ALIGN=" + align + "> <FONT SIZE=" + size + ">" + text + "</P></FONT></TEXTFORMAT>";

	const char* insertTemplate = MISC::VAR_STRING(10, "LITERAL_STRING", _strdup(textTemplate.c_str()));
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(42, "COLOR_STRING", 0, insertTemplate), X, Y);
}


void DrawListOption(std::string text, int index)
{
	// int selectedIndex = GetCurrentSelectedIndex();
	// int numOptions = GetNumOptionsInCurrentPage();

	// Scrolling would be implemented here I believe, but I have no idea how.
	GRAPHICS::DRAW_SPRITE("generic_textures", "selection_box_bg_1c", 0.16, 0.25f + (index * INCREMENT), 0.22, 0.05, 0, 32, 32, 32, 200, false);
	DrawCSSText(text, "body", "FFFFFF", "LEFT", 0, 22, 0.04, 0.195f + (index * INCREMENT));
}


void DrawToggleOption(std::string text, double pageIndex, int index)
{
	// GRAPHICS::DRAW_SPRITE("generic_textures", "tick", 0.255, 0.25f + (index * INCREMENT), 0.018, 0.035, 0, 255, 255, 255, 255, false);

	DrawListOption(text, index);
	std::string toggleText = GetTextState(pageIndex, index);

	if (GetCurrentSelectedIndex() == index) {
		std::string textTemplate = "<FONT FACE='$body' COLOR='#FFFFFF'> <FONT SIZE='22'> <img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + toggleText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/></FONT>";

		const char* insertTemplate = MISC::VAR_STRING(10, "LITERAL_STRING", _strdup(textTemplate.c_str()));
		UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(42, "COLOR_STRING", 0, insertTemplate), 0.2f, 0.195f + (index * INCREMENT));
	} else {
		DrawCSSText(toggleText, "body", "FFFFFF", "LEFT", 0, 22, 0.23f, 0.195f + (index * INCREMENT));
	}
}


void DrawSelectionBox()
{
	int index = GetCurrentSelectedIndex();

	// Left, Right, Top, Bottom
	GRAPHICS::DRAW_SPRITE("menu_textures", "crafting_highlight_l", 0.05, 0.25f + (index * INCREMENT), 0.01, 0.052, 0, 255, 0, 0, 255, false);
	GRAPHICS::DRAW_SPRITE("menu_textures", "crafting_highlight_r", 0.27, 0.25f + (index * INCREMENT), 0.01, 0.052, 0, 255, 0, 0, 255, false);
	GRAPHICS::DRAW_SPRITE("menu_textures", "crafting_highlight_t", 0.16, 0.23f + (index * INCREMENT), 0.221, 0.02, 0, 255, 0, 0, 255, false);
	GRAPHICS::DRAW_SPRITE("menu_textures", "crafting_highlight_b", 0.16, 0.27f + (index * INCREMENT), 0.22, 0.02, 0, 255, 0, 0, 255, false);
}


void CreateControlAction(Prompt& prompt, Hash control, const char* text)
{
	prompt = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_0xF4A5C4509BF923B1(prompt, 0); // I don't even know what this does lol
	HUD::_UIPROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UIPROMPT_SET_TEXT(prompt, text);
	HUD::_UIPROMPT_SET_STANDARD_MODE(prompt, 1);
	HUD::_UIPROMPT_REGISTER_END(prompt);

	HUD::_UIPROMPT_SET_VISIBLE(prompt, false);
	HUD::_UIPROMPT_SET_ENABLED(prompt, false);
}


void SetHeader(std::string text, int fontSize)
{
	DrawCSSText(text, "title2", "FFFFFF", "CENTER", 0, fontSize, -0.688, 0.057);
}


void SetSubHeader(std::string text)
{
	DrawCSSText(text, "title2", "FFFFFF", "CENTER", 0, 23, -0.688, 0.125);
}


void SetFooter(std::string text)
{
	DrawCSSText(text, "body", "FFFFFF", "CENTER", -10, 20, -0.69, 0.87);
}


void ShowSubtitle(const std::string& str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
