
/*
Source File:MagicFrameBufferObject.cpp
IOFO:Frame Buffer Object
author:YYYLIN
Create time:2017 05 24
*/


#include "MagicFrameBufferObject.h"
#include "MagicError.h"
#include <stdlib.h>
#include "MagicMacro.h"



MagicFrameBufferObject::MagicFrameBufferObject()
{
	m_Width = 0;
	m_Height = 0;
	pBuffer = 0;
}


MagicFrameBufferObject::~MagicFrameBufferObject()
{
	Shutdown();
}


unsigned int MagicFrameBufferObject::Initialize(const unsigned short& _width, const unsigned short& _height)
{
	if (_width % 2 != 0 && _height % 2 != 0)
		return Magic_Error_NO_TWOPOWER;

	m_Width = _width;
	m_Height = _height;

	pBuffer = new Magic::vector4[m_Height * m_Width];

	return Magic_NO_Error;
}

void MagicFrameBufferObject::Shutdown()
{
	if (pBuffer)
	{
		delete pBuffer;
		pBuffer = 0;
		m_Width = m_Height = 0;
	}
}

void MagicFrameBufferObject::Copy(void* _data, const unsigned short& _width, const unsigned short& _height, const unsigned short& _mode, const unsigned short& _type)
{
	switch (_mode)
	{
	case MG_RGBA:
		switch (_type)
		{
		case MG_UCHAR:
			CopyRGBA_UCHAR(_data, _width, _height);
			break;
		}
		break;
	case MG_BGRA:
		switch (_type)
		{
		case MG_UCHAR:
			CopyBGAR_UCHAR(_data, _width, _height);
			break;
		}
		break;
	}
}

void MagicFrameBufferObject::Clear(const unsigned int& _mode)
{
	if (_mode & MG_COLOR_BUFFER)
	{
		for (int a = 0, b = m_Width * m_Height;a < b;a++)
			pBuffer[a] = m_ClearColor;
	}
}

void MagicFrameBufferObject::SetClearColor(const Magic::vector4& _rgba)
{
	m_ClearColor = _rgba;
}

void MagicFrameBufferObject::SetPosColor(const float& _x, const float& _y, const Magic::vector4& _color)
{
	pBuffer[(unsigned short)((_x + 1.0f) * (float)(m_Width >> 1)) +
		((unsigned short)((_y + 1.0f) * (float)(m_Height >> 1)) * m_Width)] = _color;
}

void MagicFrameBufferObject::CopyRGBA_UCHAR(void* _data, const unsigned short& _width, const unsigned short& _height)
{
	Magic::tvector4<unsigned char>* _pbuffer = (Magic::tvector4<unsigned char>*)_data;
	float _FrameX = (float)m_Width / (float)_width;
	float _FrameY = (float)m_Height / (float)_height;
	unsigned int _Index = 0;
	for (int _y = _height - 1;_y >= 0;_y--)
	{
		unsigned int _FrameIndexY = (unsigned int)(_FrameY * (_y * m_Width));
		for (int _x = 0;_x < _width;_x++)
		{
			unsigned int _FrameIndex = (unsigned int)(_FrameX * _x) + _FrameIndexY;
			_pbuffer[_Index].r = (unsigned char)(pBuffer[_FrameIndex].r * 255.0f);
			_pbuffer[_Index].g = (unsigned char)(pBuffer[_FrameIndex].g * 255.0f);
			_pbuffer[_Index].b = (unsigned char)(pBuffer[_FrameIndex].b * 255.0f);
			_pbuffer[_Index].a = (unsigned char)(pBuffer[_FrameIndex].a * 255.0f);
			_Index++;
		}
	}
}

void MagicFrameBufferObject::CopyBGAR_UCHAR(void* _data, const unsigned short& _width, const unsigned short& _height)
{
	Magic::tvector4<unsigned char>* _pbuffer = (Magic::tvector4<unsigned char>*)_data;
	float _FrameX = (float)m_Width / (float)_width;
	float _FrameY = (float)m_Height / (float)_height;
	unsigned int _Index = 0;
	for (int _y = _height - 1;_y >= 0;_y--)
	{
		unsigned int _FrameIndexY = (unsigned int)(_FrameY * (_y * m_Width));
		for (int _x = 0;_x < _width;_x++)
		{
			unsigned int _FrameIndex = (unsigned int)(_FrameX * _x) + _FrameIndexY;
			_pbuffer[_Index].b = (unsigned char)(pBuffer[_FrameIndex].r * 255.0f);
			_pbuffer[_Index].g = (unsigned char)(pBuffer[_FrameIndex].g * 255.0f);
			_pbuffer[_Index].r = (unsigned char)(pBuffer[_FrameIndex].b * 255.0f);
			_pbuffer[_Index].a = (unsigned char)(pBuffer[_FrameIndex].a * 255.0f);
			_Index++;
		}
	}
}