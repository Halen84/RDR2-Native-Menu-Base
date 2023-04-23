// Licensed under the MIT License.

#pragma once
#include "Option.h"
#include "../SubmenuIDs.h"

class SubmenuOption : public Option
{
public:
	eSubmenuID SubmenuID = eSubmenuID::Submenu_Invalid;

	SubmenuOption(int index)
	{
		this->IsSubmenuOption = true;
		this->Index = index;
	}
};
