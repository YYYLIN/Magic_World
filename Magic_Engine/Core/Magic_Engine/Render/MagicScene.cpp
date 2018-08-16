#include "MagicScene.h"
#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"
#include "System/Supervisor.h"

namespace Magic
{
	bool AddSceneCommon(Magic::SceneCommon* _pSceneCommon, bool _AutoRelease)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<System::ThreadsComponent>();

		auto _Common = _TreadComponent->M_SceneCommonBox.find(_pSceneCommon->GetName());
		if (_Common == _TreadComponent->M_SceneCommonBox.end())
		{
			_TreadComponent->M_SceneCommonBox.insert(std::make_pair(_pSceneCommon->GetName(), Magic::System::SceneCommonBox(_pSceneCommon, _AutoRelease)));
			return true;
		}
		else
		{
			Magic::SetEngineErrorMessage("1.A scene with the same name already exists\n");
			return false;
		}
	}

	struct SceneEntityC :public EntityX::Component<SceneEntityC>
	{
		explicit SceneEntityC(const Magic::System::SceneCommonBox& _SceneCommonBox) :m_SceneCommonBox(_SceneCommonBox) {}
		~SceneEntityC();

		Magic::System::SceneCommonBox m_SceneCommonBox;
	};

	SceneEntityC::~SceneEntityC()
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<System::ThreadsComponent>();

		auto _Common = _TreadComponent->M_SceneCommonBox.find(m_SceneCommonBox.pSceneCommon->GetName());
		if (_Common != _TreadComponent->M_SceneCommonBox.end())
		{
			if (_Common->second.AutoRelease)
				delete _Common->second.pSceneCommon;
			_TreadComponent->M_SceneCommonBox.erase(_Common);
		}
	}

	SceneCommon::SceneCommon(const char* _name)
	{
		m_Name = _name;
	}

	SceneCommon::~SceneCommon()
	{
		if (m_Entity.valid())
			m_Entity.destroy();
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

		m_Entity.assign<SceneEntityC>(Magic::System::SceneCommonBox(_pSceneCommon, _AutoRelease));

		_result = AddSceneCommon(_pSceneCommon, _AutoRelease);
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
			*_EntityCommon = _ParentEntity.GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor.m_entities.create();

			EntityX::EntityX* _Supervisor = &((*_EntityCommon).assign<Magic::System::ObjectSupervisor>()->m_Supervisor);

			(*_EntityCommon).assign<Magic::System::PosSizeComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
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

	bool DeleteScene(const char* _name)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<System::ThreadsComponent>();

		auto _Common = _TreadComponent->M_SceneCommonBox.find(_name);
		if (_Common != _TreadComponent->M_SceneCommonBox.end())
		{
			_Common->second.pSceneCommon->GetEntity().destroy();

			return true;
		}
		else
		{
			Magic::SetEngineErrorMessage("1.No scene name\n");
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

	void SetSceneCallUpdata(EntityCommon _EntityCommon, Magic::System::Call_Entity _call)
	{
		_EntityCommon.GetComponent<Magic::System::UpdataComponent>()->m_Call_Updata = _call;
	}

	void SetSceneCallRenderStart(EntityCommon _EntityCommon, Magic::System::Call_Entity _call)
	{
		_EntityCommon.GetComponent<Magic::System::RenderComponent>()->m_Call_RenderStart = _call;
	}

	void SetSceneCallRender(EntityCommon _EntityCommon, Magic::System::Call_Entity _call)
	{
		_EntityCommon.GetComponent<Magic::System::RenderComponent>()->m_Call_Render = _call;
	}

	void SetSceneCallRenderEnd(EntityCommon _EntityCommon, Magic::System::Call_Entity _call)
	{
		_EntityCommon.GetComponent<Magic::System::RenderComponent>()->m_Call_RenderEnd = _call;
	}

	Magic::SceneCommon* GetSceneCommon(const char* _name)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<System::ThreadsComponent>();

		auto _auto = _TreadComponent->M_SceneCommonBox.find(_name);
		if (_auto != _TreadComponent->M_SceneCommonBox.end())
			return _auto->second.pSceneCommon;
		else
			return 0;
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