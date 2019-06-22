#ifndef _MAGICMODELOBJECT_H_
#define _MAGICMODELOBJECT_H_

#include "glm.hpp"
#include "Render/MagicVertex.h"
#include "Render/MagicTexture.h"


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

	bool CreateObject(const int &_Number);
	void Shutdown();

	bool LoadObject(const int& _pos, const char* _VertexPath, const char* _TextruePath);

	void SetWorldMatrix(const glm::mat4& _matrix);

	inline int GetNumber() { return m_Number; }

	inline unsigned int GetVAO(int _pos) { return pModuleObject[_pos].m_Vertex.GetVAO(); }
	inline unsigned int GetTextrue(int _pos) { return pModuleObject[_pos].m_Textrue.GetTextrue(); }
	inline unsigned int GetIndexNumber(int _pos) { return pModuleObject[_pos].m_Vertex.GetIndexNumber(); }
	inline unsigned int GetVertexNumber(int _pos) { return pModuleObject[_pos].m_Vertex.GetVertexNumber(); }
	inline const glm::mat4& GetWorldMatrix() { return m_WorldMatrix; }

private:
	int m_Number;
	ModuleObject* pModuleObject;
	glm::mat4 m_WorldMatrix;
};

#endif