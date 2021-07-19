#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���R���p�C�����܂�
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			mFragShader))
	{
		return false;
	}

	// ���ɁA���̂悤�ȃV�F�[�_�[�v���O�������쐬����B
	// ���_/�t���O�����g�V�F�[�_�[�������N����
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// �v���O����������Ƀ����N����Ă��邱�Ƃ��m�F����
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// �v���O����/�V�F�[�_�[���폜���܂�
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// ���̃v���O�������A�N�e�B�u�ȃv���O�����Ƃ��Đݒ肷��
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// ���̖��O��uniform��T��
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// Matrix�̃f�[�^��uniform�ɑ��M����
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	// ���̖��O��uniform��T��
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// Matrix�̃f�[�^��uniform�ɑ��M����
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	// ���̖��O��uniform��T��
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// Vector�̃f�[�^�𑗐M����
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// Vector�̃f�[�^�𑗐M����
	glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value)
{
	// ���̖��O��uniform��T��
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// Float�̃f�[�^�𑗐M����
	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	// int�̃f�[�^�𑗐M����
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// �t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// ���ׂẴe�L�X�g�𕶎���ɓǂ݂܂�
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �w�肳�ꂽ�^�C�v�̃V�F�[�_�[���쐬���܂�
		outShader = glCreateShader(shaderType);
		// �\�[�X������ݒ肵�ăR���p�C�����Ă݂Ă�������
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// �R���p�C���X�e�[�^�X���Ɖ��
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{

	GLint status;
	// �����N�X�e�[�^�X���Ɖ��
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}
