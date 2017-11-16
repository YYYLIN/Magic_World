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

	virtual glm::vec2 GetFrameBufferSize();

	inline virtual GLuint GetFBOTextrue() { return m_FBOBuffer.GetFBOTextrue(); }
	inline virtual GLuint GetTextrue() { return m_FBOBuffer.GetTextrue(); }
protected:
	virtual bool Initialize(MagicScene* _scene, glm::vec4 _PosSize);
	virtual void Render(glm::vec2 _DrawPos);
	virtual void RenderBuffer();

	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool m_DrawMessage;

	MagicFBOTextrue m_FBOBuffer;
};