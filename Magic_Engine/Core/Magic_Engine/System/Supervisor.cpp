#include "Supervisor.h"
#include <GL/glew.h>  
#include "MagicEngineContext.h"

namespace Magic
{
	namespace System
	{
		void ObjectInitializeSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<InitializeComponent> _InitializeComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<InitializeComponent>(_InitializeComponent))
			{
				if (_InitializeComponent->m_Call_Initialize)
					_InitializeComponent->m_Call_Initialize(_entity);
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectInitializeSystem>(_time);
			}
		}

		void ObjectUpdataSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<UpdataComponent> _UpdataComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<UpdataComponent>(_UpdataComponent))
			{
				if (_UpdataComponent->m_Call_Updata)
					_UpdataComponent->m_Call_Updata(_entity);
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
					_RenderComponent->m_Call_RenderStart(_entity);
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_time);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render(_entity);
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd(_entity);
			}
		}

		bool EngineInitialize(EntityX::Entity _Entity)
		{
			_Entity.assign<UpdataComponent>((Call_Entity)0);
			_Entity.assign<RenderComponent>(RenderStart, (Call_Entity)0, RenderEnd);

			return true;
		}

		void RenderStart(EntityX::Entity _Entity)
		{
			EntityX::ComponentHandle<EngineComponent> _EngineComponent = _Entity.GetComponent<EngineComponent>();

			_EngineComponent->m_DrawMessageNumber = 0;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			_EngineComponent->m_Pen_Normal.RenderStart();
			_EngineComponent->m_Pen_Normal.SetCameraMatrix(glm::mat4());
		}

		void RenderEnd(EntityX::Entity _Entity)
		{
			EntityX::ComponentHandle<EngineComponent> _EngineComponent = _Entity.GetComponent<EngineComponent>();
			EntityX::ComponentHandle<PosSizeComponent> _PosSizeComponent = _Entity.GetComponent<PosSizeComponent>();

			glm::vec2 _WH = _PosSizeComponent->m_Size;
			_EngineComponent->m_Pen_Normal.SetDrawWH(_WH.x, _WH.y);
			_EngineComponent->m_Pen_Normal.RenderEnd();

			SwapBuffers(_EngineComponent->m_HDC);
		}

		bool Supervisor::Initialize(HWND _hwnd, float _x, float _y, float _w, float _h)
		{
			::EntityX::Entity _Entity = m_entities.create();
			_Entity.assign<InitializeComponent>(EngineInitialize);
			::EntityX::ComponentHandle<PosSizeComponent> _PosSizeComponent = _Entity.assign<PosSizeComponent>(glm::vec2(_x, _y), glm::vec2(_w, _h));
			::EntityX::ComponentHandle<EngineComponent> _EngineComponent = _Entity.assign<EngineComponent>();

			_EngineComponent->m_hWnd = _hwnd;
			_EngineComponent->m_HDC = GetDC(_EngineComponent->m_hWnd);
			_EngineComponent->m_hRC = CreateRCContxt(_EngineComponent->m_HDC);
			if (!_EngineComponent->m_hRC)
				return false;

			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				fprintf(stderr, "Error:'%s'\n", glewGetErrorString(err));
				return false;
			}

			//ÆôÓÃ±³ÃæÌÞ³ý
			glEnable(GL_CULL_FACE);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			bool _result = _EngineComponent->m_Pen_Normal.Initialize();
			if (!_result)
				return false;


			m_systems.add<ObjectInitializeSystem>();
			m_systems.add<ObjectUpdataSystem>();
			m_systems.add<ObjectRenderSystem>();
			m_systems.configure();

			m_systems.Update<ObjectInitializeSystem>(0.0f);

			return true;
		}

		void Supervisor::Run()
		{
			m_systems.Update<ObjectUpdataSystem>(0.0f);
			m_systems.Update<ObjectRenderSystem>(0.0f);
		}
	}
}