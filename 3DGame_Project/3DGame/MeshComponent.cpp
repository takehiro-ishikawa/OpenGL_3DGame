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
		// ���[���h���W��ݒ肷��
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());

		// ���ʔ��˒l��ݒ肷��
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// �A�N�e�B�u�ȃe�N�X�`����ݒ肷��
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// ���b�V���̒��_�z����A�N�e�B�u�Ƃ��Đݒ肷��
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// �`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
