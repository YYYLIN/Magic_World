#include "DebugScene.h"
#include "System/Supervisor.h"

namespace Magic
{
	namespace UI
	{

		DebugScene::DebugScene():SceneCommon("DebugScene")
		{
		}


		DebugScene::~DebugScene()
		{
		}

		bool DebugScene::OnInitialize()
		{
			EntityX::EntityX* _Supervisor = &(GetEntity().GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor);
			_Supervisor->m_systems.add<Magic::MouseCollisionCheckSystem>();

			return true;
		}
	}
}
