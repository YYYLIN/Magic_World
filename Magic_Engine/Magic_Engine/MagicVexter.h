#pragma once
#pragma comment(lib,"glew32.lib")

#include <gl/glew.h>
#include "glut.h"

class MagicVexter
{
public:
	MagicVexter();
	~MagicVexter();

	void Create(unsigned int _size,unsigned char _vexterNumber,float* _data);
	void Delete();

	float* StartUpdataVexter();
	void EndUpdataVexter();

	GLuint GetVBO() { return vaoHandle; }
private:
	GLuint vboHandles;
	GLuint vaoHandle;//vertex array object 
};

