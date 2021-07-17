#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	// 指定された名前で頂点/フラグメントシェーダーをロードします
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	// これをアクティブなシェーダープログラムとして設定します
	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);             // Matrixのuniformを設定する
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);// Matrixのuniformの配列を設定する	
	void SetVectorUniform(const char* name, const Vector3& vector);             // Vector3のuniformを設定する
	void SetVector2Uniform(const char* name, const Vector2& vector);            // Vector3のuniformを設定する
	void SetFloatUniform(const char* name, float value);                        // Floatのuniformを設定する
	void SetIntUniform(const char* name, int value);                            // Intのuniformを設定する

private:
	// 指定されたシェーダーをコンパイルする
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	// シェーダーが正常にコンパイルされたかどうかをテストする
	bool IsCompiled(GLuint shader);

	// 頂点/フラグメントプログラムがリンクするかどうかをテストする
	bool IsValidProgram();

	// シェーダーオブジェクトIDを保存します
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

