// Licensed under the MIT License.

#include <string>
#include "script.h"
#include "menu.h"

// This file is specifically for drawing and creating pages.
// Might be a good idea to separate pages using #region

void DrawPage()
{
	double pageIndex = *GetCurrentPageIndex();

	if (pageIndex == 0.0) {
		Header::SetHeader("HEADER");
		Header::SetSubHeader("Sub Header");
		// If you set bHasPage to true, make sure to create the page
		Draw::DrawOption("Option Examples", 0, true);
		Draw::DrawOption("Function Examples", 1, true);
		Draw::DrawOption("Page 3", 2, true);
		Draw::DrawOption("Page 4", 3, true);
		Draw::DrawOption("Page 5", 4, true);
		Draw::DrawOption("Page 6", 5, true);
		Draw::DrawOption("Page 7", 6, true);
		Draw::DrawOption("Page 8", 7, true);
	}

	#pragma region Page 1
	else if (pageIndex == 1.0) {
		Header::SetHeader("EXAMPLES");
		Header::SetSubHeader("Examples");
		Draw::DrawOption("Toggle Option Examples", 0, true);
		Draw::DrawOption("Example Button 1", 1);
		Draw::DrawOption("Example Button 2", 2);
	}

	else if (pageIndex == 1.1) {
		Header::SetHeader("TOGGLE EXAMPLES");
		Header::SetSubHeader("Toggle Option Examples");

		Draw::DrawOption("Toggle Option - Vector", 0, false, true);
		Menu::AddOptionsToToggle(0, std::vector<std::string>{"Red", "Dead", "Redemption", "2"});

		Draw::DrawOption("Toggle Option - Static", 1, false, true);
		Menu::AddOptionsToToggle(1, 10, "BaseText "); // This will create 10 options
	}
	#pragma endregion

	#pragma region Page 2
	else if (pageIndex == 2.0) {
		Header::SetHeader("FUNCTIONS");
		Header::SetSubHeader("Executing Functions");
		Draw::DrawOption("Change Weather", 0, true);
		// More to come maybe..?
	}

	else if (pageIndex == 2.1) {
		Header::SetHeader("WEATHER");
		Header::SetSubHeader("Change Weather");
		Draw::DrawOption("High Pressure", 0);
		Draw::DrawOption("Rain", 1);
		Draw::DrawOption("Snow", 2);
		Draw::DrawOption("Misty", 3);
		Draw::DrawOption("Fog", 4);
		Draw::DrawOption("Sunny", 5);
		Draw::DrawOption("Clouds", 6);
		Draw::DrawOption("Overcast", 7);
		Draw::DrawOption("Thunderstorm", 8);
		Draw::DrawOption("Hurricane", 9);
		Draw::DrawOption("Thunder", 10);
		Draw::DrawOption("Shower", 11);
		Draw::DrawOption("Blizzard", 12);
		Draw::DrawOption("Snow Light", 13);
		Draw::DrawOption("Whiteout", 14);
		Draw::DrawOption("Hail", 15);
		Draw::DrawOption("Sleet", 16);
		Draw::DrawOption("Drizzle", 17);
		Draw::DrawOption("Sandstorm", 18);
		Draw::DrawOption("Overcast Dark", 19);
		Draw::DrawOption("Ground Blizzard", 20);
	}
	#pragma endregion

	else if (pageIndex == 3.0) {
		Header::SetHeader("PAGE 3 HEADER");
		Header::SetSubHeader("Page 3 Sub Header");
		Draw::DrawOption("Page 3 - Option 1", 0);
		Draw::DrawOption("Page 3 - Option 2", 1);
		Draw::DrawOption("Page 3 - Option 3", 2);
	}

	else if (pageIndex == 4.0) {
		Header::SetHeader("PAGE 4 HEADER");
		Header::SetSubHeader("Page 4 Sub Header");
		Draw::DrawOption("Page 4 - Option 1", 0);
		Draw::DrawOption("Page 4 - Option 2", 1);
		Draw::DrawOption("Page 4 - Option 3", 2);
	}

	else if (pageIndex == 5.0) {
		Header::SetHeader("PAGE 5 HEADER");
		Header::SetSubHeader("Page 5 Sub Header");
		Draw::DrawOption("Page 5 - Option 1", 0);
		Draw::DrawOption("Page 5 - Option 2", 1);
		Draw::DrawOption("Page 5 - Option 3", 2);
	}

	else if (pageIndex == 6.0) {
		Header::SetHeader("PAGE 6 HEADER");
		Header::SetSubHeader("Page 6 Sub Header");
		Draw::DrawOption("Page 6 - Option 1", 0);
		Draw::DrawOption("Page 6 - Option 2", 1);
		Draw::DrawOption("Page 6 - Option 3", 2);
	}

	else if (pageIndex == 7.0) {
		Header::SetHeader("PAGE 7 HEADER");
		Header::SetSubHeader("Page 7 Sub Header");
		Draw::DrawOption("Page 7 - Option 1", 0);
		Draw::DrawOption("Page 7 - Option 2", 1);
		Draw::DrawOption("Page 7 - Option 3", 2);
	}

	else if (pageIndex == 8.0) {
		Header::SetHeader("PAGE 8 HEADER");
		Header::SetSubHeader("Page 8 Sub Header");
		Draw::DrawOption("Page 8 - Option 1", 0);
		Draw::DrawOption("Page 8 - Option 2", 1);
		Draw::DrawOption("Page 8 - Option 3", 2);
	}

	// Keep in mind that you can have more than 8 options in a page
}
