// Licensed under the MIT License.

#pragma once
#include "menu.hpp"
#include "submenu_ids.hpp"
#include "../common.hpp"

namespace Menu
{
	class Option
	{
	private:
		std::string m_LeftText = "";
		std::string m_RightText = "";
		std::string m_Footer = "";
		
		bool bWentLeft = false;
		bool bWentRight = false;
		bool bWasJustExecuted = false;
		bool bInitialTextSet = false;

		std::vector<std::string> m_Vector;
		bool* m_BoolPtr = nullptr;
		std::function<void()> m_Func;

	public:
		bool m_IsRegularOption = false;
		bool m_IsBoolOption = false;
		bool m_IsVectorOption = false;
		bool m_IsSubmenuOption = false;
		bool m_IsPageBreak = false;

		int m_Index = 0;
		int m_VectorIndex = 0;
		int m_SubmenuID = Submenu_Invalid;

		Option(bool bRegularOption, bool bBooleanOption, bool bVectorOption, bool bSubmenuOption, bool bPageBreak)
		{
			m_IsRegularOption = bRegularOption;
			m_IsBoolOption = bBooleanOption;
			m_IsVectorOption = bVectorOption;
			m_IsSubmenuOption = bSubmenuOption;
			m_IsPageBreak = bPageBreak;
		}


		//==================================//


		// bFromRight: Get text from the vector option
		std::string GetText(bool bFromRight = false) const
		{
			if (bFromRight)
				return m_RightText;
			return m_LeftText;
		}


		std::string GetFooter() const
		{
			return m_Footer;
		}


		bool* GetBoolPtr()
		{
			return m_BoolPtr;
		}


		// bRight: Set text for the vector
		void SetText(const std::string &text, bool bRight = false)
		{
			if (bRight) {
				m_RightText = text;
				m_Vector[m_VectorIndex] = text;
			}
			else {
				m_LeftText = text;
			}
			
			if (!bInitialTextSet && m_IsVectorOption) {
				m_RightText = m_Vector[0];
				bInitialTextSet = true;
			}	
		}


		void SetFooter(const std::string &footer)
		{
			m_Footer = footer;
		}


		void SetBoolPtr(bool* ptr)
		{
			m_BoolPtr = ptr;
		}


		void SetSubmenuID(int id)
		{
			m_SubmenuID = id;
		}


		void SetFunction(const std::function<void()> &func)
		{
			m_Func = func;
		}


		template<typename T>
		void SetVector(const std::vector<T> &vec)
		{
			std::vector<std::string> _temp;
			for (int i = 0; i < vec.size(); i++)
				_temp.push_back(std::to_string(vec[i]));
			m_Vector.swap(_temp);
		}


		void SetVector(std::vector<std::string> &vec)
		{
			m_Vector.swap(vec);
		}


		void SetVectorIndex(int newIndex)
		{
			if (newIndex < m_Vector.size()) {
				m_VectorIndex = newIndex;
				m_RightText = m_Vector[m_VectorIndex];
			}
		}
		

		// Get which way the vector went when the function was executed
		// Note: This should be used in the function this option called
		void GetVectorDirection(bool* bWentLeft, bool* bWentRight)
		{
			*bWentLeft = this->bWentLeft;
			*bWentRight = this->bWentRight;
		}


		inline bool WasJustExecuted()	{ return bWasJustExecuted; }
		inline bool WasJustPressed()	{ return bWasJustExecuted; }


		void ExecuteFunc()
		{
			if (m_Func != nullptr) {
				bWasJustExecuted = true;
				m_Func();
				bWasJustExecuted = false;
			}
		}


		void ExecuteVectorFunc(bool bLeft, bool bRight)
		{
			bWentLeft = bLeft;
			bWentRight = bRight;
			
			if (bLeft) {
				m_VectorIndex -= 1;
				if (m_VectorIndex < 0) 
					m_VectorIndex = (int)m_Vector.size() - 1;
			}
			else {
				m_VectorIndex += 1;
				if (m_VectorIndex > (int)m_Vector.size() - 1)
					m_VectorIndex = 0;
			}

			m_RightText = m_Vector[m_VectorIndex];
			
			if (m_Func != nullptr) {
				bWasJustExecuted = true;
				m_Func();
				bWasJustExecuted = false;
			}
			
			bWentLeft = false;
			bWentRight = false;
		}
	};
}
