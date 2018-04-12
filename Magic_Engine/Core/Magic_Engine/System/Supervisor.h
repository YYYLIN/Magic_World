#pragma once

#include "EntityX.h"
#include "MagicPen.h"
#include "windows.h"
#include "glm.hpp"

namespace Magic
{
	namespace System
	{
		typedef void(*Call_Entity)(EntityX::Entity _entity);
		typedef bool(*Call_Entity_Bool)(EntityX::Entity _entity);

		struct ObjectSupervisor : EntityX::Component<ObjectSupervisor>
		{
			explicit ObjectSupervisor(EntityX::EntityX* _pParent) :pParentSupervisor(_pParent) {}
			EntityX::EntityX m_Supervisor;
			EntityX::EntityX* pParentSupervisor;
		};

		struct InitializeComponent : EntityX::Component<InitializeComponent>
		{
			explicit InitializeComponent(Call_Entity_Bool _Call_Initialize)
				:m_Call_Initialize(_Call_Initialize) {}

			Call_Entity_Bool m_Call_Initialize;
		};

		struct RenderComponent : EntityX::Component<RenderComponent>
		{
			explicit RenderComponent(Call_Entity _Call_RenderStart = 0, Call_Entity _Call_Render = 0, Call_Entity _Call_RenderEnd = 0)
				:m_Call_RenderStart(_Call_RenderStart), m_Call_Render(_Call_Render), m_Call_RenderEnd(_Call_RenderEnd) {}

			Call_Entity m_Call_RenderStart;
			Call_Entity m_Call_Render;
			Call_Entity m_Call_RenderEnd;
		};

		struct PosSizeComponent :EntityX::Component<PosSizeComponent>
		{
			explicit PosSizeComponent(const glm::vec2& _pos, const glm::vec2 _size)
				:m_Pos(_pos), m_Size(_size) {}
			glm::vec2 m_Pos;
			glm::vec2 m_Size;
		};

		struct UpdataComponent : EntityX::Component<UpdataComponent>
		{
			explicit UpdataComponent(Call_Entity _Call_Updata = 0) : m_Call_Updata(_Call_Updata) {}

			Call_Entity m_Call_Updata;
		};

		struct EngineComponent : EntityX::Component<EngineComponent>
		{
			Magic::Pen_Normal m_Pen_Normal;
			unsigned int m_DrawMessageNumber;
			//Windows
			HGLRC m_hRC;
			HDC m_HDC;
			HWND m_hWnd;
		};

		class ObjectInitializeSystem :public EntityX::System<ObjectInitializeSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time) override;
		};

		class ObjectUpdataSystem :public EntityX::System<ObjectUpdataSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time) override;
		};

		class ObjectRenderSystem :public EntityX::System<ObjectRenderSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, EntityX::TimeDelta _time) override;
		};

		bool EngineInitialize(EntityX::Entity _Entity);
		void RenderStart(EntityX::Entity _Entity);
		void RenderEnd(EntityX::Entity _Entity);

		//系统主管
		class Supervisor :public EntityX::EntityX
		{
		public:
			bool Initialize(HWND _hwnd, float _x, float _y, float _w, float _h);

			void Run();
		};
	}
}