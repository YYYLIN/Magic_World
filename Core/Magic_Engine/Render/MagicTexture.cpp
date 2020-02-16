#include "MagicTexture.h"
#include <wchar.h>
#include <stdio.h>
#include <gl/glew.h>
#include <png.h>

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
	R = (float)(_R / 255);
	G = (float)(_G / 255);
	B = (float)(_B / 255);
	A = (float)(_A / 255);
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
	total_bytes = _width * _height * 4;
	// 分配一个新的纹理编号
	glGenTextures(1, &texture);
	if (texture == 0)
	{
		return false;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

	// 之前为pixels分配的内存可在使用glTexImage2D以后释放
	// 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）

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

void MagicTexture::SetParameteri(SAMPLING_MODE _mode)
{
	glBindTexture(GL_TEXTURE_2D, texture); //将纹理绑定到名字

	//GL_LINEAR线性插值， S T == U V ， GL_REPEAT复制，GL_NEAREST最接近的进行像素采样
	switch (_mode)
	{
	case MagicTexture::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case MagicTexture::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
}

void MagicTexture::UpdataData(int _xoffset, int _yoffset, unsigned int _width, unsigned int _hegiht, void* _pdata)
{
	glBindTexture(GL_TEXTURE_2D, texture); //将纹理绑定到名字
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
	//4.0以前的只能为2的次方
	glTexSubImage2D(GL_TEXTURE_2D, 0, _xoffset, _yoffset, _width, _hegiht, _internalformat, _type, _pdata);
}

bool MagicTexture::LoadBMP(const char* file_name)
{
	GLubyte* pixels = 0;
	//	GLuint last_texture_ID;
	GLuint texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile = NULL;
	errno_t _err = fopen_s(&pFile, file_name, "rb");
	if (_err != 0)
		return false;

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = new GLubyte[total_bytes];
	if (pixels == 0)
	{
		fclose(pFile);
		return false;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		delete[]pixels;
		fclose(pFile);
		return false;
	}

	// 在旧版本的OpenGL中
	// 如果图象的宽度和高度不是的整数次方，则需要进行缩放
	// 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
	// 另外，无论是旧版本还是新版本，
	// 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存
			new_pixels = new GLubyte[new_total_bytes];
			if (new_pixels == 0)
			{
				delete[]pixels;
				fclose(pFile);
				return false;
			}

			// 进行像素缩放
			/*gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);*/

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			delete[]pixels;
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		delete[]pixels;
		fclose(pFile);
		return false;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	m_MODE = RGB;
	m_FORMAT = UNSIGNED_BYTE;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	//	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	// 之前为pixels分配的内存可在使用glTexImage2D以后释放
	// 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
	delete[]pixels;
	fclose(pFile);
	texture = texture_ID;

	return true;
}


bool MagicTexture::LoadPNG(const char* file_name)
{
	unsigned char header[8];     //8
	png_byte color_type; //图片到类型（可能会用在是否是开启来通道）
	png_byte bit_depth; //字节深度

	png_structp png_ptr; //图片
	png_infop info_ptr; //图片的信息

	FILE* fp = NULL;
	errno_t _err = fopen_s(&fp, file_name, "rb");
	if (_err != 0)
		return false;
	//读取文件头判断是否所png图片.不是则做出相应处理
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
	{
		fclose(fp);
		return 0; //每个错误处理都是一样的！这样报错之后锁定就要花点小时间来！
	}

	//根据libpng的libpng-manual.txt的说明使用文档 接下来必须初始化png_structp 和 png_infop
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //后三个是绑定错误以及警告的函数这里设置为空
	if (!png_ptr)//做出相应到初始化失败的处理
	{
		fclose(fp);
		return 0;
	}
	//根据初始化的png_ptr初始化png_infop
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		//初始化失败以后销毁png_structp
		png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr);
		fclose(fp);
		return 0;
	}

	//错误处理！
	if (setjmp(png_jmpbuf(png_ptr)))

	{
		//释放占用的内存！然后关闭文件返回一个贴图ID此处应该调用一个生成默认贴图返回ID的函数
		png_destroy_info_struct(png_ptr, &info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr);
		fclose(fp);
		return false;

	}
	//你需要确保是通过2进制打开的文件。通过i/o定制函数png_init_io
	png_init_io(png_ptr, fp);
	//似乎是说要告诉libpng文件从第几个开始missing
	png_set_sig_bytes(png_ptr, 8);
	//如果你只想简单的操作你现在可以实际读取图片信息了！
	png_read_info(png_ptr, info_ptr);
	//获得图片到信息 width height 颜色类型  字节深度
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	//如果图片带有alpha通道就需要
	/*	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	png_set_swap_alpha(png_ptr);*/

	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	/*
		//隔行扫描图片  这个必须要调用才能进行
		int number_of_passes = png_set_interlace_handling(png_ptr);
		//将读取到的信息更新到info_ptr
		png_read_update_info(png_ptr, info_ptr);*/
		/*
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				fclose(fp);
				return false;
			}*/

	total_bytes = width * height * 4;
	GLubyte* rgba = new GLubyte[total_bytes];

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

	//开启纹理贴图特效
	glEnable(GL_TEXTURE_2D);

	//创建纹理 
	glGenTextures(1, &texture);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture); //将纹理绑定到名字

	//GL_LINEAR线性插值， S T == U V ， GL_REPEAT复制，GL_NEAREST最接近的进行像素采样
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//设置贴图和纹理的混合效果这里是默认只用纹理
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	m_MODE = RGBA;
	m_FORMAT = UNSIGNED_BYTE;
	//设置纹理所用到图片数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	delete[] rgba;
	rgba = 0;

	return true;
}