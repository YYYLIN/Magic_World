#include "MagicFBO.h"
#include <gl/glew.h>
#include <glut.h>
#include <stdio.h>


MagicFBO::MagicFBO()
{
	m_Textrue = 0;
	m_Depth_Stencil = 0;
	m_MultisampleNumber = 0;
}

MagicFBO::~MagicFBO()
{
	Shutdown();
}

bool MagicFBO::Initialize(const int& _w, const int& _h, const MODE _mode, unsigned char _MultisampleNumber)
{
	width = _w;
	height = _h;


	glGenFramebuffers(1, &m_Textrue);
	//绑定
	glBindFramebuffer(GL_FRAMEBUFFER, m_Textrue);

	//分配一块RGBA贴图空间给FBO绘图使用    可以绑定多个纹理 glActiveTexture(GL_TEXTURE0);使用来区分
	glGenTextures(1, &texture);

	GLenum _textrue_mode = GL_TEXTURE_2D, _attachment = GL_COLOR_ATTACHMENT0;
	m_MultisampleNumber = _MultisampleNumber;
	m_MODE = _mode;

	if (m_MultisampleNumber)
		_textrue_mode = GL_TEXTURE_2D_MULTISAMPLE;
	else
		_textrue_mode = GL_TEXTURE_2D;
	glBindTexture(_textrue_mode, texture);

	//声明贴图大小及格式分配空间
	switch (m_MODE)
	{
	case COLOR4:
	case COLOR4_DEPTH32:
	case COLOR4_DEPTH24_STENCIL8:
		//设置过滤
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (m_MultisampleNumber)
			glTexImage2DMultisample(_textrue_mode, m_MultisampleNumber, GL_RGBA, width, height, GL_TRUE);
		else
			glTexImage2D(_textrue_mode, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		_attachment = GL_COLOR_ATTACHMENT0;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	case DEPTH:
		//储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(_textrue_mode, GL_TEXTURE_BORDER_COLOR, borderColor);
		if (m_MultisampleNumber)
			glTexImage2DMultisample(_textrue_mode, m_MultisampleNumber, GL_DEPTH_COMPONENT, width, height, GL_TRUE);
		else
			glTexImage2D(_textrue_mode, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		_attachment = GL_DEPTH_ATTACHMENT;
		//不适用任何颜色数据进行渲染
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		break;
	}

	glTexParameteri(_textrue_mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(_textrue_mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//framebuffer的RGBA贴图-绑定纹理与FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, _attachment, _textrue_mode, texture, 0);

	if (m_MODE == COLOR4_DEPTH32)
		CreateDepthStencil(GL_DEPTH_COMPONENT32);
	else if (m_MODE == COLOR4_DEPTH24_STENCIL8)
		CreateDepthStencil(GL_DEPTH24_STENCIL8);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	total_bytes = width * height * 4;

	return true;
}

bool MagicFBO::ResetSize(const int& _w, const int& _h, unsigned char _MultisampleNumber)
{
	this->Shutdown();
	MagicTexture::Shutdown();
	return this->Initialize(_w, _h, m_MODE, _MultisampleNumber);
}

void MagicFBO::CopyFBOTO(MagicFBO* _pTagetFBO, int _tagetX, int _tagetY, int _tagetW, int _tagetH,
	MagicFBO* _pSourceFBO, int _sourceX, int _sourceY, int _sourceW, int _sourceH)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _pSourceFBO->GetFBOTexture());
	if (_pTagetFBO)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _pTagetFBO->GetFBOTexture());
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(_sourceX, _sourceY, _sourceX + _sourceW, _sourceY + _sourceH,
		_tagetX, _tagetY, _tagetX + _tagetW, _tagetY + _tagetH, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void MagicFBO::Clear(unsigned int _b_mode)
{
	unsigned int _mode = 0;
	if (_b_mode & BUFFER_MODE::B_COLOR)
		_mode |= GL_COLOR_BUFFER_BIT;
	if (_b_mode & BUFFER_MODE::B_DEPTH)
		_mode |= GL_DEPTH_BUFFER_BIT;
	if (_b_mode & BUFFER_MODE::B_STENCIL)
		_mode |= GL_STENCIL_BUFFER_BIT;
	glClear(_mode);
}

void MagicFBO::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Textrue);
}

void MagicFBO::UnUse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool MagicFBO::CreateDepthStencil(unsigned int _type)
{
	//分配zbuffer给FBO 使用 
	glGenRenderbuffers(1, &m_Depth_Stencil);
	//绑定
	glBindRenderbuffer(GL_RENDERBUFFER, m_Depth_Stencil);
	if (m_MultisampleNumber)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MultisampleNumber, _type, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, _type, width, height);

	//绑定到当前的FBO对象上

	if (_type == GL_DEPTH24_STENCIL8)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Depth_Stencil);
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Depth_Stencil);
	GLenum status = glCheckFramebufferStatus(GL_RENDERBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "FBO #2 Error!\n");
		return false;
	}

	return true;
}

void MagicFBO::Shutdown()
{
	if (m_Textrue)
	{
		glDeleteFramebuffers(1, &m_Textrue);
		m_Textrue = 0;
	}
	if (m_Depth_Stencil)
	{
		glDeleteFramebuffers(1, &m_Depth_Stencil);
		m_Depth_Stencil = 0;
	}
}