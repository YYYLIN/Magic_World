#pragma once
#include "MagicEngineContext.h"

struct SceneExMessage
{
	unsigned int Message;
	unsigned int Parameter;
	SceneExMessage();
	SceneExMessage(unsigned int, unsigned int);
};

class MagicSceneEx :public MagicScene
{
public:
	MagicSceneEx();

	virtual void DrawSpirit();
	virtual void SetDisplayState(bool);

	inline virtual GLuint GetFBOTextrue() { return m_FBOBuffer.GetFBOTextrue(); }
	inline virtual GLuint GetTextrue() { return m_FBOBuffer.GetTextrue(); }
protected:
	virtual bool Initialize(glm::vec4 _PosSize);
	virtual void Render(glm::mat4 CameraMatrix);
	virtual	void RenderReset(glm::mat4& CameraMatrix);

protected:
	bool m_DrawMessage;

	MagicFBOTextrue m_FBOBuffer;
};