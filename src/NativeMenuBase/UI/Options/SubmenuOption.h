// Licensed under the MIT License.

#pragma once
#include "Option.h"
#include "../SubmenuIDs.h"

class CSubmenuOption : public COption
{
public:
	eSubmenuID SubmenuID = eSubmenuID::Invalid;

	CSubmenuOption(int index)
	{
		this->IsSubmenuOption = true;
		this->Index = index;
	}
};
