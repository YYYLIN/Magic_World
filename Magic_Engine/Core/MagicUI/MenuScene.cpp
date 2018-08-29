#include "MenuScene.h"
#include "System/Supervisor.h"

namespace Magic
{
	namespace UI
	{

		MenuScene::MenuScene():SceneCommon("MenuScene")
		{
		}


		MenuScene::~MenuScene()
		{
		}

		bool MenuScene::OnInitialize()
		{
			EntityX::EntityX* _Supervisor = &(GetEntity().GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor);
			_Supervisor->m_systems.add<Magic::System::MouseCollisionCheckSystem>();

			return true;
		}
	}
}