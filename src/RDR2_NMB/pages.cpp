// Licensed under the MIT License.

#include ".\Core\menu.h"
#include "functions.h"

using namespace Menu;

// This file is specifically for drawing and creating pages.
// Create functions for options in functions.cpp, or just use lambdas.
// 
// You don't HAVE to put all of your pages here. You can create separate files
// for each page if you want to, but just be sure to #include them


bool exampleBoolean = false;

void Page_WeatherOptions()
{
	Header::SetHeader("EXAMPLES");
	Header::SetSubHeader("Change Weather");
	New::RegularOption("High Pressure", "", &ChangeWeather);
	New::RegularOption("Rain", "", &ChangeWeather);
	New::RegularOption("Snow", "", &ChangeWeather);
	New::RegularOption("Misty", "", &ChangeWeather);
	New::RegularOption("Fog", "", &ChangeWeather);
	New::RegularOption("Sunny", "", &ChangeWeather);
	New::RegularOption("Clouds", "", &ChangeWeather);
	New::RegularOption("Overcast", "", &ChangeWeather);
	New::RegularOption("Thunderstorm", "", &ChangeWeather);
	New::RegularOption("Hurricane", "", &ChangeWeather);
	New::RegularOption("Thunder", "", &ChangeWeather);
	New::RegularOption("Shower", "", &ChangeWeather);
	New::RegularOption("Blizzard", "", &ChangeWeather);
	New::RegularOption("Snow Light", "", &ChangeWeather);
	New::RegularOption("Whiteout", "", &ChangeWeather);
	New::RegularOption("Hail", "", &ChangeWeather);
	New::RegularOption("Sleet", "", &ChangeWeather);
	New::RegularOption("Drizzle", "", &ChangeWeather);
	New::RegularOption("Sandstorm", "", &ChangeWeather);
	New::RegularOption("Overcast Dark", "", &ChangeWeather);
	New::RegularOption("Ground Blizzard", "", &ChangeWeather);

	// Change number of visible options so we can see more of them
	m_visibleOptionsInThisPage = MAX;
}
void Page_TimeOptions()
{
	Header::SetHeader("EXAMPLES");
	Header::SetSubHeader("Change Time");
	New::VectorOption("Hour", "", 24, "", "", &ChangeTime);
	Util::SetVectorSelection(m_pageIndex, 0, CLOCK::GET_CLOCK_HOURS());

	New::VectorOption("Minute", "", 60, "", "", &ChangeTime);
	Util::SetVectorSelection(m_pageIndex, 1, CLOCK::GET_CLOCK_MINUTES());

	New::VectorOption("Second", "", 60, "", "", &ChangeTime);
	Util::SetVectorSelection(m_pageIndex, 2, CLOCK::GET_CLOCK_SECONDS());
}


void Page_Examples()
{
	Header::SetHeader("EXAMPLES");
	Header::SetSubHeader("Function Examples");
	New::PageOption("Change Weather", "", 5.1, &Page_WeatherOptions);
	New::PageOption("Change Time", "", 5.2, &Page_TimeOptions);

	// As long as we reach this page, it will stay the same for the
	// rest of the pages unless specified if that makes sense.
	// So we don't have to specify this on every page.
	m_visibleOptionsInThisPage = DEFAULT;
}


// Note: This is a required function!
void __EntryPage__()
{
	Header::SetHeader("HEADER");
	Header::SetSubHeader("Sub Header");

	// You can also use lambdas for smaller functions, or even pages if you feel like it
	New::RegularOption("Regular Option", "Regular Option Example Button", [] {
		PrintSubtitle("Regular Option Pressed");
	});

	New::ToggleOption("Toggle Option", "Toggle Option Example Button", &exampleBoolean, true, [] {
		PrintSubtitle(exampleBoolean == true ? "On - true" : "Off - false");
	});

	New::VectorOption("Vector Option 1", "Vector Option Example", std::vector<std::string>{"Option 0", "Option 1", "Option 2"}, &VectorOption1);
	New::VectorOption("Vector Option 2", "\"Static\" Vector Option Example", 3, "", "", &VectorOption2);
	New::PageOption("Examples Page", "Some examples of executing functions", 5.0, &Page_Examples);
}
