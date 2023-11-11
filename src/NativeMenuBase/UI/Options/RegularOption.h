// Licensed under the MIT License.

#pragma once
#include "Option.h"

class CRegularOption : public COption
{
public:
	CRegularOption(int index)
	{
		this->IsRegularOption = true;
		this->Index = index;
	}
};
