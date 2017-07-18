#pragma once

#include "MagicEngineContext.h"
#include "MagicModelObject.h"

class MagicWorld :public MagicCommon
{
public:
	MagicWorld();
	~MagicWorld();

	bool Initialize();
	void Shutdown();

	void AddModelObject(MagicModelObject* _pMagicModelObject);
protected:
	virtual void Updata();
	virtual	void Render(glm::mat4 CameraMatrix);

	void RenderLightModel(glm::mat4 CameraMatrix, MagicModelObject* _pMagicModelObject);
private:
	MagicModelObject m_MagicModelObject;
	MagicShader* pLightShader;

	vector<MagicModelObject*> G_MagicModelObject;
};

