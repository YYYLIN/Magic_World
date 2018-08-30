#include "BaseWindow.h"
#include "System/Supervisor.h"

namespace Magic
{
	namespace UI
	{
		BaseWindow::BaseWindow(const char* _name) :SceneCommon(_name)
		{
			m_DisplayState = true;
		}

		void BaseWindow::SetDisplayState(bool _State)
		{
			m_DisplayState = _State;
		}

		bool BaseWindow::Initialize(const EntityCommon& _ParentEntity, bool _AutoRelease)
		{
			bool _result = SceneCommon::Initialize(_ParentEntity, _AutoRelease);
			if (!_result)
				return false;

			EntityX::EntityX* _Supervisor = &(GetEntity().GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor);

			GetEntity().assign<Magic::MouseCollisionStateC>();

			_Supervisor->m_systems.add<Magic::MouseCollisionCheckSystem>();

			return true;
		}

		int BaseWindow::MessageHandle(const Magic::System::MessageStruct& _MessageStruct)
		{
			if (m_DisplayState)
			{
				return this->OnMessage(_MessageStruct);
			}

			return MAGIC_RETURN_OK;
		}

		void BaseWindow::Updata()
		{
			if (m_DisplayState)
				this->OnUpdata();
		}

		void BaseWindow::Render()
		{
			if (m_DisplayState)
				this->Draw();
		}

		int BaseWindow::OnMessage(const Magic::System::MessageStruct& _MessageStruct)
		{
			return MAGIC_RETURN_OK;
		}

		void BaseWindow::OnUpdata()
		{

		}

		void BaseWindow::Draw()
		{

		}
	}
}

