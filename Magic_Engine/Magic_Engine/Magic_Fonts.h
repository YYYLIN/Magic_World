/***********************************************************
ͷ�ļ���XY_Fonts.h
˵�����������ֵĴ�����ʾ��
************************************************************/

#ifndef _XY_FONTS_H_
#define _XY_FONTS_H_

//#include "ft2build.h"
//#include  FT_FREETYPE_H
#include <fstream>
#include <glut.h>
#include <png.h>
using namespace std;


#pragma comment(lib,"libpng16.lib")
//#pragma comment(lib, "freetype26d.lib")

typedef struct FontsTexture
{
	unsigned int    rows;
    unsigned int    width;
	signed   int    left;  //�ַ�������߽߱�ľ���
	signed   int    top;  //��׼�ߵ��ַ���ߵ�ľ���
	signed   int    Ascender;//��׼�ߵ��ַ���������ľ���
	int X, Y;
	
	FontsTexture();
} FontsTexture;

#define  XY_FONTS_CharNumber  99

class Magic_Fonts
{
public:
	Magic_Fonts();
	~Magic_Fonts();

public:
	bool Initialize(const char* ,int,int);//��ʼ��
	bool Initialize(const char* ,int);
	void Shutdown();    //�ر� ��������

	inline FontsTexture& GetChar(const char Char){ return fontstexture[(unsigned char)Char - 32]; }
	inline GLuint GetTextureID(){ return textureID; }
	inline int GetTextureWidth(){ return MapSize; }
	int GetTextAllWidth(const char*);
private:
	bool GetFontsTexture(wstring ,float ,float);
	bool LoadPNG(const char* );

private:
	//��������
 //   FT_Library library;	 // ��ľ��
//    FT_Face face; // face����ľ��
	FontsTexture* fontstexture;
	GLuint   textureID;
	int MapSize;
};

#endif