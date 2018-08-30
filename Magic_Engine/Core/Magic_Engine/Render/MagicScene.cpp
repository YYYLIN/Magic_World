#include "MagicScene.h"
#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"
#include "System/Supervisor.h"

namespace Magic
{
	bool AddSceneCommon(Magic::SceneCommon* _pSceneCommon, bool _AutoRelease)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<System::ThreadsComponent>();

		bool _result;
		EnterCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		auto _Common = _TreadComponent->M_SceneCommonBox.find(_pSceneCommon->GetName());
		if (_Common == _TreadComponent->M_SceneCommonBox.end())
		{
			_TreadComponent->M_SceneCommonBox.insert(std::make_pair(_pSceneCommon->GetName(), Magic::System::SceneCommonBox(_pSceneCommon, _AutoRelease)));
			_result = true;
		}
		else
			_result = false;
		LeaveCriticalSection(&_TreadComponent->m_MutexSceneCommon);

		if (!_result)
			Magic::SetEngineErrorMessage("1.A scene with the same name already exists\n");

		return _result;
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

	//------------------------------------------------------------------

	void MouseCollisionCheckSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
	{
		glm::vec2 _MousePos = GetNowSceneTOParentsScenePos(m_MousePos, _NowEntity, EntityCommon());
		EntityX::ComponentHandle<Magic::System::ObjectPositionSizeC> _ObjectPositionSizeC;
		EntityX::ComponentHandle<MouseCollisionStateC> _MouseCollisionStateC;
		for (EntityX::Entity _entity : _es.entities_with_components<Magic::System::ObjectPositionSizeC, MouseCollisionStateC>(_ObjectPositionSizeC, _MouseCollisionStateC))
		{

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

	int SceneCommon::S_MessageHandle(EntityX::Entity _Entity, const Magic::System::MessageStruct& _MessageStruct)
	{
		if (_MessageStruct.MessageType == MAGIC_UI_MESSAGE_MOUSE_MOVE)
		{
			if (_Entity.has_component<MouseCollisionStateC>() && !_Entity.GetComponent<MouseCollisionStateC>()->IsCollision)
			{
				return MAGIC_RETURN_EXIT;
			}
			EntityX::EntityX* _pSupervisor = &_Entity.GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor;

			if (_Entity.has_component<Magic::System::ObjectSupervisor>() && _pSupervisor->m_systems.Has_System<MouseCollisionCheckSystem>())
			{
				_pSupervisor->m_systems.system<MouseCollisionCheckSystem>()->m_MousePos = 
					glm::vec2(LONG_TO_MOUSE_X(_MessageStruct.Message), LONG_TO_MOUSE_Y(_MessageStruct.Message));
				_pSupervisor->m_systems.Update<MouseCollisionCheckSystem>(_Entity, 0);
			}
		}

		return _Entity.GetComponent<SceneEntityC>()->m_SceneCommonBox.pSceneCommon->MessageHandle(_MessageStruct);
	}

	void SceneCommon::S_Updata(EntityX::Entity _Entity)
	{
		_Entity.GetComponent<SceneEntityC>()->m_SceneCommonBox.pSceneCommon->Updata();
	}

	void SceneCommon::S_RenderStart(EntityCommon _Entity)
	{
		_Entity.GetComponent<SceneEntityC>()->m_SceneCommonBox.pSceneCommon->RenderStart();
	}

	void SceneCommon::S_Render(EntityCommon _Entity)
	{
		_Entity.GetComponent<SceneEntityC>()->m_SceneCommonBox.pSceneCommon->Render();
	}

	void SceneCommon::S_RenderEnd(EntityCommon _Entity)
	{
		_Entity.GetComponent<SceneEntityC>()->m_SceneCommonBox.pSceneCommon->RenderEnd();
	}

	bool SceneCommon::Initialize(const EntityCommon& _ParentEntity, bool _AutoRelease)
	{
		if (m_Entity.valid())
		{
			SetEngineErrorMessage("只能初始化一次\n");
			return false;
		}
		bool _result = Magic::CreateScene(_ParentEntity, &m_Entity);
		if (!_result)
			return false;

		m_Entity.assign<SceneEntityC>(Magic::System::SceneCommonBox(this, _AutoRelease));

		m_Entity.GetComponent<Magic::System::MessageHandleComponent>()->m_Call_MessageHandle = Magic::SceneCommon::S_MessageHandle;
		m_Entity.GetComponent<Magic::System::UpdataComponent>()->m_Call_Updata = Magic::SceneCommon::S_Updata;
		m_Entity.GetComponent<Magic::System::RenderComponent>()->m_Call_RenderStart = Magic::SceneCommon::S_RenderStart;
		m_Entity.GetComponent<Magic::System::RenderComponent>()->m_Call_Render = Magic::SceneCommon::S_Render;
		m_Entity.GetComponent<Magic::System::RenderComponent>()->m_Call_RenderEnd = Magic::SceneCommon::S_RenderEnd;

		_result = AddSceneCommon(this, _AutoRelease);
		if (!_result)
			return false;

		_result = OnInitialize();
		if (!_result)
			return false;

		return true;
	}

	int SceneCommon::MessageHandle(const Magic::System::MessageStruct& _MessageStruct)
	{
		return MAGIC_RETURN_OK;
	}

	void SceneCommon::Updata()
	{

	}

	void SceneCommon::RenderStart()
	{
		glm::vec2 _pos = m_Entity.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;

		glm::mat4 _Camera = CONST_CAMERA;
		_Camera[3].x = _pos.x;
		_Camera[3].y = _pos.y;
		Magic::GetPen_Normal()->SetCameraMatrix(_Camera);
	}

	void SceneCommon::Render()
	{

	}

	void SceneCommon::RenderEnd()
	{
		EntityCommon _pParentSupervisor = m_Entity.GetComponent<Magic::System::ObjectSupervisor>()->pParentSupervisor;
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

			EntityX::EntityX* _Supervisor = &((*_EntityCommon).assign<Magic::System::ObjectSupervisor>(_ParentEntity)->m_Supervisor);

			(*_EntityCommon).assign<Magic::System::PosSizeComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
			(*_EntityCommon).assign<Magic::System::MessageHandleComponent>();
			(*_EntityCommon).assign<Magic::System::UpdataComponent>();
			(*_EntityCommon).assign<Magic::System::RenderComponent>();

			_Supervisor->m_systems.add<Magic::System::MessageHandleSystem>();
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

		SceneCommon* _pSceneCommon;
		EnterCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		auto _auto = _TreadComponent->M_SceneCommonBox.find(_name);
		if (_auto != _TreadComponent->M_SceneCommonBox.end())
			_pSceneCommon = _auto->second.pSceneCommon;
		else
			_pSceneCommon = 0;
		LeaveCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		return _pSceneCommon;
	}

	Magic::SceneCommon* GetSceneCommon(const char* _ThreadsName, const char* _SceneName)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = MagicEngineContext::Instance()->GetThreadsResourceManager(_ThreadsName).GetComponent<System::ThreadsComponent>();

		SceneCommon* _pSceneCommon;
		EnterCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		auto _auto = _TreadComponent->M_SceneCommonBox.find(_SceneName);
		if (_auto != _TreadComponent->M_SceneCommonBox.end())
			_pSceneCommon = _auto->second.pSceneCommon;
		else
			_pSceneCommon = 0;
		LeaveCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		return _pSceneCommon;
	}

	Magic::SceneCommon* GetSceneCommon(EntityCommon& _ThreadsEntity, const char* _SceneName)
	{
		EntityX::ComponentHandle<System::ThreadsComponent> _TreadComponent = _ThreadsEntity.GetComponent<System::ThreadsComponent>();

		SceneCommon* _pSceneCommon;
		EnterCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		auto _auto = _TreadComponent->M_SceneCommonBox.find(_SceneName);
		if (_auto != _TreadComponent->M_SceneCommonBox.end())
			_pSceneCommon = _auto->second.pSceneCommon;
		else
			_pSceneCommon = 0;
		LeaveCriticalSection(&_TreadComponent->m_MutexSceneCommon);
		return _pSceneCommon;
	}

	const glm::vec2& GetScenePos(EntityCommon _EntityCommon)
	{
		return _EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;
	}

	const glm::vec2& GetSceneSize(EntityCommon _EntityCommon)
	{
		return _EntityCommon.GetComponent<Magic::System::PosSizeComponent>()->m_Size;
	}

	const glm::vec2& GetNowSceneTOParentsScenePos(glm::vec2 _Pos, EntityCommon _NowScene, const EntityCommon& _ParentsScene)
	{
		while (_NowScene != _ParentsScene && _NowScene.valid())
		{
			if (_NowScene.has_component<Magic::System::PosSizeComponent>())
				_Pos += _NowScene.GetComponent<Magic::System::PosSizeComponent>()->m_Pos;
			if (_NowScene.has_component<Magic::System::ObjectSupervisor>())
				_NowScene = _NowScene.GetComponent<Magic::System::ObjectSupervisor>()->pParentSupervisor;
		}

		return _Pos;
	}
}