#include "Supervisor.h"

namespace Magic
{
	namespace System
	{
		void MessageHandleSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<MessageHandleComponent> _MessageHandleComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<MessageHandleComponent>(_MessageHandleComponent))
			{
				if (m_MessageStruct.MessageType == MAGIC_UI_MESSAGE_MOUSE_MOVE &&
					(_entity.has_component<MouseCollisionStateC>() && !_entity.GetComponent<MouseCollisionStateC>()->IsCollision))
				{
					continue;
				}

				if (_MessageHandleComponent->m_Call_MessageHandle)
					_MessageHandleComponent->m_Call_MessageHandle(_entity, m_MessageStruct);
				if (_entity.has_component<ObjectSupervisor>())
				{
					ObjectSupervisor* _pObjectSupervisor = _entity.GetComponent<ObjectSupervisor>().operator->();
					_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_MessageStruct = m_MessageStruct;
					_pObjectSupervisor->m_Supervisor.m_systems.Update<MessageHandleSystem>(_entity, _time);
				}
			}
		}

		void ObjectUpdataSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			if (_NowEntity.has_component<Magic::System::UpdataComponent>())
			{
				EntityX::ComponentHandle<Magic::System::UpdataComponent> _UpdataComponent = _NowEntity.GetComponent<Magic::System::UpdataComponent>();
				if (_UpdataComponent->m_Call_Updata)
					_UpdataComponent->m_Call_Updata(_NowEntity);
			}

			EntityX::ComponentHandle<UpdataComponent> _UpdataComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<UpdataComponent>(_UpdataComponent))
			{
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectUpdataSystem>(_entity, _time);
			}
		}

		void ObjectRenderSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			if (_NowEntity.has_component<Magic::System::RenderComponent>())
			{
				EntityX::ComponentHandle<Magic::System::RenderComponent> _RenderComponent = _NowEntity.GetComponent<Magic::System::RenderComponent>();
				if (_RenderComponent->m_Call_RenderStart)
					_RenderComponent->m_Call_RenderStart(_NowEntity);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render(_NowEntity);
			}

			EntityX::ComponentHandle<RenderComponent> _RenderComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<RenderComponent>(_RenderComponent))
			{
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_entity, _time);
			}

			if (_NowEntity.has_component<Magic::System::RenderComponent>())
			{
				EntityX::ComponentHandle<Magic::System::RenderComponent> _RenderComponent = _NowEntity.GetComponent<Magic::System::RenderComponent>();
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd(_NowEntity);
			}
		}

		void MouseCollisionCheckSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<ObjectPositionSizeC> _ObjectPositionSizeC;
			EntityX::ComponentHandle<MouseCollisionStateC> _MouseCollisionStateC;
			for (EntityX::Entity _entity : _es.entities_with_components<ObjectPositionSizeC, MouseCollisionStateC>(_ObjectPositionSizeC, _MouseCollisionStateC))
			{

			}
		}
	}
}