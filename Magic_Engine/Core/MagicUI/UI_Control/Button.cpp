#include "Button.h"
#include "System/Supervisor.h"
#include "Render/MagicEngineAPI.h"
#include "Render/MagicScene.h"

void RenderButton(EntityX::Entity _Entity)
{
	Magic::Pen_Normal* pPen_Normal = Magic::GetPen_Normal();
	EntityX::ComponentHandle<Magic::System::ObjectPositionSizeC> _ObjectPositionSizeC = _Entity.GetComponent<Magic::System::ObjectPositionSizeC>();
	EntityX::ComponentHandle<Magic::MouseCollisionStateC> _MouseCollisionStateC = _Entity.GetComponent<Magic::MouseCollisionStateC>();


	if (_MouseCollisionStateC->IsCollision)
		pPen_Normal->SetColor(Magic::Color4(0.0f, 1.0f, 1.0f, 1.0f));
	else
		pPen_Normal->SetColor(Magic::Color4(1.0f, 1.0f, 0.0f, 1.0f));
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, _ObjectPositionSizeC->x, _ObjectPositionSizeC->y, _ObjectPositionSizeC->w, _ObjectPositionSizeC->h);
}

bool CreateButton(EntityCommon& _ParentEntity, EntityCommon* _pCommon)
{
	if (_ParentEntity.has_component<Magic::System::ObjectSupervisor>())
	{
		*_pCommon = _ParentEntity.GetComponent<Magic::System::ObjectSupervisor>()->m_Supervisor.m_entities.create();

		_pCommon->assign<Magic::System::ObjectPositionSizeC>(200.0f, 200.0f, 0.0f, 100.0f, 50.0f);
		_pCommon->assign<Magic::System::RenderComponent>((Magic::System::Call_Entity)0,
			RenderButton, (Magic::System::Call_Entity)0);
		_pCommon->assign<Magic::MouseCollisionStateC>();

		return true;
	}
	else
	{
		Magic::SetEngineErrorMessage("1.There is no ObjectSupervisor Component\n");
		return false;
	}

	return true;
}