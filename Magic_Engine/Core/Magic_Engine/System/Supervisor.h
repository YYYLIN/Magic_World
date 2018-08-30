#pragma once

#include "Define/MagicType.h"
#include "glm.hpp"
#include "windows.h"
#include "Render/MagicPen.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Magic
{
	namespace System
	{
		struct DLL_MAGIC_ENGINE_OUTPUT_INPUT ObjectSupervisor : EntityX::Component<ObjectSupervisor>
		{
			explicit ObjectSupervisor(EntityCommon _pParent = EntityCommon()) :pParentSupervisor(_pParent) {}
			ObjectSupervisor(const ObjectSupervisor& _ObjectSupervisor)
			{
				memcpy(this, &_ObjectSupervisor, sizeof(ObjectSupervisor));
			}

			EntityX::EntityX m_Supervisor;
			EntityCommon pParentSupervisor;
		};

		struct RenderComponent : EntityX::Component<RenderComponent>
		{
			explicit RenderComponent(Call_Entity _Call_RenderStart = 0, Call_Entity _Call_Render = 0, Call_Entity _Call_RenderEnd = 0)
				: m_Call_RenderStart(_Call_RenderStart), m_Call_Render(_Call_Render), m_Call_RenderEnd(_Call_RenderEnd) {}

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

		struct ThreadsComponent :EntityX::Component<ThreadsComponent>
		{
			explicit ThreadsComponent();
			~ThreadsComponent();

			std::vector<ObjectMessageStruct> m_vec_ObjectMessageStruct;
			std::map<std::string, SceneCommonBox> M_SceneCommonBox;
			HANDLE m_Threads;
			CRITICAL_SECTION m_MutexObjectMessage;
			CRITICAL_SECTION m_MutexSceneCommon;
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

		class MessageHandleSystem :public EntityX::System<MessageHandleSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;

			MessageStruct m_MessageStruct;
			int m_Return;
		};

		/*
		*功能：
		*	线程消息转发系统
		*警告：
		*	只能添加到线程对象中
		*/
		class ThreadsMessageForwardSystem :public EntityX::System<ThreadsMessageForwardSystem>
		{
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};

		class ObjectUpdataSystem :public EntityX::System<ObjectUpdataSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};

		/*
		*功能：
		*	线程渲染处理系统
		*警告：
		*	只能添加到线程对象中
		*/
		class ThreadsRenderSystem :public EntityX::System<ThreadsRenderSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};

		class ObjectRenderSystem :public EntityX::System<ObjectRenderSystem>
		{
		public:
			virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;
		};
	}
}

#pragma warning(pop)