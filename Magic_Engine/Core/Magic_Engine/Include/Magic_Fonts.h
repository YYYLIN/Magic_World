/***********************************************************
头文件：XY_Fonts.h
说明：对于文字的处理显示库
************************************************************/

#ifndef _XY_FONTS_H_
#define _XY_FONTS_H_



typedef struct FontsTexture
{
	unsigned int    rows;
    unsigned int    width;
	signed   int    left;  //字符到最左边边界的距离
	signed   int    top;  //基准线到字符最高点的距离
	signed   int    Ascender;//基准线到字符最高轮廓的距离
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
	bool Initialize(const char* ,int,int);//初始化
	bool Initialize(const char* ,int);
	void Shutdown();    //关闭 结束函数

	inline FontsTexture& GetChar(const char Char){ return fontstexture[(unsigned char)Char - 32]; }
	inline 	unsigned int GetTextureID(){ return textureID; }
	inline int GetTextureWidth(){ return MapSize; }
	int GetTextAllWidth(const char*);

private:
	//创建对象
 //   FT_Library library;	 // 库的句柄
//    FT_Face face; // face对象的句柄
	FontsTexture* fontstexture;
	unsigned int textureID;
	int MapSize;
};

#endif