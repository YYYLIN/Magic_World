#include "Supervisor.h"

namespace Magic
{
	namespace System
	{
		void ObjectUpdataSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<ObjectSupervisor> _ObjectSupervisor;
			EntityX::ComponentHandle<UpdataComponent> _UpdataComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<UpdataComponent>(_UpdataComponent))
			{
				if (_UpdataComponent->m_Call_Updata)
					_UpdataComponent->m_Call_Updata();
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectUpdataSystem>(_time);
			}
		}

		void ObjectRenderSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time)
		{
			/*
						EntityX::ComponentHandle<ObjectSupervisor> _ObjectSupervisor;
						EntityX::ComponentHandle<RenderComponent> _RenderComponent;
						for (EntityX::Entity _entity : _es.entities_with_components<ObjectSupervisor, RenderComponent>(_ObjectSupervisor, _RenderComponent))
						{
							if (_RenderComponent->m_Call_RenderStart)
								_RenderComponent->m_Call_RenderStart();
							_ObjectSupervisor->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_time);
							if (_RenderComponent->m_Call_Render)
								_RenderComponent->m_Call_Render();
							if (_RenderComponent->m_Call_RenderEnd)
								_RenderComponent->m_Call_RenderEnd();
						}*/
		}

		bool Supervisor::Initialize()
		{
			return true;
		}
	}
}