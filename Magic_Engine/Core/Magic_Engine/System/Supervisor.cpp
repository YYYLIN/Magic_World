#include "Supervisor.h"

namespace Magic
{
	namespace System
	{
		ThreadsComponent::ThreadsComponent() :m_RunState(false), m_DiffTime(0.0), m_LastTime(0.0), m_Threads(0) 
		{
			InitializeCriticalSection(&m_MutexObjectMessage);
			InitializeCriticalSection(&m_MutexSceneCommon);
		}

		ThreadsComponent::~ThreadsComponent()
		{
			DeleteCriticalSection(&m_MutexSceneCommon);
			DeleteCriticalSection(&m_MutexObjectMessage);
		}

		void MessageHandleSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			if (_NowEntity.has_component<Magic::System::MessageHandleComponent>())
			{
				EntityX::ComponentHandle<Magic::System::MessageHandleComponent> _MessageHandleComponent = _NowEntity.GetComponent<Magic::System::MessageHandleComponent>();
				if (m_MessageStruct.MessageType == MAGIC_UI_MESSAGE_MOUSE_MOVE)
				{
					if (_NowEntity.has_component<MouseCollisionStateC>() && !_NowEntity.GetComponent<MouseCollisionStateC>()->IsCollision)
					{
						return;
					}

					if (_NowEntity.has_component<ObjectSupervisor>() && _NowEntity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Has_System<MouseCollisionCheckSystem>())
					{
						_NowEntity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<MouseCollisionCheckSystem>(_NowEntity, _time);
					}
				}

				if (_MessageHandleComponent->m_Call_MessageHandle)
					_MessageHandleComponent->m_Call_MessageHandle(_NowEntity, m_MessageStruct);
			}

			EntityX::ComponentHandle<ObjectSupervisor> _ObjectSupervisor;
			EntityX::ComponentHandle<MessageHandleComponent> _MessageHandleComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<ObjectSupervisor, MessageHandleComponent>(_ObjectSupervisor, _MessageHandleComponent))
			{
				_ObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_MessageStruct = m_MessageStruct;
				_ObjectSupervisor->m_Supervisor.m_systems.Update<MessageHandleSystem>(_entity, _time);
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

			EntityX::ComponentHandle<ObjectSupervisor> _ObjectSupervisor;
			EntityX::ComponentHandle<UpdataComponent> _UpdataComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<ObjectSupervisor, UpdataComponent>(_ObjectSupervisor, _UpdataComponent))
			{
				_ObjectSupervisor->m_Supervisor.m_systems.Update<ObjectUpdataSystem>(_entity, _time);
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

			EntityX::ComponentHandle<ObjectSupervisor> _ObjectSupervisor;
			EntityX::ComponentHandle<RenderComponent> _RenderComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<ObjectSupervisor, RenderComponent>(_ObjectSupervisor, _RenderComponent))
			{
				_ObjectSupervisor->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_entity, _time);
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