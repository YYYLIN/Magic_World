#include "Button.h"
#include "System/Supervisor.h"
#include "Render/MagicEngineAPI.h"


bool CreateButton(const EntityCommon& _ParentEntity, EntityCommon* _pCommon)
{
	if (/*_ParentEntity.has_component<Magic::System::ObjectSupervisor>()*/TRUE)
	{
/*
		*_pCommon = _ParentEntity.GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor.m_entities.create();

		EntityX::EntityX* _Supervisor = &((*_pCommon).assign<Magic::System::ObjectSupervisor>()->m_Supervisor);

		(*_EntityCommon).assign<Magic::System::PosSizeComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
		(*_EntityCommon).assign<Magic::System::UpdataComponent>((Magic::System::Call_Entity)0);
		(*_EntityCommon).assign<Magic::System::RenderComponent>(_ParentEntity, Magic::SceneRenderStart,
			(Magic::System::Call_Entity)0, Magic::SceneRenderEnd);

		_Supervisor->m_systems.add<Magic::System::ObjectUpdataSystem>();
		_Supervisor->m_systems.add<Magic::System::ObjectRenderSystem>();
		_Supervisor->m_systems.configure();*/

		return true;
	}
	else
	{
		Magic::SetEngineErrorMessage("1.There is no ObjectSupervisor Component\n");
		return false;
	}

	return true;
}