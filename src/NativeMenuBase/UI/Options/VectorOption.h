// Licensed under the MIT License.

#pragma once
#include "Option.h"
#include "../../console.h"

class VectorOption : public Option
{
private:
	std::vector<std::string> m_VectorContent = {};
	bool m_bWentLeft = false;
	bool m_bWentRight = false;
	int m_VectorIndex = 0;
public:

	std::string RightText = "";

	VectorOption(int index)
	{
		this->IsVectorOption = true;
		this->Index = index;
	}

	template<typename T>
	void SetVector(const std::vector<T>& vec)
	{
		std::vector<std::string> _temp;
		_temp.reserve(vec.size());
		for (const auto& item : vec) {
			_temp.emplace_back(std::to_string(item));
		}
		m_VectorContent.swap(_temp);
	}

	inline void SetVector(std::vector<std::string> &vec)
	{
		m_VectorContent.swap(vec);
	}

	// Safely set the index of the vector
	void SetVectorIndex(int index)
	{
		if (index < m_VectorContent.size()) {
			m_VectorIndex = index;
			RightText = m_VectorContent[m_VectorIndex];
		}
		else {
			PRINT_ERROR("Index was out of range of this vector: ", index, " - m_VectorContent.size(): ", m_VectorContent.size());
		}
	}

	// Gets the index of this option's vector
	inline int GetVectorIndex()
	{
		return m_VectorIndex;
	}

	/// <summary>
	/// Executes the function assigned to this option
	/// </summary>
	/// <param name="bLeft">- Execute as if the user went back (left) when clicking this option</param>
	/// <param name="bRight">- Execute as if the user went forward (right) when clicking this option</param>
	void ExecuteVectorFunction(bool bLeft, bool bRight)
	{
		m_bWentLeft = bLeft;
		m_bWentRight = bRight;

		if (bLeft) {
			m_VectorIndex -= 1;
			if (m_VectorIndex < 0) {
				m_VectorIndex = (int)m_VectorContent.size() - 1;
			}
		}
		else {
			m_VectorIndex += 1;
			if (m_VectorIndex > (int)m_VectorContent.size() - 1) {
				m_VectorIndex = 0;
			}
		}

		RightText = m_VectorContent[m_VectorIndex];
		this->Execute();

		m_bWentLeft = false;
		m_bWentRight = false;
	}

	// Gets the direction the vector went in when the function was executed
	// This should be called inside of the assigned function.
	void GetVectorDirection(bool* bWentLeft, bool* bWentRight)
	{
		*bWentLeft = this->m_bWentLeft;
		*bWentRight = this->m_bWentRight;
	}
};
