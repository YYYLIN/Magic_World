
/*
Source File:MagicGlobalVariable.h
IOFO:Frame Buffer Object
author:YYYLIN
Create time:2017 05 24
*/

#ifndef _MAGICFRAMEBUFFEROBJECT_H_
#define _MAGICFRAMEBUFFEROBJECT_H_
#include "MagicMath.hpp"


class MagicFrameBufferObject
{
public:
	MagicFrameBufferObject();
	~MagicFrameBufferObject();

	unsigned int Initialize(const unsigned short& _width, const unsigned short& _height);
	void Shutdown();

	void Copy(void* _data, const unsigned short& _width, const unsigned short& _height, const unsigned short& _mode, const unsigned short& _type);

	void Clear(const unsigned int& _mode);

	void SetClearColor(const Magic::vector4& _rgba);
	void SetPosColor(const float& _x, const float& _y, const Magic::vector4& _color);
private:
	void CopyRGBA_UCHAR(void* _data, const unsigned short& _width, const unsigned short& _height);
	void CopyBGAR_UCHAR(void* _data, const unsigned short& _width, const unsigned short& _height);
private:
	unsigned short m_Width, m_Height;
	Magic::vector4* pBuffer;
	Magic::vector4 m_ClearColor;
};

#endif