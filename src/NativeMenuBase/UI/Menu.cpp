// Licensed under the MIT License.

#include "Menu.h"
#include "Drawing.h"
#include "../common.h"

static void PlaySoundFrontend(const char* name, const char* soundset)
{
	AUDIO::_STOP_SOUND_WITH_NAME(name, soundset);
	AUDIO::PLAY_SOUND_FRONTEND(name, soundset, true, 0);
}

static constexpr int MAX_VISIBLE = 8;

static bool s_IsOpen = false;
static std::vector<Page> s_Path = {}; // Open pages, root first. Empty = menu closed
static int s_PathDepth = 0;
static int s_OptionCount = 0;
static int s_NextId = 0;

static struct { bool up, down, left, right, select, back; } s_Input;
static enum class Nav { None, Push, Pop } s_Nav;

// Cheesy way of handling separator options...
static std::vector<u8> s_IsSep;      // This frame values
static std::vector<u8> s_PrevIsSep;  // Last frame values, used for cursor skipping
static const char* s_HoverFooter = "";

struct OptionResult
{
	bool hovered = false;
	bool selected = false;
	bool left = false;
	bool right = false;
};

static bool OnVisiblePage() { return s_IsOpen && s_PathDepth == (int)s_Path.size(); }

// +1 down / -1 up
static void MoveCursor(Page& pg, int dir)
{
	const int count = pg.OptionCount;
	if (count <= 0) return;

	if (dir == -1)
		PlaySoundFrontend("NAV_UP", "PAUSE_MENU_SOUNDSET");
	else
		PlaySoundFrontend("NAV_DOWN", "PAUSE_MENU_SOUNDSET");

	for (int i = 0; i < count; i++) {
		pg.Cursor = (pg.Cursor + dir + count) % count; // Wrap around
		bool separator = pg.Cursor < (int)s_PrevIsSep.size() && s_PrevIsSep[pg.Cursor];
		if (!separator) break;
	}

	// Keep cursor inside the scrollable view
	if (pg.Cursor < pg.Scroll)
		pg.Scroll = pg.Cursor;

	if (pg.Cursor >= pg.Scroll + MAX_VISIBLE)
		pg.Scroll = pg.Cursor - MAX_VISIBLE + 1;
}

static void EndPage()
{
	Page& pg = CMenu::GetCurrentPage();

	// Update count from last frame to next frame
	pg.OptionCount = s_OptionCount;

	// Clamp the cursor if options disappeared since last frame
	if (pg.OptionCount > 0)
		pg.Cursor = min(pg.Cursor, pg.OptionCount - 1);
	else
		pg.Cursor = 0;
}

static OptionResult BeginOption(const char* label, const char* footer, OptionKind kind, void* extradata = nullptr)
{
	Page& pg = CMenu::GetCurrentPage();
	int idx = s_OptionCount++;
	OptionResult r{};
	r.hovered = (idx == pg.Cursor);

	if (r.hovered) {
		s_HoverFooter = footer;
		r.selected = s_Input.select;
		r.left = s_Input.left && kind == OptionKind::List; 
		r.right = s_Input.right && kind == OptionKind::List;
		s_Input.select = false;
	}
	
	// Only draw options inside the scrollable view
	if (idx >= pg.Scroll && idx < pg.Scroll + MAX_VISIBLE) {
		CDrawing::DrawOption(idx - pg.Scroll, label, footer, r.hovered, kind, extradata);
	}

	return r;
}

void CMenu::Open()
{
	if (s_Path.empty()) {
		s_Path.emplace_back((int)SubmenuId::Root);
	}
	s_IsOpen = true;
	PlaySoundFrontend("MENU_ENTER", "HUD_PLAYER_MENU");
}
void CMenu::Close()  { s_IsOpen = false; PlaySoundFrontend("MENU_CLOSE", "HUD_PLAYER_MENU"); }
void CMenu::Reset()  { s_IsOpen = false; s_Path.clear(); }
bool CMenu::IsOpen() { return s_IsOpen; }
Page& CMenu::GetCurrentPage() { return s_Path.back(); }

void CMenu::BeginFrame()
{
	bool up, down, left, right, select, back, openKeyPressed;
	up     = PAD::IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_UP);
	down   = PAD::IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_DOWN);
	left   = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_LEFT);
	right  = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_RIGHT);
	select = PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_ACCEPT);
	back   = PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_CANCEL);
	openKeyPressed = IsKeyJustUp(VK_F5) || PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) && PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A);

	if (openKeyPressed)
	{
		if (CMenu::IsOpen())
			CMenu::Close();
		else
			CMenu::Open();
	}

	s_Input = { up, down, left, right, select, back };
	s_PathDepth = 0;
	s_OptionCount = 0;
	s_HoverFooter = nullptr;
	s_Nav = Nav::None;
	s_IsSep.clear();

	if (!s_IsOpen) return;

	Page& pg = GetCurrentPage();
	if (s_Input.up)     MoveCursor(pg, -1);
	if (s_Input.down)   MoveCursor(pg, +1);
	if (s_Input.select) PlaySoundFrontend("SELECT", "HUD_SHOP_SOUNDSET");
	if (s_Input.back) {
		s_Nav = Nav::Pop;
		PlaySoundFrontend("BACK", "HUD_SHOP_SOUNDSET");
	}
}

void CMenu::EndFrame()
{
	s_PrevIsSep = s_IsSep;

	switch (s_Nav)
	{
		case Nav::Push: s_Path.emplace_back(s_NextId); break;
		case Nav::Pop:
			s_Path.pop_back();
			if (s_Path.empty()) Close();
			break;
		default: break;
	}
}

bool CMenu::BeginSubmenu(const char* label, const char* footer, const char* header, const char* subHeader, SubmenuId id)
{
	if (!s_IsOpen) return false;

	const int top = (int)s_Path.size();

	// Inside an ancestor of the visible page: just follow the path down
	if (s_PathDepth < top)
	{
		if (s_Path[s_PathDepth].ID != (int)id) return false;
		s_PathDepth++;
		if (s_PathDepth == top) CDrawing::DrawMenu(header, subHeader);
		return true;
	}

	// Inside the visible page: this call is an option that leads to a child
	OptionResult row = BeginOption(label, footer, OptionKind::Submenu);
	if (row.selected) {
		s_Nav = Nav::Push;
		s_NextId = (int)id;
	}

	return false;
}

void CMenu::EndSubmenu()
{
	if (s_PathDepth == (int)s_Path.size())
		EndPage();
	s_PathDepth--;
}

bool CMenu::Option(const char* label, const char* footer)
{
	if (!OnVisiblePage()) return false;
	s_IsSep.push_back(false);
	OptionResult r = BeginOption(label, footer, OptionKind::Regular);
	return r.selected;
}

bool CMenu::CheckboxOption(const char* label, const char* footer, bool* toggle)
{
	if (!OnVisiblePage()) return false;
	s_IsSep.push_back(false);
	OptionResult r = BeginOption(label, footer, OptionKind::Checkbox, toggle);
	if (toggle != nullptr && r.selected) {
		*toggle ^= true;
	}
	return r.selected;
}

bool CMenu::ListOption(const char* label, const char* footer, int* currentItem, const char* const* items, int itemCount)
{
	if (!OnVisiblePage() || !currentItem || itemCount <= 0) return false;
	s_IsSep.push_back(false);
	OptionResult r = BeginOption(label, footer, OptionKind::List, (void*)items[*currentItem]);

	if (r.left)
		PlaySoundFrontend("NAV_LEFT", "PAUSE_MENU_SOUNDSET");
	else if (r.right)
		PlaySoundFrontend("NAV_RIGHT", "PAUSE_MENU_SOUNDSET");

	int old = *currentItem;
	if (r.left)
		*currentItem = (*currentItem - 1 + itemCount) % itemCount;
	if (r.right || r.selected)
		*currentItem = (*currentItem + 1) % itemCount;
	
	return *currentItem != old;   
}

void CMenu::Separator(const char* label)
{
	if (!OnVisiblePage()) return;
	s_IsSep.push_back(true);
	Page& pg = GetCurrentPage();
	int idx = s_OptionCount++;
	if (idx >= pg.Scroll && idx < pg.Scroll + MAX_VISIBLE) {
		int slot = idx - pg.Scroll;
		float y = (slot * (SCREEN_HEIGHT * 0.05f)) + uiOption::PositionYStart;
		float textY = y - (uiOption::Height * 0.5f) + (uiFontSizes::Option * 0.5f);
		CDrawing::Text(label, Font::Title, COLOR_WHITE_SMOKE, Alignment::Center, uiFontSizes::Separator, uiOption::SeparatorTextPositionX, textY);
	}
}
