// Licensed under the MIT License.

#include <string>
#include ".\Core\menu.h"
#include "functions.h"

// This file is specifically for drawing and creating pages.
// Create your functions for options in functions.cpp.
// 
// You don't HAVE to put all of your pages here. You can create separate files
// for each page if you want to, but just be sure to #include them


bool exampleBoolean = false;
void ExampleToggleChange()
{
	PrintSubtitle(exampleBoolean == true ? "true" : "false");
}


void WeatherPage()
{
	Header::SetHeader("WEATHER");
	Header::SetSubHeader("Change Weather");
	MenuCreate::RegularOption("High Pressure", "", &ChangeWeather);
	MenuCreate::RegularOption("Rain", "", &ChangeWeather);
	MenuCreate::RegularOption("Snow", "", &ChangeWeather);
	MenuCreate::RegularOption("Misty", "", &ChangeWeather);
	MenuCreate::RegularOption("Fog", "", &ChangeWeather);
	MenuCreate::RegularOption("Sunny", "", &ChangeWeather);
	MenuCreate::RegularOption("Clouds", "", &ChangeWeather);
	MenuCreate::RegularOption("Overcast", "", &ChangeWeather);
	MenuCreate::RegularOption("Thunderstorm", "", &ChangeWeather);
	MenuCreate::RegularOption("Hurricane", "", &ChangeWeather);
	MenuCreate::RegularOption("Thunder", "", &ChangeWeather);
	MenuCreate::RegularOption("Shower", "", &ChangeWeather);
	MenuCreate::RegularOption("Blizzard", "", &ChangeWeather);
	MenuCreate::RegularOption("Snow Light", "", &ChangeWeather);
	MenuCreate::RegularOption("Whiteout", "", &ChangeWeather);
	MenuCreate::RegularOption("Hail", "", &ChangeWeather);
	MenuCreate::RegularOption("Sleet", "", &ChangeWeather);
	MenuCreate::RegularOption("Drizzle", "", &ChangeWeather);
	MenuCreate::RegularOption("Sandstorm", "", &ChangeWeather);
	MenuCreate::RegularOption("Overcast Dark", "", &ChangeWeather);
	MenuCreate::RegularOption("Ground Blizzard", "", &ChangeWeather);
}
void TimePage()
{
	Header::SetHeader("TIME");
	Header::SetSubHeader("Change Time");
	MenuCreate::VectorOption("Hour", "", 24, "", "", &ChangeTime);
	Menu::SetVectorSelection(*GetCurrentPageIndex(), 0, CLOCK::GET_CLOCK_HOURS());

	MenuCreate::VectorOption("Minute", "", 60, "", "", &ChangeTime);
	Menu::SetVectorSelection(*GetCurrentPageIndex(), 1, CLOCK::GET_CLOCK_MINUTES());

	MenuCreate::VectorOption("Second", "", 60, "", "", &ChangeTime);
	Menu::SetVectorSelection(*GetCurrentPageIndex(), 2, CLOCK::GET_CLOCK_SECONDS());
}


void ExamplesPage()
{
	Header::SetHeader("EXAMPLES");
	Header::SetSubHeader("Function Examples");
	MenuCreate::PageOption("Change Weather", "", 5.1, &WeatherPage); // 5.1 because it's the 1st option in page 5.0
	MenuCreate::PageOption("Change Time", "", 5.2, &TimePage);
}


// Note: This is a required function!
void __DrawEntryPage__()
{
	Header::SetHeader("HEADER");
	Header::SetSubHeader("Sub Header");

	MenuCreate::RegularOption("Regular Option", "Regular Option Example Button", &RegularButtonExample);
	MenuCreate::ToggleOption("Toggle Option", "Toggle Option Example Button", &exampleBoolean, true, &ExampleToggleChange);
	MenuCreate::VectorOption("Vector Option 1", "Vector Option Example", std::vector<std::string>{"Option 0", "Option 1", "Option 2"}, &VectorOption1);
	MenuCreate::VectorOption("Vector Option 2", "\"Static\" Vector Option Example", 3, "", "", &VectorOption2);
	MenuCreate::PageOption("Examples Page", "Some examples of executing functions", 5.0, &ExamplesPage); // 5.0 because it's the 5th option (1 based for page 0.0)
}
