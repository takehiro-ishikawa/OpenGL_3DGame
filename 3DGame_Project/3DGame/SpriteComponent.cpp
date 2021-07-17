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
		// テクスチャの幅/高さで矩形をスケーリングします
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// すべてのスプライトは同じシェーダー/頂点を使用するため、
		// スプライトが引く前に、ゲームは最初にそれらをアクティブに設定します

		// ワールド座標を設定する
		shader->SetMatrixUniform("uWorldTransform", world);
		// 現在のテクスチャを設定する
		mTexture->SetActive();
		// 矩形を描画する
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// 幅/高さを設定する
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}