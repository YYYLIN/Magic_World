#include "MagicTexture.h"
#include <wchar.h>
#include <stdio.h>
#include <gl/glew.h>
#include <glut.h>
#include <png.h>


#pragma comment(lib,"libpng16.lib")

int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

Magic::Color4::Color4()
{
	R = G = B = A = 1.0f;
}

Magic::Color4::Color4(float _R, float _G, float _B, float _A)
	:R(_R), G(_G), B(_B), A(_A)
{}

Magic::Color4::Color4(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
{
	R = _R / 255;
	G = _G / 255;
	B = _B / 255;
	A = _A / 255;
}

MagicTexture::MagicTexture()
{
	External = false;
	texture = 0;
	width = height = 0;
}


MagicTexture::~MagicTexture()
{
	Shutdown();
}


bool MagicTexture::Initialize(const char* file_name, char format)
{
	bool result;

	if (format == LOAD_BMP)
	{
		result = LoadBMP(file_name);
		if (!result)
			return false;
	}
	else if (format == LOAD_PNG)
	{
		result = LoadPNG(file_name);
		if (!result)
			return false;
	}

	return true;
}

bool MagicTexture::Initialize(const unsigned char* Data, int _width, int _height, MODE _mode, FORMAT _format)
{
	width = _width;
	height = _height;
	total_bytes = _width *_height * 4;
	// ����һ���µ�������
	glGenTextures(1, &texture);
	if (texture == 0)
	{
		return false;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLint _internalformat;
	m_MODE = _mode;
	switch (m_MODE)
	{
	case MagicTexture::RGBA:
		_internalformat = GL_RGBA;
		break;
	case MagicTexture::RGB:
		_internalformat = GL_RGB;
		break;
	case MagicTexture::RED:
		_internalformat = GL_RED;
		break;
	case MagicTexture::ALPHA:
		_internalformat = GL_ALPHA;
		break;
	}
	GLenum _type;
	m_FORMAT = _format;
	switch (m_FORMAT)
	{
	case MagicTexture::UNSIGNED_BYTE:
		_type = GL_UNSIGNED_BYTE;
		break;
	case MagicTexture::BYTE:
		_type = GL_BYTE;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, _internalformat, width, height, 0,
		_internalformat, _type, Data);
	//	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	// ֮ǰΪpixels������ڴ����ʹ��glTexImage2D�Ժ��ͷ�
	// ��Ϊ��ʱ���������Ѿ���OpenGL���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�

	return true;
}

bool MagicTexture::Initialize(unsigned int _texture, int w, int h)
{
	texture = _texture;
	External = true;
	width = w;
	height = h;

	return true;
}

void MagicTexture::Shutdown()
{
	if (!External)
		if (texture)
			glDeleteTextures(1, &texture);
}

void MagicTexture::SetParameteri(unsigned char _data)
{
	glBindTexture(GL_TEXTURE_2D, texture); //������󶨵�����

	//GL_LINEAR���Բ�ֵ�� S T == U V �� GL_REPEAT���ƣ�GL_NEAREST��ӽ��Ľ������ز���
	if (_data == MagicTextrue_LINEAR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void MagicTexture::UpdataData(int _xoffset, int _yoffset, unsigned int _width, unsigned int _hegiht, void* _pdata)
{
	glBindTexture(GL_TEXTURE_2D, texture); //������󶨵�����
	GLint _internalformat;
	switch (m_MODE)
	{
	case MagicTexture::RGBA:
		_internalformat = GL_RGBA;
		break;
	case MagicTexture::RGB:
		_internalformat = GL_RGB;
		break;
	case MagicTexture::RED:
		_internalformat = GL_RED;
		break;
	case MagicTexture::ALPHA:
		_internalformat = GL_ALPHA;
		break;
	}
	GLenum _type;
	switch (m_FORMAT)
	{
	case MagicTexture::UNSIGNED_BYTE:
		_type = GL_UNSIGNED_BYTE;
		break;
	case MagicTexture::BYTE:
		_type = GL_BYTE;
		break;
	}
	glTexSubImage2D(GL_TEXTURE_2D, 0, _xoffset, _yoffset, _width, _hegiht, _internalformat, _type, _pdata);
}

bool MagicTexture::LoadBMP(const char* file_name)
{
	GLubyte* pixels = 0;
	//	GLuint last_texture_ID;
	GLuint texture_ID = 0;

	// ���ļ������ʧ�ܣ�����
	FILE* pFile = NULL;
	errno_t _err = fopen_s(&pFile, file_name, "rb");
	if (_err != 0)
		return false;

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = new GLubyte[total_bytes];
	if (pixels == 0)
	{
		fclose(pFile);
		return false;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		delete[]pixels;
		fclose(pFile);
		return false;
	}

	// �ھɰ汾��OpenGL��
	// ���ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ���ﲢû�м��OpenGL�汾�����ڶ԰汾�����ԵĿ��ǣ����ɰ汾����
	// ���⣬�����Ǿɰ汾�����°汾��
	// ��ͼ��Ŀ�Ⱥ͸߶ȳ�����ǰOpenGLʵ����֧�ֵ����ֵʱ��ҲҪ��������
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�
			new_pixels = new GLubyte[new_total_bytes];
			if (new_pixels == 0)
			{
				delete[]pixels;
				fclose(pFile);
				return false;
			}

			// ������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			delete[]pixels;
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		delete[]pixels;
		fclose(pFile);
		return false;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	m_MODE = RGB;
	m_FORMAT = UNSIGNED_BYTE;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	//	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	// ֮ǰΪpixels������ڴ����ʹ��glTexImage2D�Ժ��ͷ�
	// ��Ϊ��ʱ���������Ѿ���OpenGL���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�
	delete[]pixels;
	fclose(pFile);
	texture = texture_ID;

	return true;
}


bool MagicTexture::LoadPNG(const char*file_name)
{
	unsigned char header[8];     //8
	png_byte color_type; //ͼƬ�����ͣ����ܻ������Ƿ��ǿ�����ͨ����
	png_byte bit_depth; //�ֽ����

	png_structp png_ptr; //ͼƬ
	png_infop info_ptr; //ͼƬ����Ϣ

	FILE* fp = NULL;
	errno_t _err = fopen_s(&fp, file_name, "rb");
	if (_err != 0)
		return false;
	//��ȡ�ļ�ͷ�ж��Ƿ���pngͼƬ.������������Ӧ����
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
	{
		fclose(fp);
		return 0; //ÿ����������һ���ģ���������֮��������Ҫ����Сʱ������
	}

	//����libpng��libpng-manual.txt��˵��ʹ���ĵ� �����������ʼ��png_structp �� png_infop
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //�������ǰ󶨴����Լ�����ĺ�����������Ϊ��
	if (!png_ptr)//������Ӧ����ʼ��ʧ�ܵĴ���
	{
		fclose(fp);
		return 0;
	}
	//���ݳ�ʼ����png_ptr��ʼ��png_infop
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		//��ʼ��ʧ���Ժ�����png_structp
		png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr);
		fclose(fp);
		return 0;
	}

	//������
	if (setjmp(png_jmpbuf(png_ptr)))

	{
		//�ͷ�ռ�õ��ڴ棡Ȼ��ر��ļ�����һ����ͼID�˴�Ӧ�õ���һ������Ĭ����ͼ����ID�ĺ���
		png_destroy_info_struct(png_ptr, &info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr);
		fclose(fp);
		return false;

	}
	//����Ҫȷ����ͨ��2���ƴ򿪵��ļ���ͨ��i/o���ƺ���png_init_io
	png_init_io(png_ptr, fp);
	//�ƺ���˵Ҫ����libpng�ļ��ӵڼ�����ʼmissing
	png_set_sig_bytes(png_ptr, 8);
	//�����ֻ��򵥵Ĳ��������ڿ���ʵ�ʶ�ȡͼƬ��Ϣ�ˣ�
	png_read_info(png_ptr, info_ptr);
	//���ͼƬ����Ϣ width height ��ɫ����  �ֽ����
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	//���ͼƬ����alphaͨ������Ҫ
	/*	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	png_set_swap_alpha(png_ptr);*/

	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	/*
		//����ɨ��ͼƬ  �������Ҫ���ò��ܽ���
		int number_of_passes = png_set_interlace_handling(png_ptr);
		//����ȡ������Ϣ���µ�info_ptr
		png_read_update_info(png_ptr, info_ptr);*/
		/*
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				fclose(fp);
				return false;
			}*/

	total_bytes = width * height * 4;
	GLubyte *rgba = new GLubyte[total_bytes];

	int pos = total_bytes - (4 * width);
	for (int row = 0; row < height; row++)
	{
		png_bytep row_pointers = &rgba[pos];
		png_read_rows(png_ptr, &row_pointers, NULL, 1);
		pos -= 4 * width;
	}

	png_destroy_info_struct(png_ptr, &info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr);
	fclose(fp);

	//����������ͼ��Ч
	glEnable(GL_TEXTURE_2D);

	//�������� 
	glGenTextures(1, &texture);
	//������
	glBindTexture(GL_TEXTURE_2D, texture); //������󶨵�����

	//GL_LINEAR���Բ�ֵ�� S T == U V �� GL_REPEAT���ƣ�GL_NEAREST��ӽ��Ľ������ز���
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//������ͼ������Ļ��Ч��������Ĭ��ֻ������
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	m_MODE = RGBA;
	m_FORMAT = UNSIGNED_BYTE;
	//�����������õ�ͼƬ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	delete[] rgba;
	rgba = 0;

	return true;
}


MagicFBOTexture::MagicFBOTexture()
{
	m_Textrue = 0;
	m_Depth_Stencil = 0;
	m_MultisampleNumber = 0;
}

MagicFBOTexture::~MagicFBOTexture()
{
	Shutdown();
}

bool MagicFBOTexture::Initialize(const int& _w, const int& _h, const MODE _mode, unsigned char _MultisampleNumber)
{
	width = _w;
	height = _h;


	glGenFramebuffers(1, &m_Textrue);
	//��
	glBindFramebuffer(GL_FRAMEBUFFER, m_Textrue);

	//����һ��RGBA��ͼ�ռ��FBO��ͼʹ��    ���԰󶨶������ glActiveTexture(GL_TEXTURE0);ʹ��������
	glGenTextures(1, &texture);

	GLenum _textrue_mode = GL_TEXTURE_2D, _attachment = GL_COLOR_ATTACHMENT0;
	m_MultisampleNumber = _MultisampleNumber;
	m_MODE = _mode;

	if (m_MultisampleNumber)
		_textrue_mode = GL_TEXTURE_2D_MULTISAMPLE;
	else
		_textrue_mode = GL_TEXTURE_2D;
	glBindTexture(_textrue_mode, texture);

	//������ͼ��С����ʽ����ռ�
	switch (m_MODE)
	{
	case COLOR4:
	case COLOR4_DEPTH32:
	case COLOR4_DEPTH24_STENCIL8:
		//���ù���
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
		//����һ���߿���ɫ��Ȼ��������ͼ��������ѡ������ΪGL_CLAMP_TO_BORDER
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(_textrue_mode, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(_textrue_mode, GL_TEXTURE_BORDER_COLOR, borderColor);
		if (m_MultisampleNumber)
			glTexImage2DMultisample(_textrue_mode, m_MultisampleNumber, GL_DEPTH_COMPONENT, width, height, GL_TRUE);
		else
			glTexImage2D(_textrue_mode, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		_attachment = GL_DEPTH_ATTACHMENT;
		//�������κ���ɫ���ݽ�����Ⱦ
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		break;
	}

	glTexParameteri(_textrue_mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(_textrue_mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//framebuffer��RGBA��ͼ-��������FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, _attachment, _textrue_mode, texture, 0);

	if (m_MODE == COLOR4_DEPTH32)
		CreateDepthStencil(GL_DEPTH_COMPONENT32);
	else if (m_MODE == COLOR4_DEPTH24_STENCIL8)
		CreateDepthStencil(GL_DEPTH24_STENCIL8);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	total_bytes = width * height * 4;

	return true;
}

bool MagicFBOTexture::ResetSize(const int& _w, const int& _h, unsigned char _MultisampleNumber)
{
	this->Shutdown();
	MagicTexture::Shutdown();
	return this->Initialize(_w, _h, m_MODE, _MultisampleNumber);
}

void MagicFBOTexture::CopyFBOTO(MagicFBOTexture* _pTagetFBO, int _tagetX, int _tagetY, int _tagetW, int _tagetH,
	MagicFBOTexture* _pSourceFBO, int _sourceX, int _sourceY, int _sourceW, int _sourceH)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _pSourceFBO->GetFBOTexture());
	if (_pTagetFBO)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _pTagetFBO->GetFBOTexture());
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(_sourceX, _sourceY, _sourceX + _sourceW, _sourceY + _sourceH,
		_tagetX, _tagetY, _tagetX + _tagetW, _tagetY + _tagetH, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void MagicFBOTexture::Clear(unsigned int _b_mode)
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

void MagicFBOTexture::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Textrue);
}

void MagicFBOTexture::UnUse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool MagicFBOTexture::CreateDepthStencil(unsigned int _type)
{
	//����zbuffer��FBO ʹ�� 
	glGenRenderbuffers(1, &m_Depth_Stencil);
	//��
	glBindRenderbuffer(GL_RENDERBUFFER, m_Depth_Stencil);
	if (m_MultisampleNumber)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MultisampleNumber, _type, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, _type, width, height);

	//�󶨵���ǰ��FBO������

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

void MagicFBOTexture::Shutdown()
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