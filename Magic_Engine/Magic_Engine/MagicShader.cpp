#include "MagicShader.h"



MagicShader::MagicShader()
{
	totalShaders = 0;
	Shaders[VERTEX_SHADER] = 0;
	Shaders[FRAGMENT_SHADER] = 0;
	Shaders[GEOMETRY_SHADER] = 0;
	programHandle = 0;
	AttributeList.clear();
	UniformLocationList.clear();
}


MagicShader::~MagicShader()
{
	DeleteShaderProgram();
}

bool MagicShader::LoadFromFile(GLenum whichShader, const string& filename)
{
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp.bad())
	{
		cout << "��ȡ��ɫ������: " << filename << endl;
		return false;
	}
	string line, buffer;
	while (getline(fp, line))
	{
		buffer.append(line);
		buffer.append("\r\n");
	}
	//copy to source
	bool result = LoadFromString(whichShader, buffer);
	if (!result)
		return false;

	fp.close();

	return true;
}


bool MagicShader::LoadFromString(GLenum type, const string& source)
{
	//����
	//1. glCreateShader
	//2. glShaderSource
	//3. glCompileShader
	//4. glGetShaderInfoLog

	//1. ����
	GLuint shader = glCreateShader(type);
	//������  
	if (0 == shader)
	{
		cerr << "���󣺴�����ɫ��ʧ��" << endl;
		exit(1);
	}
	const char * ptmp = source.c_str();
	//2.�����ڴ�
	glShaderSource(shader, 1, &ptmp, NULL);

	//3.������ɫ������  
	glCompileShader(shader);

	//4.�������Ƿ�ɹ�  
	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//�õ�������־����  
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//�õ���־��Ϣ�����  
			glGetShaderInfoLog(shader, logLen, &written, log);
			cerr << "vertex shader compile log : " << endl;
			cerr << log << endl;
			free(log);//�ͷſռ�  
			return false;
		}
	}

	Shaders[totalShaders++] = shader;

	return true;
}


bool MagicShader::CreateAndLinkProgram()
{
	//1.������ɫ������  
	programHandle = glCreateProgram();
	if (!programHandle)
	{
		cerr << "ERROR : create program failed" << endl;
		exit(1);
	}
	if (Shaders[VERTEX_SHADER] != 0) {
		glAttachShader(programHandle, Shaders[VERTEX_SHADER]);
	}
	if (Shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(programHandle, Shaders[FRAGMENT_SHADER]);
	}
	if (Shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader(programHandle, Shaders[GEOMETRY_SHADER]);
	}
	//2.����Щ�������ӳ�һ����ִ�г���  
	glLinkProgram(programHandle);
	//3.��ѯ���ӵĽ��  
	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	if (GL_FALSE == linkStatus)
	{
		cerr << "���� : ������ɫ���������" << endl;
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			cerr << "Program log : " << endl;
			cerr << log << endl;
		}
	}

	glDeleteShader(Shaders[VERTEX_SHADER]);
	glDeleteShader(Shaders[FRAGMENT_SHADER]);
	glDeleteShader(Shaders[GEOMETRY_SHADER]);

	return true;
}

void MagicShader::Use()
{
	glUseProgram(programHandle);
}

void MagicShader::UnUse()
{
	glUseProgram(0);
}

void MagicShader::AddAttribute(const string& attribute)
{
	AttributeList[attribute] = glGetAttribLocation(programHandle, attribute.c_str());
}

//An indexer that returns the location of the attribute
GLuint MagicShader::operator [](const string& attribute)
{
	return AttributeList[attribute];
}

void MagicShader::AddUniform(const string& uniform)
{
	UniformLocationList[uniform] = glGetUniformLocation(programHandle, uniform.c_str());
}

GLuint MagicShader::operator()(const string& uniform)
{
	return UniformLocationList[uniform];
}

void MagicShader::DeleteShaderProgram()
{
	if (programHandle)
		glDeleteProgram(programHandle);
}


GLuint MagicShader::GetHandler()
{
	return programHandle;
}