// Licensed under the MIT License.

#include "drawing.h"
#include "util.h"


#pragma region UI_Constants
// Background Constants
const float BG_X_OFFSET	= 25.0f;
const float BG_Y_OFFSET	= 25.0f;
const float BG_WIDTH	= 576.0f;
const float BG_HEIGHT	= 1026.0f;
const float BG_X_MIDDLE = BG_X_OFFSET + (BG_WIDTH * 0.5f); // The center X position for the background
const float BG_Y_MIDDLE = BG_Y_OFFSET + (BG_HEIGHT / 2.0f); // The center Y position for the background

// Header Constants
const float TOP_HEADER_WIDTH	= 442.0f;
const float TOP_HEADER_HEIGHT	= 108.0f;
const float TOP_HEADER_X_POS	= BG_X_OFFSET + ((BG_WIDTH * 0.5f) - (TOP_HEADER_WIDTH * 0.5f));
const float TOP_HEADER_Y_POS	= BG_Y_OFFSET + ((BG_WIDTH - TOP_HEADER_WIDTH) * 0.2f);

// Footer Constants
const float FOOTER_LINE_WIDTH		= TOP_HEADER_WIDTH;
const float FOOTER_LINE_HEIGHT		= 2.0f;
const float FOOTER_LINE_X_POS		= BG_X_MIDDLE;
const float FOOTER_LINE_Y_POS		= BG_Y_OFFSET + (BG_HEIGHT - ((BG_WIDTH - FOOTER_LINE_WIDTH) * 0.5f)) - (FOOTER_LINE_HEIGHT * 5.0f);

// Scroller Constants
const float SCROLLER_SIZE_OFFSET	= 15.0f;
const float SCROLLER_LEFT_WIDTH		= (TOP_HEADER_WIDTH / 2.0f) - SCROLLER_SIZE_OFFSET;
const float SCROLLER_LEFT_X_POS		= (BG_X_MIDDLE - (SCROLLER_LEFT_WIDTH + SCROLLER_SIZE_OFFSET));
const float SCROLLER_RIGHT_WIDTH	= SCROLLER_LEFT_WIDTH;
const float SCROLLER_RIGHT_X_POS	= BG_X_MIDDLE + SCROLLER_SIZE_OFFSET;
const float SCROLLER_MIDDLE_WIDTH	= SCROLLER_SIZE_OFFSET * 2.0f;
const float SCROLLER_MIDDLE_X_POS	= BG_X_MIDDLE;

// Option Constants
const float OPTION_WIDTH			= TOP_HEADER_WIDTH;
const float OPTION_HEIGHT			= 52.0f; // Probably should be adjusted...
const float OPTION_X_POS			= BG_X_MIDDLE;
const float OPTION_TEXT_OFFSET		= 8.0f;
const float OPTION_TEXT_LEFT_X_POS	= (OPTION_X_POS - (OPTION_WIDTH / 2.0f)) + OPTION_TEXT_OFFSET;
const float OPTION_TEXT_RIGHT_X_POS	= (OPTION_X_POS + (OPTION_WIDTH / 2.0f)) - OPTION_TEXT_OFFSET;

// Selection Box Constants
const float SELECTION_BOX_UD_OFFSET = 12.0f;
const float SELECTION_BOX_LR_OFFSET = 5.0f;
// Selection Box Top
const float SELECTION_BOX_TOP_WIDTH = OPTION_WIDTH + SELECTION_BOX_UD_OFFSET;
const float SELECTION_BOX_TOP_X_POS = OPTION_X_POS;
const float SELECTION_BOX_TOP_Y_POS = (270.0f - (OPTION_HEIGHT / 2.0f)) + 3.0f; // 270 because its the option Y position start, and 3 is a funny magic number
// Selection Box Bottom
const float SELECTION_BOX_BOTTOM_WIDTH = SELECTION_BOX_TOP_WIDTH;
const float SELECTION_BOX_BOTTOM_X_POS = OPTION_X_POS;
const float SELECTION_BOX_BOTTOM_Y_POS = (270.0f + (OPTION_HEIGHT / 2.0f)) - 3.0f; // 270 because its the option Y position start, and 3 is a funny magic number
// Selection Box Left
const float SELECTION_BOX_LEFT_HEIGHT	= OPTION_HEIGHT + SELECTION_BOX_LR_OFFSET;
const float SELECTION_BOX_LEFT_X_POS	= (OPTION_X_POS - (OPTION_WIDTH / 2.0f)) - SELECTION_BOX_LR_OFFSET;
// Selection Box Right
const float SELECTION_BOX_RIGHT_HEIGHT	= SELECTION_BOX_LEFT_HEIGHT;
const float SELECTION_BOX_RIGHT_X_POS	= (OPTION_X_POS + (OPTION_WIDTH / 2.0f)) + SELECTION_BOX_LR_OFFSET;

// Font Size Constants
//const int HEADER_FONT_SIZE = 45;
const int SUB_HEADER_FONT_SIZE = 23;
const int OPTION_FONT_SIZE = 22;
const int FOOTER_FONT_SIZE = 18;
const int PAGE_BREAK_FONT_SIZE = 22;

// Other Constants
const float PAGE_BREAK_TEXT_X_POS = BG_X_MIDDLE;
const float OPTION_COUNTER_X_POS = OPTION_X_POS + (OPTION_WIDTH / 2.0f);
const float INCREMENT = (SCREEN_HEIGHT * 0.051f); // Y increment for option and sprite positions
#pragma endregion


const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };


// I didn't want to import the ~200KB std::format lib, so I just made my own dumbed down version
template<typename... Args>
void format(std::string* str, const Args&... args)
{
	const std::vector<std::string> vec_args = { args... };
	for (int i = 0; i < (int)vec_args.size(); i++) {
		int pos = str->find("%s");
		if (pos != std::string::npos) {
			str->replace(pos, 2, vec_args[i]);
		}
	}
}

// Fix font size based on display size
inline void calculateFontSize(int* fontSize)
{
	*fontSize = (*fontSize * Menu::Util::g_screenWidth) /GetSystemMetrics(SM_CXSCREEN);
}

// Used in DrawOption()
inline void DrawFooter(const std::string& text)
{
	// TODO: Fix position based on font size
	Menu::Drawing::DrawFormattedText(text, Font::Body, 255,255,255,255, Alignment::Center, FOOTER_FONT_SIZE, BG_X_MIDDLE, 980);
}


namespace Menu
{
	void Drawing::DrawHeader(const std::string& text, int size, float yPos)
	{
		// TODO: Fix position based on font size
		Drawing::DrawFormattedText(text, Font::Title, 255,255,255,255, Alignment::Center, size, BG_X_MIDDLE, yPos);
	}


	void Drawing::DrawSubHeader(const std::string& text)
	{
		// TODO: Fix position based on font size
		Drawing::DrawFormattedText(text, Font::Title, 255,255,255,255, Alignment::Center, SUB_HEADER_FONT_SIZE, BG_X_MIDDLE, 172);
	}


	void Drawing::DrawOptionCounter()
	{
		Submenu* currentSubmenu = g_Menu->CurrentSubmenu;
		if (currentSubmenu->m_NumOptions >= currentSubmenu->m_NumVisibleOptions) {
			Drawing::DrawFormattedText(std::to_string(g_Menu->SelectionIndex + 1) + " of " + std::to_string(currentSubmenu->m_NumOptions), Font::Body, 144,144,144,230, Alignment::Right, 20, OPTION_COUNTER_X_POS, 243.0f + (currentSubmenu->m_NumVisibleOptions * INCREMENT), 0, -1);
		} else {
			Drawing::DrawFormattedText(std::to_string(g_Menu->SelectionIndex + 1) + " of " + std::to_string(currentSubmenu->m_NumOptions), Font::Body, 144,144,144,230, Alignment::Right, 20, OPTION_COUNTER_X_POS, 243.0f + (currentSubmenu->m_NumOptions * INCREMENT), 0, -1);
		}
	}


	void Drawing::DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth, int letterSpacing)
	{
		calculateFontSize(&textSize);
		
		const std::string _font = FontList[static_cast<int>(font)];

		float x = (posX / SCREEN_WIDTH);
		float y = (posY / SCREEN_HEIGHT);
		if (align == Alignment::Right) { x = 0.0f; }
		if (align == Alignment::Center) { x = -1.0f + (x * 2.0f); }

#if BUILD_1311_COMPATIBLE
		HUD::_SET_TEXT_COLOR(red, green, blue, alpha);
#else
		UIDEBUG::_BG_SET_TEXT_COLOR(red, green, blue, alpha);
#endif

		const std::string _rightMargin = (align == Alignment::Right ? std::to_string((int)(SCREEN_WIDTH)-(int)(posX)) : (wrapWidth != 0 ? std::to_string((int)(SCREEN_WIDTH) - ((int)(posX) + wrapWidth)) : "0"));
		const std::string _align = (align == Alignment::Left ? "Left" : align == Alignment::Right ? "Right" : "Center");
		const std::string _letterSpacing = std::to_string(letterSpacing);
		const std::string _size = std::to_string(textSize);

		std::string formatText = "<TEXTFORMAT RIGHTMARGIN = '%s'><P ALIGN = '%s'><FONT FACE = '$%s' LETTERSPACING = '%s' SIZE = '%s'>~s~%s</FONT></P><TEXTFORMAT>";
		// BASELINE, BLOCKINDENT, CLASS, HEIGHT, WIDTH, HREF, HSPACE, VSPACE, ID, INDENT, KERNING, LEADING, LEFTMARGIN, SRC, TABSTOPS, TARGET, COLOR

		format(&formatText, _rightMargin, _align, _font, _letterSpacing, _size, text);

#if BUILD_1311_COMPATIBLE
		HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(formatText.c_str()))), x, y);
#else
		UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(formatText.c_str()))), x, y);
#endif
	}


	void Drawing::DrawOption(Option* option)
	{
		int selection = g_Menu->SelectionIndex;
		int visibleOptions = g_Menu->CurrentSubmenu->m_NumVisibleOptions;
		int index = option->m_Index;

		const bool drawInRange = (selection <= visibleOptions - 1 && index <= visibleOptions - 1);
		const bool drawOutOfRange = ((index > (selection - visibleOptions)) && index <= selection);

		// Draw option text and background
		if (drawInRange) {
			if (!option->m_IsPageBreak) {
				DrawSprite("generic_textures", "selection_box_bg_1c", OPTION_X_POS, 270 + (index * INCREMENT), OPTION_WIDTH, OPTION_HEIGHT, 0, 50,50,50,110, true);
				DrawFormattedText(option->GetText(), Font::Body, 255,255,255,255, Alignment::Left, OPTION_FONT_SIZE, OPTION_TEXT_LEFT_X_POS, (276 - (OPTION_FONT_SIZE)) + (index * INCREMENT));
			}
			else {
				DrawFormattedText(option->GetText(), Font::Title, 255,255,255,255, Alignment::Center, PAGE_BREAK_FONT_SIZE, PAGE_BREAK_TEXT_X_POS, (276 - (PAGE_BREAK_FONT_SIZE)) + (index * INCREMENT));
			}
		}
		else if (drawOutOfRange) {
			if (!option->m_IsPageBreak) {
				DrawSprite("generic_textures", "selection_box_bg_1c", OPTION_X_POS, 270 + ((index - (selection - (visibleOptions-1))) * INCREMENT), OPTION_WIDTH, OPTION_HEIGHT, 0, 50,50,50,110, true);
				DrawFormattedText(option->GetText(), Font::Body, 255,255,255,255, Alignment::Left, OPTION_FONT_SIZE, OPTION_TEXT_LEFT_X_POS, (276 - (OPTION_FONT_SIZE)) + ((index - (selection - (visibleOptions - 1))) * INCREMENT));
			}
			else {
				DrawFormattedText(option->GetText(), Font::Title, 255,255,255,255, Alignment::Center, PAGE_BREAK_FONT_SIZE, PAGE_BREAK_TEXT_X_POS, (276 - (PAGE_BREAK_FONT_SIZE)) + ((index - (selection - (visibleOptions - 1))) * INCREMENT));
			}
		}

		// Draw the footer
		if (selection == index && !option->m_IsPageBreak) {
			DrawFooter(option->GetFooter());
		}

		// Draw the vector option arrows and text
		if (option->m_IsVectorOption) {
			if (drawInRange) {
				if (selection == index)
					DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->GetText(true) + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, 255,255,255,255, Alignment::Right, OPTION_FONT_SIZE, OPTION_TEXT_RIGHT_X_POS, (276 - (OPTION_FONT_SIZE)) + (index * INCREMENT));
				else
					DrawFormattedText(option->GetText(true), Font::Body, 255,255,255,255, Alignment::Right, OPTION_FONT_SIZE, OPTION_TEXT_RIGHT_X_POS, (276 - (OPTION_FONT_SIZE)) + (index * INCREMENT));
			}
			else if (drawOutOfRange) {
				if (selection == index)
					DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->GetText(true) + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, 255,255,255,255, Alignment::Right, OPTION_FONT_SIZE, OPTION_TEXT_RIGHT_X_POS, (276 - (OPTION_FONT_SIZE)) + ((index - (selection - (visibleOptions-1))) * INCREMENT));
				else
					DrawFormattedText(option->GetText(true), Font::Body, 255,255,255,255, Alignment::Right, OPTION_FONT_SIZE, OPTION_TEXT_RIGHT_X_POS, (276 - (OPTION_FONT_SIZE)) + ((index - (selection - (visibleOptions-1))) * INCREMENT));
			}
		}

		// Draw checkmark
		bool drawCheckmark = (option->GetBoolPtr() != nullptr && *option->GetBoolPtr() == true);
		if (option->m_IsBoolOption) {
			if (drawInRange) {
				if (drawCheckmark)
					DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Body, 255,255,255,255, Alignment::Right, 22, OPTION_TEXT_RIGHT_X_POS, 253 + (index * INCREMENT));
				DrawFormattedText("<img src='img://generic_textures/tick_box' height='30' width='30'/>", Font::Body, 255,255,255,255, Alignment::Right, 22, OPTION_TEXT_RIGHT_X_POS, 253 + (index * INCREMENT));
			}
			else if (drawOutOfRange) {
				if (drawCheckmark)
					DrawFormattedText("<img src='img://generic_textures/tick' height='30' width='30'/>", Font::Body, 255,255,255,255, Alignment::Right, 22, OPTION_TEXT_RIGHT_X_POS, 253 + ((index - (selection - (visibleOptions-1))) * INCREMENT));
				DrawFormattedText("<img src='img://generic_textures/tick_box' height='30' width='30'/>", Font::Body, 255,255,255,255, Alignment::Right, 22, OPTION_TEXT_RIGHT_X_POS, 253 + ((index - (selection - (visibleOptions-1))) * INCREMENT));
			}
		}
	}


	void Drawing::DrawMenuTextures()
	{
		int numOptions = g_Menu->CurrentSubmenu->m_NumOptions;
		int visOptions = g_Menu->CurrentSubmenu->m_NumVisibleOptions;
		int selection = g_Menu->SelectionIndex;

		// Background
		DrawSprite("generic_textures", "inkroller_1a", BG_X_OFFSET, BG_Y_OFFSET, BG_WIDTH, BG_HEIGHT, 0, 0,0,0,230, false);
		
		// Header
		DrawSprite("generic_textures", "menu_header_1a", TOP_HEADER_X_POS, TOP_HEADER_Y_POS, TOP_HEADER_WIDTH, TOP_HEADER_HEIGHT, 0, 255,255,255,255, false);
		
		// Footer
		// TODO: Find the real texture
		DrawSprite("generic_textures", "menu_bar", FOOTER_LINE_X_POS, FOOTER_LINE_Y_POS, FOOTER_LINE_WIDTH, FOOTER_LINE_HEIGHT, 45, 255,255,255,175, true);
		
		//////////////////////
		// Scroller Sprites //
		//////////////////////
		
		// Top
		DrawSprite("menu_textures", "scroller_left_top", SCROLLER_LEFT_X_POS, 215, SCROLLER_LEFT_WIDTH, 25, 0, 255,255,255,255, false);
		DrawSprite("menu_textures", "scroller_right_top", SCROLLER_RIGHT_X_POS, 215, SCROLLER_RIGHT_WIDTH, 25, 0, 255,255,255,255, false);
		if (selection >= visOptions)
			DrawSprite("menu_textures", "scroller_arrow_top", SCROLLER_MIDDLE_X_POS, 227.5f, SCROLLER_MIDDLE_WIDTH, 25, 0, 255,255,255,255, true);
		else
			DrawSprite("menu_textures", "scroller_line_up", SCROLLER_MIDDLE_X_POS, 227.5f, SCROLLER_MIDDLE_WIDTH, 25, 0, 255,255,255,255, true);

		// Bottom
		if (numOptions <= visOptions) {
			DrawSprite("menu_textures", "scroller_left_bottom", SCROLLER_LEFT_X_POS, 244 + (numOptions * INCREMENT), SCROLLER_LEFT_WIDTH, 25, 0, 255,255,255,255, false);
			DrawSprite("menu_textures", "scroller_right_bottom", SCROLLER_RIGHT_X_POS, 244 + (numOptions * INCREMENT), SCROLLER_RIGHT_WIDTH, 25, 0, 255,255,255,255, false);
			DrawSprite("menu_textures", "scroller_line_down", SCROLLER_MIDDLE_X_POS, 256.5f + (numOptions * INCREMENT), SCROLLER_MIDDLE_WIDTH, 25, 0, 255,255,255,255, true);
		}
		else {
			DrawSprite("menu_textures", "scroller_left_bottom", SCROLLER_LEFT_X_POS, 244 + (visOptions * INCREMENT), SCROLLER_LEFT_WIDTH, 25, 0, 255,255,255,255, false);
			DrawSprite("menu_textures", "scroller_right_bottom", SCROLLER_RIGHT_X_POS, 244 + (visOptions * INCREMENT), SCROLLER_RIGHT_WIDTH, 25, 0, 255,255,255,255, false);
			if (selection == numOptions - 1) // We're at the very last option, don't draw the arrow
				DrawSprite("menu_textures", "scroller_line_down", SCROLLER_MIDDLE_X_POS, 256.5f + (visOptions * INCREMENT), SCROLLER_MIDDLE_WIDTH, 25, 0, 255,255,255,255, true);
			else
				DrawSprite("menu_textures", "scroller_arrow_bottom", SCROLLER_MIDDLE_X_POS, 256.5f + (visOptions * INCREMENT), SCROLLER_MIDDLE_WIDTH, 25, 0, 255,255,255,255, true);
		}
	}


	void Drawing::DrawSelectionBox()
	{
		// Couldn't add the corner textures because they get stretched out and
		// just don't look right, so the lines are sized to be touching each other.

		int selection = g_Menu->SelectionIndex;
		int visibleOptions = g_Menu->CurrentSubmenu->m_NumVisibleOptions;

		// Left, Right, Top, Bottom
		if (selection >= visibleOptions-1) {
			DrawSprite("menu_textures", "crafting_highlight_l", SELECTION_BOX_LEFT_X_POS,   270 + ((visibleOptions-1) * INCREMENT), 19, SELECTION_BOX_LEFT_HEIGHT, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_r", SELECTION_BOX_RIGHT_X_POS,  270 + ((visibleOptions-1) * INCREMENT), 19, SELECTION_BOX_RIGHT_HEIGHT, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_t", SELECTION_BOX_TOP_X_POS,    SELECTION_BOX_TOP_Y_POS + ((visibleOptions - 1) * INCREMENT), SELECTION_BOX_TOP_WIDTH, 22, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_b", SELECTION_BOX_BOTTOM_X_POS, SELECTION_BOX_BOTTOM_Y_POS + ((visibleOptions-1) * INCREMENT), SELECTION_BOX_BOTTOM_WIDTH, 22, 0, 255,0,0,255, true);
		} else {
			DrawSprite("menu_textures", "crafting_highlight_l", SELECTION_BOX_LEFT_X_POS,   270 + (selection * INCREMENT), 19, SELECTION_BOX_LEFT_HEIGHT, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_r", SELECTION_BOX_RIGHT_X_POS,  270 + (selection * INCREMENT), 19, SELECTION_BOX_RIGHT_HEIGHT, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_t", SELECTION_BOX_TOP_X_POS,    SELECTION_BOX_TOP_Y_POS + (selection * INCREMENT), SELECTION_BOX_TOP_WIDTH, 22, 0, 255,0,0,255, true);
			DrawSprite("menu_textures", "crafting_highlight_b", SELECTION_BOX_BOTTOM_X_POS, SELECTION_BOX_BOTTOM_Y_POS + (selection * INCREMENT), SELECTION_BOX_BOTTOM_WIDTH, 22, 0, 255,0,0,255, true);
		}
	}
}
