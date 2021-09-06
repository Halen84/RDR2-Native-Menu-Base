// Licensed under the MIT License.

#pragma once

#include <string>

// Draws a new page
void DrawPage(double pageIndex);

void OnSelect();

void OnToggle();

bool DoesOptionHavePage(double pageIndex, int selectedIndex);

bool DoesOptionHaveToggle(double pageIndex, int selectedIndex);

std::string GetTextState(double pageIndex, int selectedIndex);

int GetNumOptionsInCurrentPage();
