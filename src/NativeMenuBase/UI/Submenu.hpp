// Licensed under the MIT License.

#pragma once
#include "Menu.hpp"
#include "Options/Option.h"
#include "Options/RegularOption.h"
#include "Options/BoolOption.h"
#include "Options/VectorOption.h"
#include "Options/SubmenuOption.h"
#include "Options/EmptyOption.h"
#include "../console.h"


class Submenu
{
protected:
	std::vector<std::unique_ptr<Option>> mOptions{};

private:
	std::function<void(Submenu*)> m_StructureFunc; // Function that sets the submenu layout

	template<class T>
	std::enable_if_t<std::is_pointer_v<T>, T> GetBackOption()
	{
		return mOptions.back().get()->As<T>();
	}

public:
	std::string Header = "";
	std::string SubHeader = "";
	eSubmenuID ID = eSubmenuID::Submenu_Invalid;
	int NumberOfVisibleOptions = 8; // Recommended: 8, Recommended Max: 12


	Submenu() = default;
	Submenu(const std::string& header, const std::string& subHeader, eSubmenuID id, int numVisibleOptions, std::function<void(Submenu*)> &submenuFunc)
	{
		Header = header;
		SubHeader = subHeader;
		ID = id;
		NumberOfVisibleOptions = numVisibleOptions;
		m_StructureFunc = submenuFunc;
		std::invoke(m_StructureFunc, static_cast<Submenu*>(this)); // Create the submenu
	}
	~Submenu()
	{
		mOptions.clear(); // unique_ptr objects are deleted here
	}


	/// <summary>
	/// An option with only one purpose: to execute a function when clicked.
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	//template <typename Function>
	RegularOption* AddRegularOption(const std::string& text, const std::string& footer, std::function<void()> func = [] {})
	{
		RegularOption option((int)mOptions.size());
		option.Text = text;
		option.Footer = footer;
		option.SetFunction(func);
		mOptions.push_back(std::make_unique<RegularOption>(option));
		return this->GetBackOption<RegularOption*>();
	}

	/// <summary>
	/// Option that acts as a on/off (true/false) switch
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="pbVariable">- Boolean pointer to toggle</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	BoolOption* AddBoolOption(const std::string& text, const std::string& footer, bool* pBoolVariable, std::function<void()> func = [] {})
	{
		BoolOption option((int)mOptions.size());
		option.Text = text;
		option.Footer = footer;
		option.SetFunction(func);
		option.pBoolPointer = pBoolVariable;
		mOptions.push_back(std::make_unique<BoolOption>(option));
		return this->GetBackOption<BoolOption*>();
	}

	/// <summary>
	/// An option with left and right arrows with multiple selections
	/// </summary>
	/// <typeparam name="T">Vector Data Type</typeparam>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="vec">- The std::vector</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	template<typename T>
	VectorOption* AddVectorOption(const std::string& text, const std::string& footer, std::vector<T> vec, std::function<void()> func = [] {})
	{
		VectorOption option((int)mOptions.size());
		option.Text = text;
		option.Footer = footer;
		option.SetFunction(func);
		option.SetVector(vec);
		option.SetVectorIndex(0);
		mOptions.push_back(std::make_unique<VectorOption>(option));
		return this->GetBackOption<VectorOption*>();
	}

	/// <summary>
	/// An option with left and right arrows with multiple selections
	/// </summary>
	/// <typeparam name="T">Vector Data Type</typeparam>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="_vec">- The const char* std::vector</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	VectorOption* AddVectorOption(const std::string& text, const std::string& footer, std::vector<const char*> _vec, std::function<void()> func = [] {})
	{
		std::vector<std::string> temp;
		temp.reserve(_vec.size());
		for (const auto& str : _vec) {
			temp.emplace_back(str);
		}
		return AddVectorOption(text, footer, temp, func);
	}

	/// <summary>
	/// An option with left and right arrows with multiple selections
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="numElements">- Number of items in the vector</param>
	/// <param name="startText">- Text at the beginning of the right text</param>
	/// <param name="endText">- Text at the end of the right text</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	VectorOption* AddVectorOption(const std::string& text, const std::string& footer, int numElements, const std::string& startText = "", const std::string& endText = "", std::function<void()> func = [] {})
	{
		std::vector<std::string> _temp;
		for (int i = 0; i < numElements; i++)
			_temp.push_back(startText + std::to_string(i) + endText);
		return AddVectorOption(text, footer, _temp, func);
	}

	/// <summary>
	/// An option that brings you to a new submenu
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="id">- The id of the submenu that you will be brought to</param>
	/// <returns>A pointer to this option instance</returns>
	SubmenuOption* AddSubmenuOption(const std::string& text, const std::string& footer, eSubmenuID id)
	{
		SubmenuOption option((int)mOptions.size());
		option.Text = text;
		option.Footer = footer;
		option.SubmenuID = id;
		mOptions.push_back(std::make_unique<SubmenuOption>(option));
		return this->GetBackOption<SubmenuOption*>();
	}

	/// <summary>
	/// An option that brings you to a new submenu
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="submenu">- The submenu that you will be brought to</param>
	/// <returns>A pointer to this option instance</returns>
	SubmenuOption* AddSubmenuOption(const std::string& text, const std::string& footer, Submenu* pSubmenu)
	{
		if (pSubmenu == nullptr) {
			PRINT_ERROR("Failed to add submenu option, pSubmenu is null");
			return nullptr;
		}
		return AddSubmenuOption(text, footer, pSubmenu->ID);
	}

	/// <summary>
	/// Inserts a blank spot into this submenu. Mainly used for organization purposes.
	/// </summary>
	/// <returns>A pointer to this option instance</returns>
	EmptyOption* AddEmptyOption(const std::string &text = "")
	{
		EmptyOption option((int)mOptions.size());
		option.Text = text;
		mOptions.push_back(std::make_unique<EmptyOption>(option));
		return this->GetBackOption<EmptyOption*>();
	}

	// Returns the number of options in this submenu
	int GetNumberOfOptions()
	{
		return (int)mOptions.size();
	}

	// Returns a pointer to an option in this submenu via its index
	Option* GetOption(int index)
	{
		if (index >= mOptions.size()) {
			PRINT_ERROR("Bad option index (", index, "). Submenu ID: ", (int)ID, ", mOptions.size(): ", mOptions.size(), ". Returning null");
			return nullptr;
		}
		else {
			Option* ptr = nullptr;
			if (mOptions[index] != nullptr) {
				ptr = mOptions[index].get();
			}
			else {
				PRINT_ERROR("unique_ptr for option index ", index, " is null, returning null");
			}
			return ptr;
		}
	}

	// Delete an option from this submenu
	void DeleteOption(int index)
	{
		for (auto it = mOptions.begin(); it != mOptions.end(); it++) {
			auto option = it->get();
			if (option->Index == index) {
				mOptions.erase(it);
				break;
			}
		}
	}

	// Delete an option from this submenu
	void DeleteOption(Option* pOption)
	{
		if (pOption == nullptr) {
			PRINT_ERROR("Failed to delete option, pOption is null");
		}
		else {
			this->DeleteOption(pOption->Index);
		}
	}

	// Remove all options from this submenu
	void Clear()
	{
		mOptions.clear(); // unique_ptr objects are deleted here
		mOptions.shrink_to_fit();
	}
};
