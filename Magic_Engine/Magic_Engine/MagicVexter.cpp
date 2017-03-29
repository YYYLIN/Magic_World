#include "MagicVexter.h"



MagicVexter::MagicVexter()
{
	vaoHandle = 0;
	vboHandles = 0;
}


MagicVexter::~MagicVexter()
{
	if (vaoHandle)
		Delete();
}


void MagicVexter::Create(unsigned int _size, unsigned char _vexterNumber, float* _data)
{
	// Create and populate the buffer objects  
	glGenBuffers(1, &vboHandles);

	//GL_ARRAY_BUFFER()（顶点数据）或GL_ELEMENT_ARRAY_BUFFER(索引数据)
	//绑定VBO以供使用  
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles);
	//加载数据到VBO  
	glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float) * _vexterNumber, _data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles);
	glVertexAttribPointer(0, _vexterNumber, GL_FLOAT, GL_FALSE, 0, (GLubyte *)0);

	glBindVertexArray(0);
}

void MagicVexter::Delete()
{
	glDeleteBuffers(1, &vboHandles);
	glDeleteVertexArrays(1, &vaoHandle);
}

float* MagicVexter::StartUpdataVexter()
{
	glBindBuffer(GL_ARRAY_BUFFER_ARB, vboHandles);
	return (float*)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
}

void MagicVexter::EndUpdataVexter()
{
	glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
}