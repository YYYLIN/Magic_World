#include "UserScene.h"
#include "System/Supervisor.h"

namespace Magic
{
	namespace UI
	{
		UserScene::UserScene():SceneCommon("UserScene")
		{
		}


		UserScene::~UserScene()
		{
		}

		bool UserScene::OnInitialize()
		{
			EntityX::EntityX* _Supervisor = &(GetEntity().GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor);
			_Supervisor->m_systems.add<Magic::MouseCollisionCheckSystem>();

			return true;
		}
	}
}