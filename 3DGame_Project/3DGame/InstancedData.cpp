#include "Mesh.h"
#include "Renderer.h"
#include "FBXData.h"
#include "Texture.h"
#include "Shader.h"
#include <GL/glew.h>

InstancedData::InstancedData()
:mMesh(nullptr)
{
	mWorldMatrices.reserve(MAX_INSTANCE);
}

InstancedData::~InstancedData(){}

void InstancedData::Unload()
{
	mWorldMatrices.clear();
}

void InstancedData::AddInstance(const Matrix4& worldMat)
{
	mWorldMatrices.emplace_back(worldMat);
}

void InstancedData::Draw(Shader* shader)
{
	if (mWorldMatrices.size() <= 0) return;

	// アクティブなテクスチャを設定する
	Texture* t = mMesh->GetTexture(0);
	if (t) { t->SetActive(); }

	// メッシュの頂点配列をアクティブとして設定する
	VertexArray* va = mMesh->GetVertexArray();
	va->SetActive();

	int count = mWorldMatrices.size();
	int index = 0;
	while (count > MAX_UNIFORM_ARRAY)
	{
		shader->SetMatrixUniforms("uWorldMat", &mWorldMatrices[index], MAX_UNIFORM_ARRAY);

		glDrawElementsInstanced(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr, MAX_UNIFORM_ARRAY);

		count -= MAX_UNIFORM_ARRAY;
		index += MAX_UNIFORM_ARRAY;
	}

	shader->SetMatrixUniforms("uWorldMat", &mWorldMatrices[index], count);
	glDrawElementsInstanced(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr, count);

	glBindVertexArray(0);
}