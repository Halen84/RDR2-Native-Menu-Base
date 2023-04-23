// Licensed under the MIT License.

#pragma once
#include <string>
#include <functional>
#include <vector>

class Option
{
private:
	bool bWasJustExecuted = false;
	bool bInitialTextSet = false;

	std::function<void()> mFunc = nullptr;
public:
	std::string Text = "";
	std::string Footer = "";

	bool IsRegularOption = false;
	bool IsBoolOption = false;
	bool IsVectorOption = false;
	bool IsSubmenuOption = false;
	bool IsEmptyOption = false;

	int Index = 0;

	Option() = default;
	virtual ~Option() = default;

	// Set the function of this option that will be executed when clicked
	void SetFunction(const std::function<void()> &func)
	{
		mFunc = func;
	}

	// Convert between option types. MAKE SURE TO CHECK OPTION TYPE BEFORE USING.
	template<class T>
	std::enable_if_t<std::is_pointer_v<T>, T> As()
	{
		return dynamic_cast<T>(this);
	}

	// Execute the assigned function
	void Execute()
	{
		if (mFunc != nullptr) {
			bWasJustExecuted = true;
			mFunc();
			bWasJustExecuted = false;
		}
	}

	// Get if this option was just executed/pressed
	bool WasJustExecuted()	{ return bWasJustExecuted; }

	// Get if this option was just executed/pressed
	bool WasJustPressed()	{ return bWasJustExecuted; }
};
