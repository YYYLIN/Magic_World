#pragma once

#include "EntityX.h"

typedef ::EntityX::Entity EntityCommon;

namespace Magic
{
	namespace System
	{
		struct MessageStruct
		{
			MessageStruct() = default;
			MessageStruct(const unsigned int& _MessageType, const long long& _Message) :MessageType(_MessageType), Message(_Message)
			{
			}

			unsigned int MessageType;
			unsigned long long Message;
		};

		struct ObjectMessageStruct
		{
			EntityCommon Object;
			MessageStruct Message;
		};

		typedef int(*Call_MessageHandle)(EntityX::Entity _entity, const MessageStruct& _MessageStruct);
		typedef void(*Call_Entity)(EntityX::Entity _entity);
		typedef bool(*Call_Entity_Bool)(EntityX::Entity _entity);
	}
}
