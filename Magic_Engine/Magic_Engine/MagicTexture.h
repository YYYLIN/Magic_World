#pragma once

#include <iostream>
#include <string.h>
#include <gl/glew.h>
#include <glut.h>
#include <stdio.h>
#include <png.h>

#pragma comment(lib,"libpng16.lib")
#define LOAD_BMP  0x00
#define LOAD_PNG  0x01

#define BMP_Header_Length 54

#define MagicTextrue_LINEAR 0x01  //线性插值

using namespace std;

namespace Magic
{
	struct Color4
	{
		float R, G, B, A;
		Color4();
		Color4(float, float, float, float);
		Color4(unsigned char, unsigned char, unsigned char, unsigned char);
	};
}

class MagicTexture
{
public:
	MagicTexture();
	~MagicTexture();

	//加载PNG ,BMP图片
	virtual bool Initialize(const char* file_name, char);
	//加载自定义RGB4通道格式
	virtual bool Initialize(const unsigned char*, int, int);
	//加载外部格式（不会自动释放缓存需要外部自己释放）
	virtual bool Initialize(GLuint _texture, int w, int h);
	virtual void Shutdown();

	virtual void SetParameteri(unsigned char _data);

	inline virtual GLuint GetTextrue() { return texture; }

	inline virtual int GetWidth() { return width; }

	inline virtual int GetHeight() { return height; }

	inline virtual int GetTotal_bytes() { return total_bytes; }
protected:
	virtual bool LoadBMP(const char* file_name);
	virtual bool LoadPNG(const char* file_name);
protected:
	bool External;

	GLuint texture;
	int width, height, total_bytes;
};


class MagicFBOTextrue :public MagicTexture
{
public:
	enum MODE {
		COLOR4,
		DEPTH
	};

	MagicFBOTextrue();
	virtual ~MagicFBOTextrue();

	virtual bool Initialize(const int&, const int&, const MODE _mode);
	bool ResetSize(const int&, const int&);

	//参数;1.只创建深度数据      GL_DEPTH_COMPONENT32
	//     2.创建深度和模板缓存  GL_DEPTH24_STENCIL8_EXT
	bool CreateDepthStencil(GLenum);

	inline GLuint GetFBOTextrue() { return m_Textrue; }

	inline GLuint GetDepth_Stencil() { return m_Depth_Stencil; }

	void Use();
	void UnUse(const int& _w, const int& _h);

protected:
	virtual void Shutdown();
private:
	GLuint m_Textrue;//颜色缓冲区
	GLuint m_Depth_Stencil;
	MODE m_MODE;
};

