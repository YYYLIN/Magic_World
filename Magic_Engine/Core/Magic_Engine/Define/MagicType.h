#pragma once

#include "EntityX.h"

typedef ::EntityX::Entity EntityCommon;

namespace Magic
{
	namespace System
	{
		typedef void(*Call_Entity)(EntityX::Entity _entity);
		typedef bool(*Call_Entity_Bool)(EntityX::Entity _entity);
	}
}
