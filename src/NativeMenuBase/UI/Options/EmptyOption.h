// Licensed under the MIT License.

#pragma once
#include "Option.h"

class CEmptyOption : public COption
{
public:
	CEmptyOption(int index)
	{
		this->IsEmptyOption = true;
		this->Index = index;
	}
};
