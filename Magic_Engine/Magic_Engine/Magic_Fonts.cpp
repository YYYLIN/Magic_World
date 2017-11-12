#include "Magic_Fonts.h"


FontsTexture::FontsTexture()
{
	rows = 0;
	width = 0;
	X = 0;
	Y = 0;
}

Magic_Fonts::Magic_Fonts()
{
	textureID = 0;
	fontstexture = 0;
	MapSize = 0;
//	library = 0;
}

Magic_Fonts::~Magic_Fonts()
{
	Shutdown();
}

bool Magic_Fonts::Initialize(const char*  filepathname, int width, int height)
{
	int error;
	wstring Text;
/*	// 1. 初始化freetype2库  
	error = FT_Init_FreeType(&library);
	if (error)
		return false;

	// 2. 创建一个face  
	error = FT_New_Face(library,
		filepathname, 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		//可以打开和读这个文件，但不支持它的字体格式 
	}
	else if (error)
	{
		//其它的错误码意味着这个字体文件不能打开和读，或者简单的说它损坏了... 
		return false;
	}*/
	for (int a = 32; a < 127; a++)
		Text += a;
	Text += L"°";

	if (width > 32)
	{
		width = 32;
	}
	if (height > 32)
	{
		height = 32;
	}
	error = GetFontsTexture(Text, width, height);
	if (!error)
		return false;

	return true;
}

bool Magic_Fonts::Initialize(const char* file_name, int width1)
{
	bool result;
	int x = 0, y = 0;

	result = LoadPNG(file_name);
	if (!result)
		return false;

	fontstexture = new FontsTexture[XY_FONTS_CharNumber];
	if (!fontstexture)
		return false;

	int h = 0, b = 0;
	for (int a = 32; a < XY_FONTS_CharNumber + 32; a++)
	{
		if (h >= 16)
		{
			y += 32;
			x = 0;
			h = 0;
		}
		if (a == 129)
		{
			fontstexture[b].width = 17;
		}
		else if (a == ':')
		{
			fontstexture[b].width = 4;
		}
		else if (a == '"')
		{
			fontstexture[b].width = width1 / 2 + 3;
		}
		else if (a == '.' || a == ',' || a == '\'')
		{
			fontstexture[b].width = width1 / 2 - 1;
		}
		else if (a == '(' || a == ')' || a == 127)
			fontstexture[b].width = width1 / 2 + 1;
		else if (a >= '0' && a <= 'Z' || a == '/' || a == '-' || a == '_' || a == '%' || a == '+' || a == 128 || a == 130)
		{
			fontstexture[b].width = width1;
		}
		else if (a == 'l' || a == 'i')
			fontstexture[b].width = width1 / 2;
		else if (a >= 'a' && a <= 'z' || a == ' ')
			fontstexture[b].width = width1 - 2;
		else
		{
			fontstexture[b].width = 14;
		}
		fontstexture[b].rows = 23;
		fontstexture[b].left = 0;
		fontstexture[b].top = 0;
		fontstexture[b].Ascender = 0;
		fontstexture[b].X = x + 8;
		fontstexture[b].Y = y + 6;
		x += 32;

		h++;
		b++;
	}

	MapSize = 512;

	return true;
}

void Magic_Fonts::Shutdown()
{
	if (fontstexture)
	{
		glDeleteTextures(1, &textureID);
		delete[]fontstexture;
		fontstexture = 0;
	}
	textureID = 0;
/*	if (library)
	{
		FT_Done_FreeType(library);
		library = 0;
	}*/
}

int Magic_Fonts::GetTextAllWidth(const char* _text)
{
	int _width = 0;

	for (unsigned int a = 0; a < strlen((char*)_text); a++)
	{
		_width += fontstexture[(unsigned char)_text[a] - 32].width;
	}

	return _width;
}

bool Magic_Fonts::GetFontsTexture(wstring text, float width, float high)
{
//	int error;

/*	unsigned char* Buffer = 0;
	int x = 0, y = 0;

	Buffer = new unsigned char[1024 * 1024];
	memset(Buffer, 0, 1024 * 1024);
	unsigned char(*BufferMap)[1024] = (unsigned char(*)[1024])Buffer;

	FT_UInt glyph_index;

	// 3. 设置字体尺寸  
	error = FT_Set_Char_Size(face, 0, 16 << 6, width * 8, high * 8);
	if (error)
	{
		return false;
	}

	fontstexture = new FontsTexture[text.size()];

	wchar_t *wChar = const_cast<wchar_t *>(text.c_str());

	for (int a = 0; a < (int)text.size(); a++)
	{
		if (wChar[a] == ' ')
		{
			fontstexture[a].width = 0;
			fontstexture[a].rows = 0;
			fontstexture[a].left = width / 4;
			fontstexture[a].top = high / 4;
			fontstexture[a].Ascender = high / 4;
		}
		else
		{
			//从字符码检索字形索引
			glyph_index = FT_Get_Char_Index(face, wChar[a]);
			//装载字形图像到字形槽（将会抹掉先前的字形图像） 
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error) { return false; }

			//转换为一个抗锯齿位图
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (error) { return false; }

			fontstexture[a].width = face->glyph->bitmap.width;
			fontstexture[a].rows = face->glyph->bitmap.rows;
			fontstexture[a].left = face->glyph->bitmap_left;
			fontstexture[a].top = face->glyph->bitmap_top;
			fontstexture[a].Ascender = face->size->metrics.ascender >> 6;

			fontstexture[a].X = x;
			fontstexture[a].Y = y;
			int Number = 0;
			for (unsigned int h = 0; h < fontstexture[a].rows; h++)
			{
				for (unsigned int w = 0; w < fontstexture[a].width; w++)
				{
					BufferMap[h + y][w + x] = face->glyph->bitmap.buffer[Number];
					Number++;
				}
			}
		}
		x += 64;
		if (x > 960)
		{
			x = 0;
			y += 64;
		}
		if (a == 94)
		{
			a = 94;
		}
	}
	glGenTextures(1, &textureID);
	if (textureID == 0)
	{
		return false;
	}
	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1024, 1024, 0,
		GL_ALPHA, GL_UNSIGNED_BYTE, Buffer);
	delete[]Buffer;

	MapSize = 1024;*/

	return true;
}


bool Magic_Fonts::LoadPNG(const char* file_name)
{
	unsigned char header[8];     //8
	int k;   //用于循环
	int width, height; //记录图片到宽和高
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
	if (setjmp(png_jmpbuf(png_ptr))){
		fclose(fp);
		return 0;
	}
	rgba = new GLubyte[width * height * 4];
	//	rgba = (GLubyte*)malloc(width * height * 4);
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
	pos = 0;
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < (4 * width); col += 4)
		{
			rgba[pos++] = row_pointers[row][col];        // red
			rgba[pos++] = row_pointers[row][col + 1];    // green
			rgba[pos++] = row_pointers[row][col + 2];    // blue
			rgba[pos++] = row_pointers[row][col + 3];    // alpha
		}
	}

	//创建纹理 
	glGenTextures(1, &textureID);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, textureID); //将纹理绑定到名字

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	//设置贴图和纹理的混合效果这里是默认只用纹理

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//设置纹理所用到图片数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[]row_pointers;
	//	free(row_pointers);
	fclose(fp);

	return true;
}