// Licensed under the MIT License.

#include "drawing.h"
#include "UIUtil.h"


#pragma region UI Constants
#define COLOR_PURE_WHITE 255,255,255,255
#define COLOR_WHITE_SMOKE 245,245,245,255
#define COLOR_RED 204,0,0,255
#define HALF(x)((x) * 0.5f)

// Font Size Constants
struct uiFontSizes {
	static constexpr int Header = 45;
	static constexpr int SubHeader = 22;
	static constexpr int Option = 22;
	static constexpr int OptionCounter = 20;
	static constexpr int Footer = 18;
	static constexpr int EmptyOption = 22;
};

// Background Constants
struct uiBackground {
	static constexpr float PositionX = 25.0f;
	static constexpr float PositionY = 25.0f;
	static constexpr float Width = 575.0f;
	static constexpr float Height = 1025.0f;
	static constexpr float MiddleX = PositionX + HALF(Width); // The center X position for the background
	static constexpr float MiddleY = PositionY + HALF(Height); // The center Y position for the background
};

// Header Constants
struct uiHeader {
	static constexpr float Width = 445.0f;
	static constexpr float Height = 110.0f;
	static constexpr float PositionX = uiBackground::MiddleX - HALF(Width);
	static constexpr float PositionY = uiBackground::PositionY + ((uiBackground::Width - Width) * 0.2f);
	// What the fuck is this calculation
	static constexpr float TextPositionY = ((PositionY + HALF(Height)) - HALF(uiFontSizes::Header)) - (HALF(uiFontSizes::Header) * 0.1f);

	static constexpr float SubHeaderTextPositionY = (PositionY + Height) + HALF(uiFontSizes::SubHeader);
};

// Footer Constants
struct uiFooter {
	static constexpr float Width = uiHeader::Width;
	static constexpr float Height = 2.0f;
	static constexpr float PositionX = uiBackground::MiddleX;
	static constexpr float PositionY = HALF(uiBackground::PositionY) + (uiBackground::Height - HALF(uiBackground::Width - Width)) - HALF(Height);
	static constexpr float TextPositionY = (PositionY + Height) + HALF(uiFontSizes::Footer) - 5.0f;
};

// Scroller Constants
struct uiScroller {
	static constexpr float SizeOffset = 15.0f;
	static constexpr float LeftWidth = HALF(uiHeader::Width) - SizeOffset;
	static constexpr float LeftPositionX = (uiBackground::MiddleX - (LeftWidth + SizeOffset));
	static constexpr float RightWidth = LeftWidth;
	static constexpr float RightPositionX = uiBackground::MiddleX + SizeOffset;
	static constexpr float MiddleWidth = SizeOffset * 2.0f;
	static constexpr float MiddlePositionX = uiBackground::MiddleX;
};

// Option Constants
struct uiOption {
	static constexpr float Width = uiHeader::Width;
	static constexpr float Height = 50.0f;
	static constexpr float PositionX = uiBackground::MiddleX;
	static constexpr float TextOffset = 8.0f;
	static constexpr float LeftTextPositionX = (PositionX - HALF(Width)) + TextOffset; // X Position of text
	static constexpr float RightTextPositionX = (PositionX + HALF(Width)) - TextOffset; // X Position of Vector text

	static constexpr float PositionYStart = 269.0f; // Position at which the first option will be drawn at
	static constexpr int TextPositionYStart = 275; // Position at which the text of the first option will be drawn at

	static constexpr float EmptyOpionTextPositionX = uiBackground::MiddleX;
	static constexpr float OptionCounterPositionX = PositionX + HALF(Width);
};

// Selection Box Constants
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

// Checkbox Constants
struct uiCheckbox {
	static constexpr float Width = 30.0f;
	static constexpr float Height = Width;
	static constexpr float PositionX = uiBackground::MiddleX + HALF(uiOption::Width) - uiOption::TextOffset - HALF(Width) - 2.0f;
};

const float INCREMENT = (SCREEN_HEIGHT * 0.050f); // Y Position increment for option and sprite positions
#pragma endregion


const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };


// std::format is slow, so I just made my own
template<typename... Args>
void format(std::string& str, const Args&... args)
{
	for (const std::string& x : { args... }) {
		size_t pos = str.find("%s");
		if (pos != std::string::npos) {
			str.replace(pos, 2, x);
		}
	}
}

// Fix font size based on display size
int _actualScreenWidth = -1;
int calculateFontSize(int fontSize)
{
	if (_actualScreenWidth == -1) {
		_actualScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	}
	fontSize = (fontSize * UIUtil::g_screenWidth) / _actualScreenWidth;
	return fontSize;
}

inline void DrawHeader(const std::string& text)
{
	Drawing::DrawFormattedText(text, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::Header, uiBackground::MiddleX, uiHeader::TextPositionY);
}

inline void DrawSubHeader(const std::string& text)
{
	Drawing::DrawFormattedText(text, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::SubHeader, uiBackground::MiddleX, uiHeader::SubHeaderTextPositionY);
}

inline void DrawFooter(const std::string& text)
{
	Drawing::DrawFormattedText(text, Font::Body, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::Footer, uiBackground::MiddleX, uiFooter::TextPositionY);
}

void DrawOptionCounter()
{
	Submenu* sm = g_Menu->GetCurrentSubmenu();
	int numOptions = sm->GetNumberOfOptions();
	if (numOptions >= sm->NumberOfVisibleOptions) {
		Drawing::DrawFormattedText(std::to_string(g_Menu->GetSelectionIndex() + 1) + " of " + std::to_string(numOptions), Font::Body, 144, 144, 144, 230, Alignment::Right, uiFontSizes::OptionCounter, uiOption::OptionCounterPositionX, 243.0f + (sm->NumberOfVisibleOptions * INCREMENT), 0, -1);
	}
	else {
		Drawing::DrawFormattedText(std::to_string(g_Menu->GetSelectionIndex() + 1) + " of " + std::to_string(numOptions), Font::Body, 144,144,144,230, Alignment::Right, uiFontSizes::OptionCounter, uiOption::OptionCounterPositionX, 243.0f + (numOptions * INCREMENT), 0, -1);
	}
}


void drawOptionInternal(Option* pOption, bool drawInRange, bool drawOutOfRange, float inRangeIncr, float outOfRangeIncr)
{
	if (drawInRange)
	{
		if (!pOption->IsEmptyOption) {
			Drawing::DrawSprite("generic_textures", "selection_box_bg_1c", uiOption::PositionX, uiOption::PositionYStart + inRangeIncr, uiOption::Width, uiOption::Height, 0, 50,50,50,110, true);
			Drawing::DrawFormattedText(pOption->Text, Font::Body, COLOR_WHITE_SMOKE, Alignment::Left, uiFontSizes::Option, uiOption::LeftTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + inRangeIncr);
		}
		else {
			Drawing::DrawFormattedText(pOption->Text, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::EmptyOption, uiOption::EmptyOpionTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::EmptyOption) + inRangeIncr);
		}
	}
	else if (drawOutOfRange)
	{
		if (!pOption->IsEmptyOption) {
			Drawing::DrawSprite("generic_textures", "selection_box_bg_1c", uiOption::PositionX, uiOption::PositionYStart + outOfRangeIncr, uiOption::Width, uiOption::Height, 0, 50,50,50,110, true);
			Drawing::DrawFormattedText(pOption->Text, Font::Body, COLOR_WHITE_SMOKE, Alignment::Left, uiFontSizes::Option, uiOption::LeftTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + outOfRangeIncr);
		}
		else {
			Drawing::DrawFormattedText(pOption->Text, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::EmptyOption, uiOption::EmptyOpionTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::EmptyOption) + outOfRangeIncr);
		}
	}
}


void drawVectorOptionInternal(Option* pOption, bool drawInRange, bool drawOutOfRange, float inRangeIncr, float outOfRangeIncr)
{
	VectorOption* option = pOption->As<VectorOption*>();
	int index = option->Index;
	int selection = g_Menu->GetSelectionIndex();

	// Draw the option
	drawOptionInternal(option, drawInRange, drawOutOfRange, inRangeIncr, outOfRangeIncr);

	// Draw the right text and arrows
	if (drawInRange)
	{
		if (selection == index) {
			// TODO: This fucks up if the screen res is anything but 1920x1080, because of the img src thing.
			// Need to convert to use Drawing::DrawSprite() somehow...
			Drawing::DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->RightText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, COLOR_PURE_WHITE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + inRangeIncr);
		}
		else {
			Drawing::DrawFormattedText(option->RightText, Font::Body, COLOR_WHITE_SMOKE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + inRangeIncr);
		}
	}
	else if (drawOutOfRange)
	{
		if (selection == index) {
			// TODO: This fucks up if the screen res is anything but 1920x1080, because of the img src thing.
			// Need to convert to use Drawing::DrawSprite() somehow...
			Drawing::DrawFormattedText("<img src='img://menu_textures/selection_arrow_left' height='18' width='18'/> " + option->RightText + " <img src='img://menu_textures/selection_arrow_right' height='18' width='18'/>", Font::Body, COLOR_PURE_WHITE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + outOfRangeIncr);
		}
		else {
			Drawing::DrawFormattedText(option->RightText, Font::Body, COLOR_WHITE_SMOKE, Alignment::Right, uiFontSizes::Option, uiOption::RightTextPositionX, (uiOption::TextPositionYStart - uiFontSizes::Option) + outOfRangeIncr);
		}
	}
}


void drawBoolOptionInternal(Option* pOption, bool drawInRange, bool drawOutOfRange, float inRangeIncr, float outOfRangeIncr)
{
	BoolOption* option = pOption->As<BoolOption*>();
	bool drawCheckmark = (option->pBoolPointer != nullptr && *option->pBoolPointer == true);

	// Draw the option
	drawOptionInternal(option, drawInRange, drawOutOfRange, inRangeIncr, outOfRangeIncr);

	// Draw checkmark and checkbox
	if (drawInRange)
	{
		if (drawCheckmark) {
			Drawing::DrawSprite("generic_textures", "tick", uiCheckbox::PositionX, uiOption::PositionYStart + inRangeIncr, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
		}
		Drawing::DrawSprite("generic_textures", "tick_box", uiCheckbox::PositionX, uiOption::PositionYStart + inRangeIncr, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
	}
	else if (drawOutOfRange)
	{
		if (drawCheckmark) {
			Drawing::DrawSprite("generic_textures", "tick", uiCheckbox::PositionX, uiOption::PositionYStart + outOfRangeIncr, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
		}
		Drawing::DrawSprite("generic_textures", "tick_box", uiCheckbox::PositionX, uiOption::PositionYStart + outOfRangeIncr, uiCheckbox::Width, uiCheckbox::Height, 0, COLOR_PURE_WHITE, true);
	}
}


void Drawing::DrawFormattedText(const std::string& text, Font font, int red, int green, int blue, int alpha, Alignment align, int textSize, float posX, float posY, int wrapWidth, int letterSpacing)
{
	textSize = calculateFontSize(textSize);

	const std::string _font = FontList[(int)font];

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

	std::string formatText = "<TEXTFORMAT RIGHTMARGIN='%s'><P ALIGN='%s'><FONT FACE='$%s' LETTERSPACING='%s' SIZE='%s'>~s~%s</FONT></P><TEXTFORMAT>";
	// BASELINE, BLOCKINDENT, CLASS, HEIGHT, WIDTH, HREF, HSPACE, VSPACE, ID, INDENT, KERNING, LEADING, LEFTMARGIN, SRC, TABSTOPS, TARGET, COLOR

	format(formatText, _rightMargin, _align, _font, _letterSpacing, _size, text);

#if BUILD_1311_COMPATIBLE
	HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", formatText.c_str()), x, y);
#else
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", formatText.c_str()), x, y);
#endif
}


void Drawing::DrawOption(Option* option)
{
	int selection = g_Menu->GetSelectionIndex();
	int visibleOptions = g_Menu->GetCurrentSubmenu()->NumberOfVisibleOptions;
	int index = option->Index;

	// Create variables here so they aren't unnecessarily recalculated in the draw internal functions
	bool drawInRange = (selection <= visibleOptions - 1 && index <= visibleOptions - 1);
	bool drawOutOfRange = ((index > (selection - visibleOptions)) && index <= selection);
	float inRangeIncr = (index * INCREMENT);
	float outOfRangeIncr = ((index - (selection - (visibleOptions - 1))) * INCREMENT);

	if (option->IsRegularOption || option->IsSubmenuOption || option->IsEmptyOption) {
		drawOptionInternal(option, drawInRange, drawOutOfRange, inRangeIncr, outOfRangeIncr);
	}

	if (option->IsVectorOption) {
		drawVectorOptionInternal(option, drawInRange, drawOutOfRange, inRangeIncr, outOfRangeIncr);
	}

	if (option->IsBoolOption) {
		drawBoolOptionInternal(option, drawInRange, drawOutOfRange, inRangeIncr, outOfRangeIncr);
	}

	// Draw the option's footer text
	if (selection == index && !option->IsEmptyOption) {
		DrawFooter(option->Footer);
	}
}


void DrawBackground()
{
	int numOptions = g_Menu->GetCurrentSubmenu()->GetNumberOfOptions();
	int visOptions = g_Menu->GetCurrentSubmenu()->NumberOfVisibleOptions;
	int selection = g_Menu->GetSelectionIndex();

	// Background
	Drawing::DrawSprite("generic_textures", "inkroller_1a", uiBackground::PositionX, uiBackground::PositionY, uiBackground::Width, uiBackground::Height, 0, 0,0,0,230, false);

	// Header
	Drawing::DrawSprite("generic_textures", "menu_header_1a", uiHeader::PositionX, uiHeader::PositionY, uiHeader::Width, uiHeader::Height, 0, COLOR_PURE_WHITE, false);

	// Footer
	Drawing::DrawSprite("generic_textures", "menu_bar", uiFooter::PositionX, uiFooter::PositionY, uiFooter::Width, uiFooter::Height, 0, 255, 255, 255, 175, true);

	//
	// Scroller Sprites
	//

	// Top
	Drawing::DrawSprite("menu_textures", "scroller_left_top", uiScroller::LeftPositionX, 215, uiScroller::LeftWidth, 25, 0, COLOR_PURE_WHITE, false);
	Drawing::DrawSprite("menu_textures", "scroller_right_top", uiScroller::RightPositionX, 215, uiScroller::RightWidth, 25, 0, COLOR_PURE_WHITE, false);
	if (selection >= visOptions) {
		Drawing::DrawSprite("menu_textures", "scroller_arrow_top", uiScroller::MiddlePositionX, 227.5f, uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
	}
	else {
		Drawing::DrawSprite("menu_textures", "scroller_line_up", uiScroller::MiddlePositionX, 227.5f, uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
	}

	// Bottom
	if (numOptions <= visOptions) {
		Drawing::DrawSprite("menu_textures", "scroller_left_bottom", uiScroller::LeftPositionX, 244 + (numOptions * INCREMENT), uiScroller::LeftWidth, 25, 0, COLOR_PURE_WHITE, false);
		Drawing::DrawSprite("menu_textures", "scroller_right_bottom", uiScroller::RightPositionX, 244 + (numOptions * INCREMENT), uiScroller::RightWidth, 25, 0, COLOR_PURE_WHITE, false);
		Drawing::DrawSprite("menu_textures", "scroller_line_down", uiScroller::MiddlePositionX, 256.5f + (numOptions * INCREMENT), uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
	}
	else {
		Drawing::DrawSprite("menu_textures", "scroller_left_bottom", uiScroller::LeftPositionX, 244 + (visOptions * INCREMENT), uiScroller::LeftWidth, 25, 0, COLOR_PURE_WHITE, false);
		Drawing::DrawSprite("menu_textures", "scroller_right_bottom", uiScroller::RightPositionX, 244 + (visOptions * INCREMENT), uiScroller::RightWidth, 25, 0, COLOR_PURE_WHITE, false);
		if (selection == numOptions - 1) {
			Drawing::DrawSprite("menu_textures", "scroller_line_down", uiScroller::MiddlePositionX, 256.5f + (visOptions * INCREMENT), uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
		}
		else {
			Drawing::DrawSprite("menu_textures", "scroller_arrow_bottom", uiScroller::MiddlePositionX, 256.5f + (visOptions * INCREMENT), uiScroller::MiddleWidth, 25, 0, COLOR_PURE_WHITE, true);
		}
	}
}


void DrawSelectionBox()
{
	// Couldn't add the corner textures because they get stretched out and
	// just don't look right, so the lines are sized to be touching each other.

	int visibleOptions = g_Menu->GetCurrentSubmenu()->NumberOfVisibleOptions;
	int selection = g_Menu->GetSelectionIndex();

	// Left, Right, Top, Bottom
	if (selection >= visibleOptions-1) {
		Drawing::DrawSprite("menu_textures", "crafting_highlight_l", uiSelectionBox::LeftPositionX,   uiOption::PositionYStart + ((visibleOptions-1) * INCREMENT), 19, uiSelectionBox::LeftHeight, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_r", uiSelectionBox::RightPositionX,  uiOption::PositionYStart + ((visibleOptions-1) * INCREMENT), 19, uiSelectionBox::RightHeight, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_t", uiSelectionBox::TopPositionX,    uiSelectionBox::TopPositionY + ((visibleOptions - 1) * INCREMENT), uiSelectionBox::TopWidth, 22, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_b", uiSelectionBox::BottomPositionX, uiSelectionBox::BottomPositionY + ((visibleOptions-1) * INCREMENT), uiSelectionBox::BottomWidth, 22, 0, COLOR_RED, true);
	}
	else {
		Drawing::DrawSprite("menu_textures", "crafting_highlight_l", uiSelectionBox::LeftPositionX,   uiOption::PositionYStart + (selection * INCREMENT), 19, uiSelectionBox::LeftHeight, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_r", uiSelectionBox::RightPositionX,  uiOption::PositionYStart + (selection * INCREMENT), 19, uiSelectionBox::RightHeight, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_t", uiSelectionBox::TopPositionX,    uiSelectionBox::TopPositionY + (selection * INCREMENT), uiSelectionBox::TopWidth, 22, 0, COLOR_RED, true);
		Drawing::DrawSprite("menu_textures", "crafting_highlight_b", uiSelectionBox::BottomPositionX, uiSelectionBox::BottomPositionY + (selection * INCREMENT), uiSelectionBox::BottomWidth, 22, 0, COLOR_RED, true);
	}
}


void Drawing::DrawMenu()
{
	DrawBackground();
	DrawSelectionBox();
	DrawHeader(g_Menu->GetCurrentSubmenu()->Header);
	DrawSubHeader(g_Menu->GetCurrentSubmenu()->SubHeader);
	DrawOptionCounter();
}
