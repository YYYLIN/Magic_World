#include "Magic_Fonts.h"

#include <glut.h>


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

	return true;
}

bool Magic_Fonts::Initialize(const char* file_name, int width1)
{
	int x = 0, y = 0;


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
	return _width;
}