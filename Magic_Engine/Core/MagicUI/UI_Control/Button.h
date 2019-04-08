#pragma once

#include "Define/MagicType.h"
#include "MagicUIMacro.h"
#include "System/Element.h"

DLL_MAGICUI_OUTPUT_INPUT bool CreateButton(EntityCommon& _ParentEntity, EntityCommon* _pCommon);

namespace Magic
{
	namespace UI
	{
		class Button :public Magic::System::Object
		{

		};
	}
}