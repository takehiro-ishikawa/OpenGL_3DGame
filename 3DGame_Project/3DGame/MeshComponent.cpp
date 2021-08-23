#include "Mesh.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
	, mIsSkeletal(isSkeletal)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ワールド座標を設定する
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());

		// 鏡面反射値を設定する
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// アクティブなテクスチャを設定する
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// メッシュの頂点配列をアクティブとして設定する
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
