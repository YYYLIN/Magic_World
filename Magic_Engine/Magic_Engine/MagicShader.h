#pragma once

#include <iostream>
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  

#include <map>
#include <string>
#include <fstream> 

using namespace std;

class MagicShader
{
public:
	MagicShader();
	~MagicShader();

	bool LoadFromFile(GLenum whichShader, const string& filename);
	bool LoadFromString(GLenum whichShader, const string& source);
	//�������ӵ���ɫ��������
	bool CreateAndLinkProgram();
	void Use();
	void UnUse();

	void AddAttribute(const string& attribute);
	void AddUniform(const string& uniform);

	GLuint operator[](const string& attribute);
	GLuint operator()(const string& uniform);

	void DeleteShaderProgram();

	GLuint GetHandler();

private:
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

	int totalShaders;
	GLuint programHandle; //��ɫ���������
	GLuint Shaders[3];
	map<string, GLuint> AttributeList;
	map<string, GLuint> UniformLocationList;
};

