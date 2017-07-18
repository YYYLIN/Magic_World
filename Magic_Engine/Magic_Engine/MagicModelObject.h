#ifndef _MAGICMODELOBJECT_H_
#define _MAGICMODELOBJECT_H_

#include "glm.hpp"
#include "MagicVertex.h"
#include "MagicTexture.h"


struct FILE_OBG_TYPE
{
	unsigned int VertexNumber;
	glm::vec3* Position;
	glm::vec2* UV;
	glm::vec3* Normal;
	unsigned int IndexNumber;
	unsigned short* Index;

	FILE_OBG_TYPE();
	~FILE_OBG_TYPE();
};

bool LoodFileOBJ(const char* _filename, FILE_OBG_TYPE* _pFILE_OBG_TYPE);

class MagicModelObject
{
	struct ModuleObject
	{
		MagicVertex m_Vertex;
		MagicTexture m_Textrue;
	};
public:
	MagicModelObject();
	virtual ~MagicModelObject();

	bool Initialize(const int &_Number);
	void Shutdown();

	bool LoadObject(const int& _pos, const char* _VertexPath, const char* _TextruePath);

	inline int GetNumber() { return m_Number; }

	inline GLuint GetVAO(int _pos) { return pModuleObject[_pos].m_Vertex.GetVAO(); }
	inline GLuint GetTextrue(int _pos) { return pModuleObject[_pos].m_Textrue.GetTextrue(); }
	inline unsigned int GetIndexNumber(int _pos) { return pModuleObject[_pos].m_Vertex.GetIndexNumber(); }
	inline unsigned int GetVertexNumber(int _pos) { return pModuleObject[_pos].m_Vertex.GetIndexNumber(); }

private:
	int m_Number;
	ModuleObject* pModuleObject;
};

#endif