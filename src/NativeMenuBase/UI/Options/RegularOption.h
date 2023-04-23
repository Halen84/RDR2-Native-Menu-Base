// Licensed under the MIT License.

#pragma once
#include "Option.h"

class RegularOption : public Option
{
public:
	RegularOption(int index)
	{
		this->IsRegularOption = true;
		this->Index = index;
	}
};
