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
/*	// 1. ��ʼ��freetype2��  
	error = FT_Init_FreeType(&library);
	if (error)
		return false;

	// 2. ����һ��face  
	error = FT_New_Face(library,
		filepathname, 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		//���Դ򿪺Ͷ�����ļ�������֧�����������ʽ 
	}
	else if (error)
	{
		//�����Ĵ�������ζ����������ļ����ܴ򿪺Ͷ������߼򵥵�˵������... 
		return false;
	}*/
	for (int a = 32; a < 127; a++)
		Text += a;
	Text += L"��";

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

	// 3. ��������ߴ�  
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
			//���ַ��������������
			glyph_index = FT_Get_Char_Index(face, wChar[a]);
			//װ������ͼ�����βۣ�����Ĩ����ǰ������ͼ�� 
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error) { return false; }

			//ת��Ϊһ�������λͼ
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
	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
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
	int k;   //����ѭ��
	int width, height; //��¼ͼƬ����͸�
	png_byte color_type; //ͼƬ�����ͣ����ܻ������Ƿ��ǿ�����ͨ����
	png_byte bit_depth; //�ֽ����

	png_structp png_ptr; //ͼƬ
	png_infop info_ptr; //ͼƬ����Ϣ
	int number_of_passes; //����ɨ��
	png_bytep * row_pointers;//ͼƬ����������
	int row, col, pos;  //���ڸı�png�������е����⡣
	GLubyte *rgba;

	FILE *fp = fopen(file_name, "rb");//��ֻ����ʽ���ļ���Ϊfile_name���ļ�
	if (!fp)//������Ӧ���ܵĴ�����
	{
		//		fclose(fp);//�رմ򿪵��ļ�������Ĭ����ͼ
		return 0;//�˴�Ӧ�õ���һ������Ĭ����ͼ����ID�ĺ���
	}
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
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}


	//����ʵʵ����libpng������˵���ȶ�������  ������
	if (setjmp(png_jmpbuf(png_ptr)))

	{
		//�ͷ�ռ�õ��ڴ棡Ȼ��ر��ļ�����һ����ͼID�˴�Ӧ�õ���һ������Ĭ����ͼ����ID�ĺ���

		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

		fclose(fp);

		return 0;

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
	//����ɨ��ͼƬ  �������Ҫ���ò��ܽ���
	number_of_passes = png_set_interlace_handling(png_ptr);
	//����ȡ������Ϣ���µ�info_ptr
	png_read_update_info(png_ptr, info_ptr);

	//���ļ�
	if (setjmp(png_jmpbuf(png_ptr))){
		fclose(fp);
		return 0;
	}
	rgba = new GLubyte[width * height * 4];
	//	rgba = (GLubyte*)malloc(width * height * 4);
	//ʹ�ö�̬����  ���ó���
	row_pointers = new png_bytep[sizeof(png_bytep)* height];
	//	row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* height);

	for (k = 0; k < height; k++)
		row_pointers[k] = NULL;

	//ͨ��ɨ���������ÿһ�н��õ������ݸ�ֵ����̬����       
	for (k = 0; k < height; k++)
		//row_pointers[k] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
		row_pointers[k] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr,
		info_ptr));
	//����png������������ ��-��-�Ӷ����� ����ͼ��Ҫ�����ض��Ǵ���-��-�׵�����������������Ҫ���������ݽ���һ����������
	//��ͼƬ
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

	//�������� 
	glGenTextures(1, &textureID);
	//������
	glBindTexture(GL_TEXTURE_2D, textureID); //������󶨵�����

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	//������ͼ������Ļ��Ч��������Ĭ��ֻ������

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//�����������õ�ͼƬ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[]row_pointers;
	//	free(row_pointers);
	fclose(fp);

	return true;
}