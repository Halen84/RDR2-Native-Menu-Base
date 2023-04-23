// Licensed under the MIT License.

#pragma once
#include "Option.h"
#include "../../console.h"

class VectorOption : public Option
{
private:
	std::vector<std::string> mVectorContent = {};
	bool bWentLeft = false;
	bool bWentRight = false;
	int VectorIndex = 0;
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
		mVectorContent.swap(_temp);
	}

	void SetVector(std::vector<std::string> &vec)
	{
		mVectorContent.swap(vec);
	}

	// Safely set the index of the vector
	void SetVectorIndex(int index)
	{
		if (index < mVectorContent.size()) {
			VectorIndex = index;
			RightText = mVectorContent[VectorIndex];
		}
		else {
			PRINT_ERROR("Index was out of range of this vector: ", index, " - mVectorContent.size(): ", mVectorContent.size());
		}
	}

	// Gets the index of this option's vector
	int GetVectorIndex()
	{
		return VectorIndex;
	}

	/// <summary>
	/// Executes the function assigned to this option
	/// </summary>
	/// <param name="bLeft">- Execute as if the user went back (left) when clicking this option</param>
	/// <param name="bLeft">- Execute as if the user went forward (right) when clicking this option</param>
	void ExecuteVectorFunction(bool bLeft, bool bRight)
	{
		bWentLeft = bLeft;
		bWentRight = bRight;

		if (bLeft) {
			VectorIndex -= 1;
			if (VectorIndex < 0) {
				VectorIndex = (int)mVectorContent.size() - 1;
			}
		}
		else {
			VectorIndex += 1;
			if (VectorIndex > (int)mVectorContent.size() - 1) {
				VectorIndex = 0;
			}
		}

		RightText = mVectorContent[VectorIndex];
		this->Execute();

		bWentLeft = false;
		bWentRight = false;
	}

	// Gets the direction the vector went in when the function was executed
	// This should be called inside of the assigned function.
	void GetVectorDirection(bool* bWentLeft, bool* bWentRight)
	{
		*bWentLeft = this->bWentLeft;
		*bWentRight = this->bWentRight;
	}
};
