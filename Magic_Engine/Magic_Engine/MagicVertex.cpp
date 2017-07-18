#include "MagicVertex.h"



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
		glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float) * m_VertexNumber, _data, GL_STATIC_DRAW);

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