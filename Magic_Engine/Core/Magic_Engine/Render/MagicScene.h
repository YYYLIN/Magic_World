#pragma once
#include "Define/MagicType.h"
#include "Define/Magic_Macro.h"
#include "glm.hpp"
#pragma warning(push)
#pragma warning(disable:4251)

namespace Magic
{
	class DLL_MAGIC_ENGINE_OUTPUT_INPUT SceneCommon
	{
	public:
		SceneCommon(const char* _name);
		virtual ~SceneCommon();

		static int S_MessageHandle(EntityX::Entity _Entity, const Magic::System::MessageStruct& _MessageStruct);
		static void S_Updata(EntityX::Entity _Entity);
		static void S_RenderStart(EntityCommon _Entity);
		static void S_Render(EntityCommon _Entity);
		static void S_RenderEnd(EntityCommon _Entity);

		virtual bool OnInitialize() = 0;

		inline EntityCommon GetEntity() { return m_Entity; }
		inline const char* GetName() { return m_Name.c_str(); }

		//�˺���ֻ�ܵ���һ��
		virtual bool Initialize(const EntityCommon& _ParentEntity, bool _AutoRelease);

	protected:
		virtual int MessageHandle(const Magic::System::MessageStruct& _MessageStruct);
		virtual void Updata();
		virtual void RenderStart();
		virtual void Render();
		virtual void RenderEnd();

	private:
		EntityCommon m_Entity;
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

		bool _result = (*_SceneCommon)->SceneCommon::Initialize(_ParentEntity, _AutoRelase);
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
	*���ܣ�
	*	��ȡ��ǰ�߳���Դ�еĴ���
	*�̱߳�����
	*	��
	*������
	*	_name = ��������
	*����ֵ��
	*	Magic::SceneCommon* = ����ָ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(const char* _name);

	/*
	*���ܣ�
	*	��ȡָ���߳���Դ�еĴ���
	*�̱߳�����
	*	��
	*������
	*	_ThreadsName = �߳�����
	*	_SceneName = ��������
	*����ֵ��
	*	Magic::SceneCommon* = ����ָ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(const char* _ThreadsName, const char* _SceneName);

	/*
	*���ܣ�
	*	��ȡָ���߳���Դ�еĴ���
	*�̱߳�����
	*	��
	*������
	*	_ThreadsEntity = �̶߳���
	*	_SceneName = ��������
	*����ֵ��
	*	Magic::SceneCommon* = ����ָ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(EntityCommon& _ThreadsEntity, const char* _SceneName);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2& GetScenePos(EntityCommon _EntityCommon);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2& GetSceneSize(EntityCommon _EntityCommon);
}
#pragma warning(pop)