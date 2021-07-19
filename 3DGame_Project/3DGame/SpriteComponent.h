#pragma once
#include "Component.h"
#include "SDL/SDL.h"

#pragma region プロトタイプ宣言
class Actor;
class Shader;
class Texture;
#pragma endregion

class SpriteComponent : public Component
{
public:
	// 描画順序（drawOrder）が低いほど遠くに置かれる
	SpriteComponent(Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(Shader* shader);
	virtual void SetTexture(Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }

protected:
	// 描画するテクスチャ
	Texture* mTexture;

	// 画家のアルゴリズムで使う描画順序
	int mDrawOrder;

	// テクスチャの幅と高さ
	int mTexWidth;
	int mTexHeight;

	bool mVisible;
};