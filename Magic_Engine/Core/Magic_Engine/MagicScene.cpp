#include "MagicScene.h"
#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"

namespace Magic
{
	SceneCommon::SceneCommon(const char* _name)
	{
		m_Name = _name;
	}

	SceneCommon::~SceneCommon()
	{
		m_Entity.GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor.m_entities.destroy(m_Entity.id());
	}

	bool SceneCommon::Initialize(const EntityCommon& _ParentEntity, Magic::SceneCommon* _pSceneCommon, bool _AutoRelease)
	{
		if (m_Entity.valid())
		{
			SetEngineErrorMessage("只能初始化一次\n");
			return false;
		}
		bool _result = Magic::CreateScene(_ParentEntity, &m_Entity);
		if (!_result)
			return false;

		_result = MagicEngineContext::Instance()->AddSceneCommon(_pSceneCommon, _AutoRelease);
		if (!_result)
			return false;

		_result = OnInitialize();
		if (!_result)
			return false;

		return true;
	}

	void SceneRenderStart(EntityCommon _Entity)
	{
		glm::vec2 _pos = _Entity.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;

		glm::mat4 _Camera = CONST_CAMERA;
		_Camera[3].x = _pos.x;
		_Camera[3].y = _pos.y;
		Magic::GetPen_Normal()->SetCameraMatrix(_Camera);
	}

	void SceneRenderEnd(EntityCommon _Entity)
	{
		EntityCommon _pParentSupervisor = _Entity.GetComponent<Magic::System::RenderComponent>()->pParentSupervisor;
		if (_pParentSupervisor.valid())
		{
			glm::vec2 _pos = _pParentSupervisor.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;

			glm::mat4 _Camera = CONST_CAMERA;
			_Camera[3].x = _pos.x;
			_Camera[3].y = _pos.y;
			Magic::GetPen_Normal()->SetCameraMatrix(_Camera);
		}
	}

	bool CreateScene(SceneCommon* _SceneCommon, EntityCommon* _EntityCommon)
	{
		return CreateScene(_SceneCommon->GetEntity(), _EntityCommon);
	}

	bool CreateScene(EntityCommon _ParentEntity, EntityCommon* _EntityCommon)
	{
		if (_ParentEntity.has_component<Magic::System::ObjectSupervisor>())
		{
			EntityX::EntityX* _Supervisor = &((*_EntityCommon).assign<Magic::System::ObjectSupervisor>()->m_Supervisor);

			*_EntityCommon = _Supervisor->m_entities.create();

			(*_EntityCommon).assign<Magic::System::UpdataComponent>((Magic::System::Call_Entity)0);
			(*_EntityCommon).assign<Magic::System::RenderComponent>(_ParentEntity, Magic::SceneRenderStart,
				(Magic::System::Call_Entity)0, Magic::SceneRenderEnd);

			_Supervisor->m_systems.add<Magic::System::ObjectUpdataSystem>();
			_Supervisor->m_systems.add<Magic::System::ObjectRenderSystem>();
			_Supervisor->m_systems.configure();

			return true;
		}
		else
		{
			SetEngineErrorMessage("1.There is no ObjectSupervisor Component\n");
			return false;
		}
	}

	void SetScenePosSize(EntityCommon _EntityCommon, const glm::vec2& _pos, const glm::vec2& _size)
	{
		EntityX::ComponentHandle<Magic::System::PosSizeComponent> _PosSizeComponent = _EntityCommon.GetComponent<Magic::System::PosSizeComponent>();
		_PosSizeComponent->m_Pos = _pos;
		_PosSizeComponent->m_Size = _size;
	}

	void SetScenePos(EntityCommon _EntityCommon, const glm::vec2& _pos)
	{
		_EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Pos = _pos;
	}

	void SetSceneSize(EntityCommon _EntityCommon, const glm::vec2& _size)
	{
		_EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Size = _size;
	}

	Magic::SceneCommon* GetSceneCommon(const char* _name)
	{
		return MagicEngineContext::Instance()->GetSceneCommon(_name);
	}

	const glm::vec2& GetScenePos(EntityCommon _EntityCommon)
	{
		return _EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;
	}

	const glm::vec2& GetSceneSize(EntityCommon _EntityCommon)
	{
		return _EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Size;
	}
}