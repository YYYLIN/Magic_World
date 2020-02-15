#ifndef _MAGICSHADER_H_
#define _MAGICSHADER_H_

#include "Define/Magic_Macro.h"
#include <map>
#include <string>
#include <fstream> 

#pragma warning(push)
#pragma warning(disable:4251)

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicShader
{
public:
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

	MagicShader();
	~MagicShader();

	bool LoadFromFile(unsigned int whichShader, const std::string& filename);
	bool LoadFromString(unsigned int whichShader, const std::string& source);
	//创建链接到着色器对象上
	bool CreateAndLinkProgram();
	bool LinkProgranm();
	void Use();
	void UnUse();

	void Enable(ShaderType _ShaderType);
	void Disable(ShaderType _ShaderType);

	void AddAttribute(const std::string& attribute);
	void AddUniform(const std::string& uniform);

	unsigned int operator[](const std::string& attribute);
	unsigned int operator()(const std::string& uniform);

	void DeleteShaderProgram();

	unsigned int GetHandler();

private:

	int totalShaders;
	unsigned int programHandle; //着色器程序对象
	unsigned int Shaders[3];
	std::map<std::string, unsigned int> AttributeList;
	std::map<std::string, unsigned int> UniformLocationList;
};

#pragma warning(pop)

#endif