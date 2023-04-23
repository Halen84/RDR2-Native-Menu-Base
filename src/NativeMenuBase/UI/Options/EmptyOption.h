// Licensed under the MIT License.

#pragma once
#include "Option.h"

class EmptyOption : public Option
{
public:
	EmptyOption(int index)
	{
		this->IsEmptyOption = true;
		this->Index = index;
	}
};
