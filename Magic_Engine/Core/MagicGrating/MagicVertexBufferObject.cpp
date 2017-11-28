#include "MagicVertexBufferObject.h"
#include "MagicMacro.h"
#include "MagicMath.hpp"
#include "malloc.h"

MagicVertexBuffer::MagicVertexBuffer()
{
	pMagicVertexBufferObject = 0;
	m_number = 0;
}

MagicVertexBuffer::~MagicVertexBuffer()
{
	Shutdown();
}

void MagicVertexBuffer::Shutdown()
{
	if (pMagicVertexBufferObject)
	{
		delete[]pMagicVertexBufferObject;
		pMagicVertexBufferObject = 0;
	}
	m_number = 0;
}

void MagicVertexBuffer::BindBuffer(unsigned char _pos, MagicVertexBufferObject* _MagicVertexBufferObject)
{
	if (_pos < m_number)
	{
		pMagicVertexBufferObject[_pos] = _MagicVertexBufferObject;
	}
}

void MagicVertexBuffer::EnableVertexAttribArray(unsigned char _number)
{
	pMagicVertexBufferObject = new PMagicVertexBufferObject[_number];
	m_number = _number;
}

MagicVertexBufferObject::MagicVertexBufferObject()
{
	pBuffer = 0;
	//每个顶点大小等于sizef(m_Type) * m_size
	m_size = 0;
	m_Type = 0;
}


MagicVertexBufferObject::~MagicVertexBufferObject()
{
	Shutdown();
}

void MagicVertexBufferObject::Shutdown()
{
	if (pBuffer)
	{
		free(pBuffer);
		pBuffer = 0;
	}
	m_size = 0;
	m_Type = 0;
	m_TypeSize = 0;
}

void MagicVertexBufferObject::GetVertex(const unsigned int& _pos, void* _pdata)
{
	switch (m_Type)
	{
	case MG_FLOAT:
		switch (m_size)
		{
		case 1:
			*(float*)_pdata = ((float*)pBuffer)[_pos];
			break;
		case 2:
			break;
		case 3:
			*(Magic::vector3*)_pdata = ((Magic::vector3*)pBuffer)[_pos];
			break;
		case 4:
			*(Magic::vector4*)_pdata = ((Magic::vector4*)pBuffer)[_pos];
			break;
		}
		break;
	}
}

void MagicVertexBufferObject::SetBufferData(const void* _pdata, const unsigned int& _size)
{
	pBuffer = malloc(_size);
	memcpy(pBuffer, _pdata, _size);
}

void MagicVertexBufferObject::SetVertexAttribPointer(unsigned char _Size, const unsigned short& _type)
{
	m_size = _Size;
	m_Type = _type;
	switch (m_Type)
	{
	case MG_FLOAT:
		m_TypeSize += 4;
		break;
	}
	m_TypeSize *= m_size;
}