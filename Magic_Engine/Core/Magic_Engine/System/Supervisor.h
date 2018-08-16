#pragma once

#include "Define/MagicType.h"
#include "glm.hpp"
#include "windows.h"
#include "Render/MagicPen.h"

namespace Magic
{
	namespace System
	{
		struct ObjectSupervisor : EntityX::Component<ObjectSupervisor>
		{
			ObjectSupervisor() = default;
			ObjectSupervisor(const ObjectSupervisor& _ObjectSupervisor)
			{
				memcpy(this, &_ObjectSupervisor, sizeof(ObjectSupervisor));
			}

			EntityX::EntityX m_Supervisor;
		};

		struct RenderComponent : EntityX::Component<RenderComponent>
		{
			explicit RenderComponent(EntityCommon _pParent, Call_Entity _Call_RenderStart = 0, Call_Entity _Call_Render = 0, Call_Entity _Call_RenderEnd = 0)
				:pParentSupervisor(_pParent), m_Call_RenderStart(_Call_RenderStart), m_Call_Render(_Call_Render), m_Call_RenderEnd(_Call_RenderEnd) {}

			Call_Entity m_Call_RenderStart;
			Call_Entity m_Call_Render;
			Call_Entity m_Call_RenderEnd;
			EntityCommon pParentSupervisor;
		};

		struct PosSizeComponent :EntityX::Component<PosSizeComponent>
		{
			explicit PosSizeComponent(const glm::vec2& _pos, const glm::vec2 _size)
				:m_Pos(_pos), m_Size(_size) {}
			glm::vec2 m_Pos;
			glm::vec2 m_Size;
		};

		struct ThreadsComponent :EntityX::Component<ThreadsComponent>
		{
			explicit ThreadsComponent() :m_RunState(false), m_DiffTime(0.0), m_LastTime(0.0), m_Threads(0) {}

			std::vector<ObjectMessageStruct> m_vec_ObjectMessageStruct;
			std::map<std::string, SceneCommonBox> M_SceneCommonBox;
			HANDLE m_Threads;
			double m_DiffTime, m_LastTime;
			bool m_RunState;
		};

		struct RenderThreadsComponent :EntityX::Component<RenderThreadsComponent>
		{
			explicit RenderThreadsComponent()
				:m_DrawMessageNumber(0), FPS(0.0), FPSTime(0.0), frameCount(0), currentTime(0.0),
				ContinuedTime(0.0) {}
			double FPS, FPSTime;

			int    frameCount;//帧数
			double  currentTime;//当前时间
			double  ContinuedTime;//持续时间

			unsigned int m_DrawMessageNumber;

			Magic::Pen_Normal m_Pen_Normal;

			HGLRC m_hRC;
			HDC m_HDC;
			HWND m_hWnd;
		};

		struct MessageHandleComponent :EntityX::Component<MessageHandleComponent>
		{
			explicit MessageHandleComponent(Call_MessageHandle _Call_MessageHandle = 0) : m_Call_MessageHandle(_Call_MessageHandle) {}

			Call_MessageHandle m_Call_MessageHandle;
		};

		struct UpdataComponent : EntityX::Component<UpdataComponent>
		{
			explicit UpdataComponent(Call_Entity _Call_Updata = 0) : m_Call_Updata(_Call_Updata) {}

			Call_Entity m_Call_Updata;
		};

		struct ObjectPositionSizeC :EntityX::Component<ObjectPositionSizeC>
		{
			explicit ObjectPositionSizeC(float _x, float _y, float _z, float _w, float _h) :
				x(_x), y(_y), z(_z), w(_w), h(_h) {}
			float x, y, z, w, h;
		};

		struct MouseCollisionStateC :EntityX::Component<MouseCollisionStateC>
		{
			bool IsCollision;
		};

		class MessageHandleSystem :public EntityX::System<MessageHandleSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;

			MessageStruct m_MessageStruct;
		};

		class ObjectUpdataSystem :public EntityX::System<ObjectUpdataSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};

		class ObjectRenderSystem :public EntityX::System<ObjectRenderSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};

		class MouseCollisionCheckSystem :public EntityX::System<MouseCollisionCheckSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};
	}
}