// Licensed under the MIT License.

#include <map>
#include <string>
#include "menu.h"

void UpdateFooter(bool override)
{
	double pageIndex = GetCurrentPageIndex();
	int selectedIndex = GetCurrentSelectedIndex();

	static std::map<const double, std::map<const int, const std::string>> footerData
	{
		// Main Menu
		{0.0, {
			{0, "Page 1"}, {1, "Page 2"}, {2, "Page 3"}, {3, "Page 4"}, {4, "Page 5"}, {5, "Page 6"}, {6, "Page 7"}, {7, "Page 8"}
		}},

		// Page 1
		{1.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"} // Based on total options in this page
		}},

		// Page 2
		{2.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{3.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{4.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{5.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{6.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{7.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},

		{8.0, {
			{0, "Description 1"}, {1, "Description 2"}, {2, "Description 3"}
		}},
	};

	// Add 3 spaces after a newline \n to center properly?
	if (!override) {
		SetFooter(footerData[pageIndex][selectedIndex]);
	} else {
		SetFooter("Override Footer"); // This footer will always be shown if override = true
	}
}
