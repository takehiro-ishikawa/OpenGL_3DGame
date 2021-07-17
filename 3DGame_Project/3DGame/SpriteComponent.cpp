#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		// �e�N�X�`���̕�/�����ŋ�`���X�P�[�����O���܂�
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// ���ׂẴX�v���C�g�͓����V�F�[�_�[/���_���g�p���邽�߁A
		// �X�v���C�g�������O�ɁA�Q�[���͍ŏ��ɂ������A�N�e�B�u�ɐݒ肵�܂�

		// ���[���h���W��ݒ肷��
		shader->SetMatrixUniform("uWorldTransform", world);
		// ���݂̃e�N�X�`����ݒ肷��
		mTexture->SetActive();
		// ��`��`�悷��
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// ��/������ݒ肷��
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}