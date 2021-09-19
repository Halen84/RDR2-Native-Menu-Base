// Licensed under the MIT License.

#pragma once

#include <string>

// Draws a new page
void DrawPage(double pageIndex);

void OnSelect();

void OnToggle(bool left, bool right);

// Add 1 option to a toggle (Useful if you want them to have unique text)
void AddSingleOption(std::string text, int index);

// Add multiple options to a toggle
void AddMultipleOptions(std::string baseText, int index, int numberOfOptions);

std::string GetText(int index);

bool DoesOptionHavePage(double pageIndex, int selectedIndex);
bool DoesOptionHaveToggle(double pageIndex, int selectedIndex);
int GetToggleSelectionIndex(double pageIndex, int index);
int GetNumOptionsInCurrentPage();
