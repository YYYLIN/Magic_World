#pragma once
#pragma comment(lib,"glew32.lib")

#include <gl/glew.h>
#include "glut.h"

class MagicVertex
{
public:
	MagicVertex();
	virtual ~MagicVertex();

	void CreateVBO(const unsigned char &_number = 1, bool _ARRAYBuffer = false);
	void BindCreate(const unsigned char &_number, unsigned char _size, unsigned int _VertexNumber, void* _data);
	void Delete();

	void* StartUpdataVertex(const unsigned char &_number);
	void EndUpdataVertex();

	void* StartUpdataIndex();
	void EndUpdataIndex();

	GLuint GetVAO() { return vaoHandle; }
	inline unsigned int GetVertexNumber() { return m_VertexNumber; }
	inline unsigned int GetIndexNumber() { return m_IndexNumber; }
private:
	unsigned char m_vboNumber;
	unsigned int m_VertexNumber, m_IndexNumber;
	GLuint*vboHandles;
	GLuint vaoHandle;//vertex array object 
};

