#include "MagicTexture.h"

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

bool MagicTexture::Initialize(const unsigned char* Data, int _width, int _height)
{
	width = _width;
	height = _height;
	total_bytes = _width *_height * 4;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, Data);
	//	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	// 之前为pixels分配的内存可在使用glTexImage2D以后释放
	// 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）

	return true;
}

bool MagicTexture::Initialize(GLuint _texture, int w, int h)
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
	glBindTexture(GL_TEXTURE_2D, texture); //将纹理绑定到名字

	//GL_LINEAR线性插值， S T == U V ， GL_REPEAT复制，GL_NEAREST最接近的进行像素采样
	if (_data == MagicTextrue_LINEAR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

bool MagicTexture::LoadBMP(const char* file_name)
{
	GLubyte* pixels = 0;
	//	GLuint last_texture_ID;
	GLuint texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
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
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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


bool MagicTexture::LoadPNG(const char*file_name)
{
	unsigned char header[8];     //8
	int k;   //用于循环
	GLuint textureID; //贴图名字
	png_byte color_type; //图片到类型（可能会用在是否是开启来通道）
	png_byte bit_depth; //字节深度

	png_structp png_ptr; //图片
	png_infop info_ptr; //图片的信息
	int number_of_passes; //隔行扫描
	png_bytep * row_pointers;//图片的数据内容
	int row, col, pos;  //用于改变png像素排列的问题。
	GLubyte *rgba;

	FILE *fp = fopen(file_name, "rb");//以只读形式打开文件名为file_name的文件
	if (!fp)//做出相应可能的错误处理
	{
		//		fclose(fp);//关闭打开的文件！给出默认贴图
		return 0;//此处应该调用一个生成默认贴图返回ID的函数
	}
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
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}


	//老老实实按照libpng给到的说明稳定步骤来  错误处理！
	if (setjmp(png_jmpbuf(png_ptr)))

	{
		//释放占用的内存！然后关闭文件返回一个贴图ID此处应该调用一个生成默认贴图返回ID的函数

		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

		fclose(fp);

		return 0;

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
	//隔行扫描图片  这个必须要调用才能进行
	number_of_passes = png_set_interlace_handling(png_ptr);
	//将读取到的信息更新到info_ptr
	png_read_update_info(png_ptr, info_ptr);

	//读文件
	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		return 0;
	}
	rgba = new GLubyte[width * height * 4];
	//	rgba = (GLubyte*)malloc(width * height * 4);
	total_bytes = width * height * 4;
	//使用动态数组  设置长度
	row_pointers = new png_bytep[sizeof(png_bytep)* height];
	//	row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* height);

	for (k = 0; k < height; k++)
		row_pointers[k] = NULL;

	//通过扫描流里面的每一行将得到的数据赋值给动态数组       
	for (k = 0; k < height; k++)
		//row_pointers[k] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
		row_pointers[k] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr,
			info_ptr));
	//由于png他的像素是由 左-右-从顶到底 而贴图需要的像素都是从左-右-底到顶的所以在这里需要把像素内容进行一个从新排列
	//读图片
	png_read_image(png_ptr, row_pointers);
	pos = (width * height * 4) - (4 * width);
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < (4 * width); col += 4)
		{
			rgba[pos++] = row_pointers[row][col];        // red
			rgba[pos++] = row_pointers[row][col + 1];    // green
			rgba[pos++] = row_pointers[row][col + 2];    // blue
			rgba[pos++] = row_pointers[row][col + 3];    // alpha
		}
		pos = (pos - (width * 4) * 2);
	}

	//开启纹理贴图特效
	glEnable(GL_TEXTURE_2D);

	//创建纹理 
	glGenTextures(1, &textureID);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, textureID); //将纹理绑定到名字

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

	//设置纹理所用到图片数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	delete[]row_pointers;
	//	free(row_pointers);
	fclose(fp);
	texture = textureID;
	delete rgba;

	return true;
}


MagicFBOTextrue::MagicFBOTextrue()
{
	m_Textrue = 0;
	m_Depth_Stencil = 0;
}

MagicFBOTextrue::~MagicFBOTextrue()
{
	Shutdown();
}

bool MagicFBOTextrue::Initialize(int& w, int& h)
{
	width = w;
	height = h;

	glGenFramebuffersEXT(1, &m_Textrue);
	//绑定
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_Textrue);

	//分配一块RGBA贴图空间给FBO绘图使用    可以绑定多个纹理 glActiveTexture(GL_TEXTURE0);使用来区分
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//设置过滤
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//声明贴图大小及格式分配空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//framebuffer的RGBA贴图-绑定纹理与FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	total_bytes = width * height * 4;

	return true;
}

bool MagicFBOTextrue::ResetSize(int& w, int& h)
{
	this->~MagicFBOTextrue();
	return this->Initialize(w, h);
}

bool MagicFBOTextrue::CreateDepthStencil(GLenum _type)
{
	//分配zbuffer给FBO 使用 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_Textrue);

	glGenRenderbuffersEXT(1, &m_Depth_Stencil);
	//绑定
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_Depth_Stencil);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, _type, width, height);
	//绑定到当前的FBO对象上
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_Depth_Stencil);
	if (_type == GL_DEPTH24_STENCIL8_EXT)
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_Depth_Stencil);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		fprintf(stderr, "FBO #2 Error!\n");
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return true;
}

void MagicFBOTextrue::Shutdown()
{
	if (m_Textrue)
	{
		glDeleteFramebuffersEXT(1, &m_Textrue);
		m_Textrue = 0;
	}
	if (m_Depth_Stencil)
	{
		glDeleteFramebuffersEXT(1, &m_Depth_Stencil);
		m_Depth_Stencil = 0;
	}
}