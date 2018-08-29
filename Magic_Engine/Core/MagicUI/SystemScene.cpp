#include "SystemScene.h"
#include "System/Supervisor.h"

namespace Magic
{
	namespace UI
	{

		SystemScene::SystemScene() :SceneCommon("SystemScene")
		{
		}


		SystemScene::~SystemScene()
		{
		}

		bool SystemScene::OnInitialize()
		{
			EntityX::EntityX* _Supervisor = &(GetEntity().GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor);
			_Supervisor->m_systems.add<Magic::System::MouseCollisionCheckSystem>();

			return true;
		}
	}
}