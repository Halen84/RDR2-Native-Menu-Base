// Licensed under the MIT License.

#pragma once
#include "Option.h"

class CBoolOption : public COption
{
public:
	bool* pBoolPointer = nullptr;

	CBoolOption(int index)
	{
		this->IsBoolOption = true;
		this->Index = index;
	}
};
