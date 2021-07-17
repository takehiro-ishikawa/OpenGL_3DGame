#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	// �w�肳�ꂽ���O�Œ��_/�t���O�����g�V�F�[�_�[�����[�h���܂�
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	// ������A�N�e�B�u�ȃV�F�[�_�[�v���O�����Ƃ��Đݒ肵�܂�
	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);             // Matrix��uniform��ݒ肷��
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);// Matrix��uniform�̔z���ݒ肷��	
	void SetVectorUniform(const char* name, const Vector3& vector);             // Vector3��uniform��ݒ肷��
	void SetVector2Uniform(const char* name, const Vector2& vector);            // Vector3��uniform��ݒ肷��
	void SetFloatUniform(const char* name, float value);                        // Float��uniform��ݒ肷��
	void SetIntUniform(const char* name, int value);                            // Int��uniform��ݒ肷��

private:
	// �w�肳�ꂽ�V�F�[�_�[���R���p�C������
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	// �V�F�[�_�[������ɃR���p�C�����ꂽ���ǂ������e�X�g����
	bool IsCompiled(GLuint shader);

	// ���_/�t���O�����g�v���O�����������N���邩�ǂ������e�X�g����
	bool IsValidProgram();

	// �V�F�[�_�[�I�u�W�F�N�gID��ۑ����܂�
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

