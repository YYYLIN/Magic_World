#pragma once

#include "MagicEngineContext.h"
#include "MagicModelObject.h"
#include "MagicMessage.h"

class MagicWorld :public MagicCommon, public MagicMessageCommon
{
public:
	MagicWorld();
	~MagicWorld();

	bool Initialize();
	void Shutdown();

	virtual int MessageHandle(unsigned int _MessageType, unsigned int _Message);

	void AddModelObject(MagicModelObject* _pMagicModelObject);
protected:
	virtual void OnUpdata();
	virtual	void Render(glm::vec2 _DrawPos);

	void RenderLightModel(MagicShader* _pShader, MagicModelObject* _pMagicModelObject);
private:
	MagicModelObject m_MagicModelObject;
	MagicModelObject m_GroundModelObject, m_BallModelObject;
	MagicFBOTextrue m_DepthFBO;

	MagicShader* pLightShader, *pDepthShader;

	glm::mat4 m_LightMatrix;
	glm::mat4 m_CameraMatrix;

	std::vector<MagicModelObject*> G_MagicModelObject;
};

