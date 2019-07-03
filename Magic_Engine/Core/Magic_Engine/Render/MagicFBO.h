#ifndef _MAGIC_FBO_H_
#define _MAGIC_FBO_H_

#include "MagicTexture.h"

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicFBO :public MagicTexture
{
public:
	enum MODE {
		COLOR4,
		COLOR4_DEPTH32,
		COLOR4_DEPTH24_STENCIL8,
		DEPTH
	};
	enum BUFFER_MODE {
		B_COLOR = 1,
		B_DEPTH = 1 << 1,
		B_STENCIL = 1 << 2
	};

	MagicFBO();
	virtual ~MagicFBO();

	virtual bool Initialize(const int& _w, const int& _h, const MODE _mode, unsigned char _MultisampleNumber = 0);
	bool ResetSize(const int& _w, const int& _h, unsigned char _MultisampleNumber);

	inline unsigned char GetMultisampleNumber() { return m_MultisampleNumber; }

	inline unsigned int GetFBOTexture() { return m_Textrue; }

	inline unsigned int GetDepth_Stencil() { return m_Depth_Stencil; }

	static void CopyFBOTO(MagicFBO* _pTagetFBO, int _tagetX, int _tagetY, int _tagetW, int _tagetH,
		MagicFBO* _pSourceFBO, int _sourceX, int _sourceY, int _sourceW, int _sourceH);

	static void Clear(unsigned int _b_mode);

	void Use();
	static void UnUse();

protected:
	//参数;1.只创建深度数据      GL_DEPTH_COMPONENT32
	//     2.创建深度和模板缓存  GL_DEPTH24_STENCIL8
	bool CreateDepthStencil(unsigned int _type);

	virtual void Shutdown();
private:
	unsigned int m_Textrue;//颜色缓冲区
	unsigned int m_Depth_Stencil;
	unsigned char m_MultisampleNumber;
	MODE m_MODE;
};


#endif