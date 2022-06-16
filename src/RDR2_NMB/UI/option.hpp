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

		std::vector<std::string> m_Vector;
		bool* m_BoolPtr = nullptr;
		std::function<void()> m_Func;

	public:
		bool m_IsRegularOption = false;
		bool m_IsBoolOption = false;
		bool m_IsVectorOption = false;
		bool m_IsSubMenuOption = false;
		bool m_IsPageBreak = false;

		int m_Index = 0;
		int m_VectorIndex = 0;
		int m_SubMenuID = Submenu_Invalid;

		Option(bool bRegular, bool bBool, bool bVector, bool bSub, bool bPB)
		{
			m_IsRegularOption = bRegular;
			m_IsBoolOption = bBool;
			m_IsVectorOption = bVector;
			m_IsSubMenuOption = bSub;
			m_IsPageBreak = bPB;
		}

		//==================================//

		// bFromRight: Get text from this vector option
		std::string GetText(bool bFromRight = false)
		{
			if (bFromRight)
				return m_RightText;
			return m_LeftText;
		}

		std::string GetFooter()
		{
			return m_Footer;
		}

		bool* GetBoolPtr()
		{
			return m_BoolPtr;
		}

		void SetText(std::string text)
		{
			m_LeftText = text;
			// temp
			if (m_IsVectorOption)
				m_RightText = m_Vector[0];
		}

		void SetFooter(std::string footer)
		{
			m_Footer = footer;
		}

		void SetBoolPtr(bool* ptr)
		{
			m_BoolPtr = ptr;
		}

		void SetSubMenuID(int id)
		{
			m_SubMenuID = id;
		}

		void SetFunction(std::function<void()> &func)
		{
			m_Func = func;
		}

		//template<typename T> // TODO
		void SetVector(std::vector<std::string> vec)
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

		void ExecuteFunc()
		{
			if (m_Func != nullptr)
				m_Func();
		}

		void ExecuteVectorFunc(bool bLeft, bool bRight)
		{
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
			
			if (m_Func != nullptr)
				m_Func();
		}
	};
}
