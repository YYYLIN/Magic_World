#pragma once


#include "Define/MagicType.h"
#include "Define/Magic_Macro.h"
#include "glm.hpp"
#include "System/Element.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Magic
{
	struct DLL_MAGIC_ENGINE_OUTPUT_INPUT MouseCollisionStateC :EntityX::Component<MouseCollisionStateC>
	{
		explicit MouseCollisionStateC(bool _State = false) :IsCollision(_State) {}
		bool IsCollision;
	};

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT MouseCollisionCheckSystem :public EntityX::System<MouseCollisionCheckSystem>
	{
	public:
		virtual void Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time) override;

		glm::vec2 m_MousePos;
	};

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT SceneCommon :public Magic::System::Object
	{
	public:
		SceneCommon(const char* _name);
		virtual ~SceneCommon();

		int C_MessageHandle(EntityX::Entity _Entity, const Magic::System::MessageStruct& _MessageStruct);
		void C_Updata(EntityX::Entity _Entity);
		void C_RenderStart(EntityCommon _Entity);
		void C_Render(EntityCommon _Entity);
		void C_RenderEnd(EntityCommon _Entity);

		virtual bool OnInitialize() = 0;

		inline const char* GetName() { return m_Name.c_str(); }

		//此函数只能调用一次
		virtual bool Initialize(const EntityCommon& _ParentEntity, bool _AutoRelease);

	protected:
		virtual int MessageHandle(const Magic::System::MessageStruct& _MessageStruct);
		virtual void Updata();
		virtual void RenderStart();
		virtual void Render();
		virtual void RenderEnd();

	private:
		std::string m_Name;
	};

	template <typename T>
	bool CreateScene(EntityCommon _ParentEntity, T** _SceneCommon)
	{
		bool _AutoRelase = false;
		if (!*_SceneCommon)
		{
			*_SceneCommon = new T;
			_AutoRelase = true;
		}

		bool _result = (*_SceneCommon)->Initialize(_ParentEntity, _AutoRelase);
		if (!_result)
			return false;

		return true;
	}

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateScene(SceneCommon* _SceneCommon, EntityCommon* _EntityCommon);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateScene(EntityCommon _ParentEntity, EntityCommon* _EntityCommon);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool DeleteScene(const char* _name);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetScenePosSize(EntityCommon _EntityCommon, const glm::vec2& _pos, const glm::vec2& _size);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetScenePos(EntityCommon _EntityCommon, const glm::vec2& _pos);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneSize(EntityCommon _EntityCommon, const glm::vec2& _size);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallUpdata(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRenderStart(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRender(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRenderEnd(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	/*
	*功能：
	*	获取当前线程资源中的窗口
	*线程保护：
	*	有
	*参数：
	*	_name = 窗口名字
	*返回值：
	*	Magic::SceneCommon* = 窗口指针
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(const char* _name);

	/*
	*功能：
	*	获取指定线程资源中的窗口
	*线程保护：
	*	有
	*参数：
	*	_ThreadsName = 线程名字
	*	_SceneName = 窗口名字
	*返回值：
	*	Magic::SceneCommon* = 窗口指针
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(const char* _ThreadsName, const char* _SceneName);

	/*
	*功能：
	*	获取指定线程资源中的窗口
	*线程保护：
	*	有
	*参数：
	*	_ThreadsEntity = 线程对象
	*	_SceneName = 窗口名字
	*返回值：
	*	Magic::SceneCommon* = 窗口指针
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(EntityCommon& _ThreadsEntity, const char* _SceneName);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2 GetScenePos(EntityCommon _EntityCommon);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2 GetSceneSize(EntityCommon _EntityCommon);

	/*
	*功能：
	*	获取从相对当前窗口的某个位置转换到相对某一父亲窗口的相对位置
	*警告：
	*	必须是当前窗口的父亲窗口
	*参数：
	*	[IN]_Pos = 输入位置
	*	[IN]_NowScene = 当前对象窗口
	*	[IN]_ParentsScene = 需要转换到的父亲窗口
	*返回值：
	*	glm::vec2 = 当前窗口
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2 GetNowSceneTOParentsScenePos(glm::vec2 _Pos, EntityCommon _NowScene, const EntityCommon& _ParentsScene);
}
#pragma warning(pop)