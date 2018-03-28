#include "Supervisor.h"

namespace Magic
{
	namespace System
	{
		void ObjectUpdataSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time)
		{
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

			EntityX::ComponentHandle<RenderComponent> _RenderComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<RenderComponent>(_RenderComponent))
			{
				if (_RenderComponent->m_Call_RenderStart)
					_RenderComponent->m_Call_RenderStart();
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_time);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render();
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd();
			}
		}

		bool Supervisor::Initialize()
		{
			::EntityX::Entity _Entity = m_entities.create();

			_Entity.assign<UpdataComponent>((UpdataComponent::Call_Updata)0);

			m_systems.add<ObjectUpdataSystem>();
			m_systems.configure();

			m_systems.Update<ObjectUpdataSystem>(0.0f);

			return true;
		}
	}
}