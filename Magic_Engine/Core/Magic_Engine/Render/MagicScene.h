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

		virtual bool OnInitialize() = 0;

		inline EntityCommon GetEntity() { return m_Entity; }
		inline const char* GetName() { return m_Name.c_str(); }

		//此函数只能调用一次
		bool Initialize(const EntityCommon& _ParentEntity, Magic::SceneCommon* _pSceneCommon, bool _AutoRelease);
	private:
		EntityCommon m_Entity;
		std::string m_Name;
	};

	void SceneRenderStart(EntityCommon _Entity);
	void SceneRenderEnd(EntityCommon _Entity);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateScene(SceneCommon* _SceneCommon, EntityCommon* _EntityCommon);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateScene(EntityCommon _ParentEntity, EntityCommon* _EntityCommon);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetScenePosSize(EntityCommon _EntityCommon, const glm::vec2& _pos, const glm::vec2& _size);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetScenePos(EntityCommon _EntityCommon, const glm::vec2& _pos);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneSize(EntityCommon _EntityCommon, const glm::vec2& _size);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallUpdata(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRenderStart(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRender(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetSceneCallRenderEnd(EntityCommon _EntityCommon, Magic::System::Call_Entity _call);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::SceneCommon* GetSceneCommon(const char* _name);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2& GetScenePos(EntityCommon _EntityCommon);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const glm::vec2& GetSceneSize(EntityCommon _EntityCommon);
}
#pragma warning(pop)