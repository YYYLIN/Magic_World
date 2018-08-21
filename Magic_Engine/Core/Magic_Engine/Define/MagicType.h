#pragma once

#include "EntityX.h"

typedef ::EntityX::Entity EntityCommon;

namespace Magic
{
	class SceneCommon;

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
			ObjectMessageStruct() = default;
			ObjectMessageStruct(const EntityCommon& _Object, const MessageStruct& _Message) :Object(_Object), Message(_Message) {}
			EntityCommon Object;
			MessageStruct Message;
		};

		struct SceneCommonBox
		{
			SceneCommonBox(Magic::SceneCommon* _pSceneCommon, bool _AutoRelease)
				:pSceneCommon(_pSceneCommon), AutoRelease(_AutoRelease) {}

			Magic::SceneCommon* pSceneCommon;
			bool AutoRelease;
		};

		typedef int(*Call_MessageHandle)(EntityX::Entity _entity, const MessageStruct& _MessageStruct);
		typedef void(*Call_Entity)(EntityX::Entity _entity);
		typedef bool(*Call_Entity_Bool)(EntityX::Entity _entity);
	}
}
