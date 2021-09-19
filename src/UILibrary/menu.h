// Licensed under the MIT License.

#pragma once

#include <string>
#include "script.h"

// Draws CSS styled text
void DrawCSSText(std::string text, std::string font, std::string color, std::string align, int leading, int rightMargin, int textSize, float X, float Y);
void DrawListOption(std::string text, int index);
// On & Off Toggle Option
void DrawToggleOption(std::string text, int index);
// Draws a selection box around currently selected item
void DrawSelectionBox();

// Creates a new UI prompt
void CreateControlAction(Prompt& prompt, Hash control, const char* text);

void SetHeader(std::string text, int fontSize = 45);
void SetSubHeader(std::string text);
void SetFooter(std::string text);

// OBJ hint
void ShowSubtitle(const std::string& str);
