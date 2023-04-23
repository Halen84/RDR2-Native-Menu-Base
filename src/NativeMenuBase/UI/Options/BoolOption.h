// Licensed under the MIT License.

#pragma once
#include "Option.h"

class BoolOption : public Option
{
public:
	bool* pBoolPointer = nullptr;

	BoolOption(int index)
	{
		this->IsBoolOption = true;
		this->Index = index;
	}
};
