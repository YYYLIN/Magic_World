#ifndef _MAGICTEXTURE_H_
#define _MAGICTEXTURE_H_

#include "Define/Magic_Macro.h"

#define LOAD_BMP  0x00
#define LOAD_PNG  0x01

#define BMP_Header_Length 54

namespace Magic
{
	struct DLL_MAGIC_ENGINE_OUTPUT_INPUT Color4
	{
		float R, G, B, A;
		Color4();
		Color4(float, float, float, float);
		Color4(unsigned char, unsigned char, unsigned char, unsigned char);
	};
}

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicTexture
{
public:
	enum MODE {
		RGBA,
		RGB,
		RED,
		ALPHA
	};

	enum FORMAT
	{
		UNSIGNED_BYTE,
		BYTE
	};

	enum SAMPLING_MODE
	{
		LINEAR,//线性插值
		NEAREST
	};
public:
	MagicTexture();
	~MagicTexture();

	//加载PNG ,BMP图片
	virtual bool Initialize(const char* file_name, char);
	//加载自定义RGB4通道格式
	virtual bool Initialize(const unsigned char*, int, int, MODE _mode, FORMAT _format);
	//加载外部格式（不会自动释放缓存需要外部自己释放）
	virtual bool Initialize(unsigned int _texture, int w, int h);
	virtual void Shutdown();

	virtual void SetParameteri(SAMPLING_MODE _mode);

	inline virtual unsigned int GetTextrue() { return texture; }

	inline virtual int GetWidth() { return width; }

	inline virtual int GetHeight() { return height; }

	inline virtual int GetTotal_bytes() { return total_bytes; }

	void UpdataData(int _xoffset, int _yoffset, unsigned int _width, unsigned int _hegiht, void* _pdata);
protected:
	virtual bool LoadBMP(const char* file_name);
	virtual bool LoadPNG(const char* file_name);
protected:
	bool External;
	MODE m_MODE;
	FORMAT m_FORMAT;
	unsigned int texture;
	int width, height, total_bytes;
};


class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicFBOTexture :public MagicTexture
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

	MagicFBOTexture();
	virtual ~MagicFBOTexture();

	virtual bool Initialize(const int& _w, const int& _h, const MODE _mode, unsigned char _MultisampleNumber = 0);
	bool ResetSize(const int& _w, const int& _h, unsigned char _MultisampleNumber);

	inline unsigned char GetMultisampleNumber() { return m_MultisampleNumber; }

	inline unsigned int GetFBOTexture() { return m_Textrue; }

	inline unsigned int GetDepth_Stencil() { return m_Depth_Stencil; }

	static void CopyFBOTO(MagicFBOTexture* _pTagetFBO, int _tagetX, int _tagetY, int _tagetW, int _tagetH,
		MagicFBOTexture* _pSourceFBO, int _sourceX, int _sourceY, int _sourceW, int _sourceH);

	void Clear(unsigned int _b_mode);

	void Use();
	void UnUse();

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