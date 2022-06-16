#pragma once
#include "menu.hpp"
#include "submenu_ids.hpp"

namespace Menu
{
	class Submenu
	{
	public:
		std::vector<Option> m_Options;

		std::string m_Header = "";
		std::string m_SubHeader = "";
		int m_ID = Submenu_Invalid;
		int m_NumVisibleOptions = 8; // 8 = DEFAULT
		int m_NumOptions = 0;


		Submenu() = default;
		Submenu(const std::string& header, const std::string& subHeader, int id, int numVisibleOptions, std::function<void(Submenu*)> &func)
		{
			m_Header = header;
			m_SubHeader = subHeader;
			m_ID = id;
			m_NumVisibleOptions = numVisibleOptions;
			m_StructureFunc = func;
			std::invoke(m_StructureFunc, static_cast<Submenu*>(this));
		}


		/// Regular plain ol' option.
		/// Passed function will execute when option is pressed.
		void AddRegularOption(const std::string& text, const std::string& footer, std::function<void()> func = [] {})
		{
			Option option(true, false, false, false, false);
			option.SetText(text);
			option.SetFooter(footer);
			option.SetFunction(func);
			option.m_Index = (int)m_Options.size();
			m_Options.push_back(option);
			m_NumOptions++;
		}


		/// An on/off (true/false) boolean toggle option.
		/// Note: bLoopToggle parameter must be an already defined boolean variable.
		/// Passed function will execute when option is pressed.
		void AddBoolOption(const std::string& text, const std::string& footer, bool* bToggle, std::function<void()> func = [] {})
		{
			Option option(false, true, false, false, false);
			option.SetText(text);
			option.SetFooter(footer);
			option.SetFunction(func);
			option.SetBoolPtr(bToggle);
			option.m_Index = (int)m_Options.size();
			m_Options.push_back(option);
			m_NumOptions++;
		}


		/// An option with left and right arrows with multiple choosable options.
		/// Passed function will execute when vector option is switched.
		void AddVectorOption(const std::string& text, const std::string& footer, std::vector<std::string> &vec, std::function<void()> func = [] {})
		{
			Option option(false, false, true, false, false);
			option.SetVector(vec);
			option.SetText(text);
			option.SetFooter(footer);
			option.SetFunction(func);
			option.m_Index = (int)m_Options.size();
			m_Options.push_back(option);
			m_NumOptions++;
		}


		void AddVectorOption(const std::string& text, const std::string& footer, int numOptions, const std::string& startText = "", const std::string& endText = "", std::function<void()> func = [] {})
		{
			std::vector<std::string> _temp;
			for (int i = 0; i < numOptions; i++)
				_temp.push_back(startText + std::to_string(i) + endText);
			AddVectorOption(text, footer, _temp, func);
		}


		/// An option that brings you to a new submenu
		void AddSubmenuOption(const std::string& text, const std::string& footer, int id)
		{
			Option option(false, false, false, true, false);
			option.SetText(text);
			option.SetFooter(footer);
			//option.SetFunction(func);
			option.SetSubMenuID(id);
			option.m_Index = (int)m_Options.size();
			m_Options.push_back(option);
			m_NumOptions++;
		}


		/// Inserts a blank spot into a page
		/// Mainly used for organization purposes
		void AddPageBreak()
		{
			Option option(false, false, false, false, true);
			option.m_Index = (int)m_Options.size();
			m_Options.push_back(option);
			m_NumOptions++;
		}


		Option* GetOption(int optionIndex)
		{
			if (optionIndex > m_Options.size()) {
#if ALLOCATE_CONSOLE
				std::cout << "[Submenu::GetOption] [ERROR]: Invalid optionIndex (" << optionIndex << "). ID: " << m_ID << ", m_Options.size(): " << m_Options.size() << ", returning nullptr" << "\n";
#endif
				return nullptr;
			} else {
				return &m_Options[optionIndex];
			}
		}

	private:
		std::function<void(Submenu*)> m_StructureFunc;
	};
}
