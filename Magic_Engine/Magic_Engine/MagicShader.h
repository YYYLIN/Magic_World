#pragma once

#include <iostream>
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  

#include <map>
#include <string>
#include <fstream> 

class MagicShader
{
public:
	MagicShader();
	~MagicShader();

	bool LoadFromFile(GLenum whichShader, const std::string& filename);
	bool LoadFromString(GLenum whichShader, const std::string& source);
	//创建链接到着色器对象上
	bool CreateAndLinkProgram();
	void Use();
	void UnUse();

	void AddAttribute(const std::string& attribute);
	void AddUniform(const std::string& uniform);

	GLuint operator[](const std::string& attribute);
	GLuint operator()(const std::string& uniform);

	void DeleteShaderProgram();

	GLuint GetHandler();

private:
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

	int totalShaders;
	GLuint programHandle; //着色器程序对象
	GLuint Shaders[3];
	std::map<std::string, GLuint> AttributeList;
	std::map<std::string, GLuint> UniformLocationList;
};

