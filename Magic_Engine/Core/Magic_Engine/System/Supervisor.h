#pragma once

#include "EntityX.h"


namespace Magic
{
	namespace System
	{
		struct ObjectSupervisor : EntityX::Component<ObjectSupervisor>
		{
			explicit ObjectSupervisor(EntityX::EntityX* _pParent) :pParentSupervisor(_pParent) {}
			EntityX::EntityX m_Supervisor;
			EntityX::EntityX* pParentSupervisor;
		};

		struct RenderComponent : EntityX::Component<RenderComponent>
		{
			typedef void(*Call_RenderStart)();
			typedef void(*Call_Render)();
			typedef void(*Call_RenderEnd)();

			explicit RenderComponent(Call_RenderStart _Call_RenderStart = 0, Call_Render _Call_Render = 0, Call_RenderEnd _Call_RenderEnd = 0)
				:m_Call_RenderStart(_Call_RenderStart), m_Call_Render(_Call_Render), m_Call_RenderEnd(_Call_RenderEnd) {}

			Call_RenderStart m_Call_RenderStart;
			Call_Render m_Call_Render;
			Call_RenderEnd m_Call_RenderEnd;
		};

		struct UpdataComponent : EntityX::Component<UpdataComponent>
		{
			typedef void(*Call_Updata)();

			explicit UpdataComponent(Call_Updata _Call_Updata = 0) : m_Call_Updata(_Call_Updata) {}

			Call_Updata m_Call_Updata;
		};

		class ObjectUpdataSystem : EntityX::System<ObjectUpdataSystem>
		{
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time) override;
		};

		class ObjectRenderSystem : EntityX::System<ObjectRenderSystem>
		{
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time) override;
		};

		//系统主管
		class Supervisor :public EntityX::EntityX
		{
		public:
			bool Initialize();
		};
	}
}