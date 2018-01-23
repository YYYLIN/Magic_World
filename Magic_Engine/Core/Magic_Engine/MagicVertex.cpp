#include "MagicVertex.h"

#pragma comment(lib,"glew32.lib")

#include <gl/glew.h>
#include "glut.h"

namespace Magic
{
	VERTEX_BUFFER::Buffer_Attribute::Buffer_Attribute()
	{
		Buffer_MaxSize = 0;
		usage = STATIC_DRAW;
	}

	VERTEX_BUFFER::VERTEX_BUFFER()
	{
		m_VAO = 0;
		pVBO = 0;
		pBuffer_Attribute = 0;
		m_Draw_Indirect = 0;
		m_Max_CardSlot = 0;
		m_Index_Pos = -1;
	}

	VERTEX_BUFFER::~VERTEX_BUFFER()
	{
		Shutdown();
	}

	void VERTEX_BUFFER::Shutdown()
	{
		if (pVBO)
		{
			glDeleteBuffers(m_Buffer_Number, pVBO);
			delete pVBO;
			pVBO = 0;
			m_Buffer_Number = 0;
		}
		if (pBuffer_Attribute)
		{
			delete pBuffer_Attribute;
			pBuffer_Attribute = 0;
		}
		if (m_VAO)
		{
			glDeleteVertexArrays(1, &m_VAO);
			m_VAO = 0;
		}
		m_Index_Pos = 0;
		m_Max_CardSlot = 0;
	}

	void VERTEX_BUFFER::CreateBuffer(unsigned char _number)
	{
		m_Buffer_Number = _number;
		pVBO = new GLuint[m_Buffer_Number];
		pBuffer_Attribute = new Buffer_Attribute[m_Buffer_Number];
		glGenBuffers(m_Buffer_Number, pVBO);
		glGenVertexArrays(1, &m_VAO);
	}

	void VERTEX_BUFFER::SetBuffer(unsigned int _BufferPos, DRAW_USAGE _usage, unsigned int _number, unsigned int* _4byteSize, unsigned int _divisor)
	{
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, pVBO[_BufferPos]);
		pBuffer_Attribute[_BufferPos].usage = _usage;
		unsigned int _start_pos = 0;
		unsigned int _Always_Size = 0;
		for (unsigned int a = 0; a < _number; a++)
			_Always_Size += _4byteSize[a];
		for (int a = m_Max_CardSlot, b = m_Max_CardSlot + _number; a < b; a++)
		{
			unsigned int _pos = a - m_Max_CardSlot;
			glEnableVertexAttribArray(a);
			glVertexAttribPointer(a, _4byteSize[_pos], GL_FLOAT, GL_FALSE, _Always_Size * sizeof(float), (GLvoid*)(_start_pos * sizeof(float)));
			//每个对象对应几个实例
			glVertexAttribDivisor(a, _divisor);
			_start_pos += _4byteSize[_pos];
		}

		m_Max_CardSlot += _number;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void VERTEX_BUFFER::SetDrawIndirectBuffer(unsigned int _BufferPos, DRAW_USAGE _usage)
	{
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, pVBO[_BufferPos]);
		pBuffer_Attribute[_BufferPos].usage = _usage;
		m_Draw_Indirect = _BufferPos;
		glBindVertexArray(0);
	}

	void VERTEX_BUFFER::SetIndexBuffer(unsigned char _BufferPos, DRAW_USAGE _usage)
	{
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVBO[_BufferPos]);
		pBuffer_Attribute[_BufferPos].usage = _usage;
		m_Index_Pos = _BufferPos;

		glBindVertexArray(0);
	}

	void VERTEX_BUFFER::DynamicWrite(unsigned int _BufferPos, unsigned int _size, void* _data)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;

		glBindBuffer(_Type, pVBO[_BufferPos]);
		if (pBuffer_Attribute[_BufferPos].Buffer_MaxSize < _size)
		{
			pBuffer_Attribute[_BufferPos].Buffer_MaxSize = _size;
			GLenum _usage = GL_STATIC_DRAW;
			switch (pBuffer_Attribute[_BufferPos].usage)
			{
			case DYNAMIC_DRAW:
				_usage = GL_DYNAMIC_DRAW;
				break;
			case STATIC_DRAW:
				_usage = GL_STATIC_DRAW;
				break;
			case STREAM_DRAW:
				_usage = GL_STREAM_DRAW;
				break;
			}
			glBufferData(_Type, _size, _data, _usage);
		}
		else
			glBufferSubData(_Type, 0, _size, _data);
	}

	void VERTEX_BUFFER::StaticWrite(unsigned int _BufferPos, unsigned int _Offset, unsigned int _size, void* _data)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;

		glBindBuffer(_Type, pVBO[_BufferPos]);
		glBufferSubData(_Type, _Offset, _size, _data);
	}

	void* VERTEX_BUFFER::StartUpdataVertex(unsigned char _BufferPos)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;
		glBindBuffer(_Type, pVBO[_BufferPos]);
		return glMapBuffer(_Type, GL_MAP_WRITE_BIT);
	}

	void VERTEX_BUFFER::EndUpdataVertex(unsigned char _BufferPos)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;
		glBindBuffer(_Type, pVBO[_BufferPos]);
		glUnmapBuffer(_Type);
	}

	void VERTEX_BUFFER::BindBuffer(unsigned char _BufferPos)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;
		glBindBuffer(_Type, pVBO[_BufferPos]);
	}

	void VERTEX_BUFFER::UnBindBuffer(unsigned char _BufferPos)
	{
		GLenum _Type = 0;
		if (m_Index_Pos == _BufferPos)
			_Type = GL_ELEMENT_ARRAY_BUFFER;
		else if (m_Draw_Indirect == _BufferPos)
			_Type = GL_DRAW_INDIRECT_BUFFER;
		else
			_Type = GL_ARRAY_BUFFER;
		glBindBuffer(_Type, 0);
	}

	void VERTEX_BUFFER::Bind()
	{
		glBindVertexArray(m_VAO);
	}

	void VERTEX_BUFFER::UnBind()
	{
		glBindVertexArray(0);
	}
}

MagicVertex::MagicVertex()
{
	m_vboNumber = 0;
	vaoHandle = 0;
	vboHandles = 0;
}


MagicVertex::~MagicVertex()
{
	if (vaoHandle)
		Delete();
}

void MagicVertex::CreateVBO(const unsigned char &_number, bool _ARRAYBuffer)
{
	m_vboNumber = _ARRAYBuffer ? _number + 1 : _number;
	vboHandles = new GLuint[m_vboNumber];
	glGenBuffers(m_vboNumber, vboHandles);
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
}

void MagicVertex::BindCreate(const unsigned char &_number, unsigned char _size, unsigned int _VertexNumber, void* _data)
{
	if (_number == 0xFF)
	{
		m_IndexNumber = _VertexNumber;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[m_vboNumber - 1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexNumber * _size,
			_data, GL_STATIC_DRAW);
	}
	else
	{
		m_VertexNumber = _VertexNumber;
		//GL_ARRAY_BUFFER()（顶点数据）或GL_ELEMENT_ARRAY_BUFFER(索引数据)
		//绑定VBO以供使用  

		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[_number]);
		//加载数据到VBO  
		glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float)* m_VertexNumber, _data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(_number);
		glVertexAttribPointer(_number, _size, GL_FLOAT, GL_FALSE, 0, (GLubyte *)0);
	}
}

void MagicVertex::Delete()
{
	glDeleteBuffers(m_vboNumber, vboHandles);
	delete[] vboHandles;
	glDeleteVertexArrays(1, &vaoHandle);
}

void* MagicVertex::StartUpdataVertex(const unsigned char &_number)
{
	glBindBuffer(GL_ARRAY_BUFFER_ARB, vboHandles[_number]);
	return glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
}

void MagicVertex::EndUpdataVertex()
{
	glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
}

void* MagicVertex::StartUpdataIndex()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, vboHandles[m_vboNumber - 1]);
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
}

void MagicVertex::EndUpdataIndex()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB);
}