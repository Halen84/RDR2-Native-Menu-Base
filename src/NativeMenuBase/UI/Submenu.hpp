#pragma once

#include "Options/Option.h"
#include "Options/RegularOption.h"
#include "Options/BoolOption.h"
#include "Options/VectorOption.h"
#include "Options/SubmenuOption.h"
#include "Options/EmptyOption.h"
#include "../console.h"


// This is a little "hack" so I can properly register the submenus in the CNativeSubmenu ctor.
// This vector gets cleared after the CNativeSubmenu ctor gets called.
// All submenus get put in this vector initially via CSubmenu ctor
//
// I have to do this because I can't use "g_Menu" in this file because msvc is literally retarded.
// Can't put the ctor in a .cpp file because the script just wont register??????????
inline std::vector<void*> __submenus = {};


class CSubmenu
{
protected:
	std::vector<std::unique_ptr<COption>> m_Options{};

private:
	template<class T>
	std::enable_if_t<std::is_pointer_v<T>, T> GetBackOption()
	{
		return m_Options.back().get()->As<T>();
	}

public:
	const char* Header = 0;
	const char* SubHeader = 0;
	eSubmenuID ID = eSubmenuID::Invalid;
	int NumberOfVisibleOptions = 8; // Recommended: 8, Recommended Max: 12

	// Pure virtual function that initializes the submenu and creates the options
	virtual void Init() = 0;

	// Virtual function that will be called only once every time you enter the submenu
	// 
	// We can __forceinline only on these implementations to optimize away the function call to just a NOP since these are empty functions here.
	// The overridden function of these will not be inlined unless specified by user
	__forceinline virtual void OnEnter() { };

	// Virtual function that will be called only once every time you exit the submenu
	__forceinline virtual void OnExit() { };

	// Virtual function that will be called every frame while you are in the submenu
	__forceinline virtual void OnTick() { };

	CSubmenu()
	{
		__submenus.push_back(this);
	}
	~CSubmenu()
	{
		m_Options.clear(); // unique_ptr objects are deleted here
	}

	// Registers/Creates a submenu without a singleton variable
	template <class T>
	static void RegisterWithoutSingleton()
	{
		// Needs to be static so the object doesn't get destroyed.
		static T temp{};
	}

	/// <summary>
	/// An option with only one purpose: to execute a function when clicked.
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	CRegularOption* AddRegularOption(const std::string& text, const std::string& footer, std::function<void()> func = [] {})
	{
		CRegularOption option((int)m_Options.size());
		option.Text = text;
		option.Footer = footer;
		option.SetFunction(func);
		m_Options.push_back(std::make_unique<CRegularOption>(option));
		return this->GetBackOption<CRegularOption*>();
	}

	/// <summary>
	/// Option that acts as a on/off (true/false) switch
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="pbVariable">- Boolean pointer to toggle</param>
	/// <param name="func">- The function to execute when clicked</param>
	/// <returns>A pointer to this option instance</returns>
	CBoolOption* AddBoolOption(const std::string& text, const std::string& footer, bool* pBoolVariable, std::function<void()> func = [] {})
	{
		CBoolOption option((int)m_Options.size());
		option.Text = text;
		option.Footer = footer;
		option.SetFunction(func);
		option.pBoolPointer = pBoolVariable;
		m_Options.push_back(std::make_unique<CBoolOption>(option));
		return this->GetBackOption<CBoolOption*>();
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
	CVectorOption* AddVectorOption(const std::string& text, const std::string& footer, std::vector<T> vec, std::function<void()> func = [] {})
	{
		if constexpr (std::is_same_v<const char*, T>)
		{
			// Must be treated differently because std::to_string()
			// does not work with "const char*"

			std::vector<std::string> temp;
			temp.reserve(vec.size());
			for (const auto& str : vec) {
				temp.emplace_back(str);
			}

			return AddVectorOption(text, footer, temp, func);
		}
		else
		{
			CVectorOption option((int)m_Options.size());
			option.Text = text;
			option.Footer = footer;
			option.SetFunction(func);
			option.SetVector(vec);
			option.SetVectorIndex(0);
			m_Options.push_back(std::make_unique<CVectorOption>(option));
			return this->GetBackOption<CVectorOption*>();
		}
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
	CVectorOption* AddVectorOption(const std::string& text, const std::string& footer, int numElements, const std::string& startText = "", const std::string& endText = "", std::function<void()> func = [] {})
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
	CSubmenuOption* AddSubmenuOption(const std::string& text, const std::string& footer, eSubmenuID id)
	{
		CSubmenuOption option((int)m_Options.size());
		option.Text = text;
		option.Footer = footer;
		option.SubmenuID = id;
		m_Options.push_back(std::make_unique<CSubmenuOption>(option));
		return this->GetBackOption<CSubmenuOption*>();
	}

	/// <summary>
	/// An option that brings you to a new submenu
	/// </summary>
	/// <param name="text">- Option text</param>
	/// <param name="footer">- Option footer text</param>
	/// <param name="submenu">- The submenu that you will be brought to</param>
	/// <returns>A pointer to this option instance</returns>
	CSubmenuOption* AddSubmenuOption(const std::string& text, const std::string& footer, CSubmenu* pSubmenu)
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
	CEmptyOption* AddEmptyOption(const std::string &text = "")
	{
		CEmptyOption option((int)m_Options.size());
		option.Text = text;
		m_Options.push_back(std::make_unique<CEmptyOption>(option));
		return this->GetBackOption<CEmptyOption*>();
	}

	// Returns the number of options in this submenu
	inline int GetNumberOfOptions()
	{
		return (int)m_Options.size();
	}

	// Returns a pointer to an option in this submenu via its index
	COption* GetOption(int index)
	{
		if (index >= m_Options.size()) {
			PRINT_ERROR("Bad option index (", index, "). Submenu ID: ", (int)ID, ", m_Options.size(): ", m_Options.size(), ". Returning null");
			return nullptr;
		}
		else {
			COption* ptr = nullptr;
			if (m_Options[index] != nullptr) {
				ptr = m_Options[index].get();
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
		for (auto it = m_Options.begin(); it != m_Options.end(); it++) {
			auto option = it->get();
			if (option->Index == index) {
				m_Options.erase(it);
				break;
			}
		}
	}

	// Delete an option from this submenu
	void DeleteOption(COption* pOption)
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
		m_Options.clear(); // unique_ptr objects are deleted here
		m_Options.shrink_to_fit();
	}
};
