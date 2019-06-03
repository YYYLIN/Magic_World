#pragma once

#include "Define/MagicType.h"

namespace Magic
{
	namespace System
	{
		class Object
		{
		public:
			virtual ~Object(){}
		public:
			inline EntityCommon GetEntity() { return m_Entity; }
		protected:
			EntityCommon m_Entity;
		};
	}
}