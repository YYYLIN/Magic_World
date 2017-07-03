#pragma once
#pragma comment(lib,"glew32.lib")

#include <gl/glew.h>
#include "glut.h"

class MagicVertex
{
public:
	MagicVertex();
	~MagicVertex();

	void Create(unsigned int _size,unsigned char _VertexNumber,float* _data);
	void Delete();

	float* StartUpdataVertex();
	void EndUpdataVertex();

	GLuint GetVBO() { return vaoHandle; }
private:
	GLuint vboHandles;
	GLuint vaoHandle;//vertex array object 
};

