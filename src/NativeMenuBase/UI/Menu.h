// Licensed under the MIT License.

#pragma once
#include "../script.h"
#include "../common.h"

enum class SubmenuId : int
{
	Root,
	Submenu1,
	Submenu2,
	Submenu3,
};

struct Page
{
	int ID = 0;
	int Cursor = 0;
	int Scroll = 0;
	int OptionCount = 0;

	explicit Page(int id) : ID(id) {}
};

enum class OptionKind
{
	Regular,
	Submenu,
	Checkbox,
	List
};

class CMenu
{
public:
	static void Open();
	static void Close();
	static void Reset();
	static bool IsOpen();

	static Page& GetCurrentPage();

	static void BeginFrame();
	static void EndFrame();
	static bool BeginSubmenu(const char* label, const char* footer, const char* header, const char* subHeader, SubmenuId id);
	static void EndSubmenu();
	static bool Option(const char* label, const char* footer);
	static bool CheckboxOption(const char* label, const char* footer, bool* toggle);
	static bool ListOption(const char* label, const char* footer, int* currentItem, const char* const* items, int itemCount);
	static void Separator(const char* label = "");
};
