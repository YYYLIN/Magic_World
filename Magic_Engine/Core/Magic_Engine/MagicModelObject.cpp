#include "MagicModelObject.h"

#include <fstream>
#include <iostream>
using namespace std;


FILE_OBG_TYPE::FILE_OBG_TYPE()
{
	VertexNumber = 0;
	Position = 0;
	UV = 0;
	Normal = 0;
	IndexNumber = 0;
	Index = 0;
}

FILE_OBG_TYPE::~FILE_OBG_TYPE()
{
	if (Position)
	{
		delete[] Position;
		Position = 0;
	}
	if (UV)
	{
		delete[] UV;
		UV = 0;
	}
	if (Normal)
	{
		delete[] Normal;
		Normal = 0;
	}
	if (Index)
	{
		delete[]Index;
		Index = 0;
	}
	IndexNumber = 0;
	VertexNumber = 0;
}

bool LoodFileOBJ(const char* _filename, FILE_OBG_TYPE* _pFILE_OBG_TYPE)
{
	ifstream file;
	char wChar[256];
	int Vnumber = 0, VTnumber = 0, VNnumber = 0, Fnumber = 0;
	int a, index, V = 0;
	glm::vec3* _pPosition;
	glm::vec2* _pTexture;
	glm::vec3* _pNormal;

	file.open(_filename);
	if (!file)
		return false;

	//获取所有数据的大小
	while (!file.eof())
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 0x00)
		{
			Vnumber++;
		}
	}
	_pPosition = new glm::vec3[Vnumber];
	file.clear();
	file.seekg(0);
	while (!file.eof())
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 't' && wChar[2] == 0x00)
		{
			VTnumber++;
		}
	}
	_pTexture = new glm::vec2[VTnumber];
	file.clear();
	file.seekg(0);
	while (!file.eof())
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 'n' && wChar[2] == 0x00)
		{
			VNnumber++;
		}
	}
	_pNormal = new glm::vec3[VNnumber];
	file.clear();
	file.seekg(0);
	memset(wChar, 0, sizeof(char) * 256);
	while (!file.eof())
	{
		if (wChar[0] == 'f' && wChar[1] == 0x00)
		{
			file >> wChar;
			while (wChar[0] == '0' ||
				wChar[0] == '1' ||
				wChar[0] == '2' ||
				wChar[0] == '3' ||
				wChar[0] == '4' ||
				wChar[0] == '5' ||
				wChar[0] == '6' ||
				wChar[0] == '7' ||
				wChar[0] == '8' ||
				wChar[0] == '9')
			{
				Fnumber++;
				_pFILE_OBG_TYPE->IndexNumber++;
				V++;
				file >> wChar;
			}
			if (V == 4)
			{
				_pFILE_OBG_TYPE->IndexNumber += 2;
				V = 0;
			}
		}
		else
			file >> wChar;
	}
	_pFILE_OBG_TYPE->Position = new glm::vec3[Fnumber];
	_pFILE_OBG_TYPE->UV = new glm::vec2[Fnumber];
	_pFILE_OBG_TYPE->Normal = new glm::vec3[Fnumber];
	file.clear();
	file.seekg(0);


	//读取顶点数据
	a = 0;
	while (a < Vnumber)
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 0x00)
		{
			file >> wChar;
			_pPosition[a].x = (float)atof(wChar);
			file >> wChar;
			_pPosition[a].y = (float)atof(wChar);
			file >> wChar;
			_pPosition[a].z = (float)atof(wChar);
			a++;
		}
	}

	//读取纹理坐标
	file.clear();
	file.seekg(0);
	a = 0;
	while (a < VTnumber)
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 't' && wChar[2] == 0x00)
		{
			file >> wChar;
			_pTexture[a].x = (float)atof(wChar);
			file >> wChar;
			_pTexture[a].y = (float)atof(wChar);
			a++;
		}
	}

	//读取法线向量
	file.clear();
	file.seekg(0);
	a = 0;
	while (a < VNnumber)
	{
		file >> wChar;
		if (wChar[0] == 'v' && wChar[1] == 'n' && wChar[2] == 0x00)
		{
			file >> wChar;
			_pNormal[a].x = (float)atof(wChar);
			file >> wChar;
			_pNormal[a].y = (float)atof(wChar);
			file >> wChar;
			_pNormal[a].z = (float)atof(wChar);
			a++;
		}
	}

	_pFILE_OBG_TYPE->Index = new unsigned short[_pFILE_OBG_TYPE->IndexNumber];
	if (!_pFILE_OBG_TYPE->Index)
		return false;

	//读取面
	file.clear();
	file.seekg(0);
	a = 0;
	V = 0;
	unsigned int _indexCount = 0;
	while (a < Fnumber)
	{
		if (wChar[0] == 'f' && wChar[1] == 0x00)
		{
			file >> wChar;
			/*
						while (wChar[0] == '0' ||
							wChar[0] == '1' ||
							wChar[0] == '2' ||
							wChar[0] == '3' ||
							wChar[0] == '4' ||
							wChar[0] == '5' ||
							wChar[0] == '6' ||
							wChar[0] == '7' ||
							wChar[0] == '8' ||
							wChar[0] == '9')*/
			while (wChar[0] >= '0' &&
				wChar[0] <= '9')
			{
				int _Index1, _Index2, _Index3;
				sscanf_s(wChar, "%d/%d/%d", &_Index1, &_Index2, &_Index3);
				index = _Index1 - 1;
				_pFILE_OBG_TYPE->Position[a].x = _pPosition[index].x;
				_pFILE_OBG_TYPE->Position[a].y = _pPosition[index].y;
				_pFILE_OBG_TYPE->Position[a].z = _pPosition[index].z;
				index = _Index2 - 1;
				_pFILE_OBG_TYPE->UV[a].x = _pTexture[index].x;
				_pFILE_OBG_TYPE->UV[a].y = _pTexture[index].y;
				index = _Index3 - 1;
				_pFILE_OBG_TYPE->Normal[a].x = _pNormal[index].x;
				_pFILE_OBG_TYPE->Normal[a].y = _pNormal[index].y;
				_pFILE_OBG_TYPE->Normal[a].z = _pNormal[index].z;
				V++;
				_pFILE_OBG_TYPE->Index[_indexCount] = a;
				_indexCount++;
				a++;
				file >> wChar;
			}
			if (V == 4)
			{
				_pFILE_OBG_TYPE->Index[_indexCount - 1] = a - 2;
				_pFILE_OBG_TYPE->Index[_indexCount] = a - 1;
				_pFILE_OBG_TYPE->Index[_indexCount + 1] = a - 4;
				_indexCount += 2;
				V = 0;
			}
		}
		else
			file >> wChar;
	}


	file.close();
	if (_pPosition)
	{
		delete[]_pPosition;
		_pPosition = 0;
	}
	if (_pTexture)
	{
		delete[]_pTexture;
		_pTexture = 0;
	}
	if (_pNormal)
	{
		delete[]_pNormal;
		_pNormal = 0;
	}

	_pFILE_OBG_TYPE->VertexNumber = Fnumber;

	return true;
}

MagicModelObject::MagicModelObject()
{
	pModuleObject = 0;
}


MagicModelObject::~MagicModelObject()
{
	Shutdown();
}


bool MagicModelObject::CreateObject(const int &_Number)
{
	m_Number = _Number;
	pModuleObject = new ModuleObject[m_Number];

	return true;
}

void MagicModelObject::Shutdown()
{
	if (pModuleObject)
	{
		delete[]pModuleObject;
		pModuleObject = 0;
	}
}

bool MagicModelObject::LoadObject(const int& _pos, const char* _VertexPath, const char* _TextruePath)
{
	ModuleObject* _pModuleObject = &pModuleObject[_pos];

	//加载OBJ文件
	FILE_OBG_TYPE _FILE_OBG_TYPE;

	bool result = LoodFileOBJ(_VertexPath, &_FILE_OBG_TYPE);
	if (!result)
		return false;


	_pModuleObject->m_Vertex.CreateVBO(3, true);
	_pModuleObject->m_Vertex.BindCreate(0, 3, _FILE_OBG_TYPE.VertexNumber, _FILE_OBG_TYPE.Position);
	_pModuleObject->m_Vertex.BindCreate(1, 2, _FILE_OBG_TYPE.VertexNumber, _FILE_OBG_TYPE.UV);
	_pModuleObject->m_Vertex.BindCreate(2, 3, _FILE_OBG_TYPE.VertexNumber, _FILE_OBG_TYPE.Normal);
	_pModuleObject->m_Vertex.BindCreate(0xFF, sizeof(unsigned short), _FILE_OBG_TYPE.IndexNumber, _FILE_OBG_TYPE.Index);

	result = _pModuleObject->m_Textrue.Initialize(_TextruePath, LOAD_PNG);
	if (!result)
		return false;

	return true;
}

void MagicModelObject::SetWorldMatrix(const glm::mat4& _matrix)
{
	m_WorldMatrix = _matrix;
}