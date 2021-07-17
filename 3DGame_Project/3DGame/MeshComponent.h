#pragma once
#include "Component.h"
#include <cstddef>

#pragma region プロトタイプ宣言
class Actor;
class Shader;
class Mesh;
#pragma endregion

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner, bool isSkeletal);
	~MeshComponent();

	// このメッシュコンポーネントを描画する
	virtual void Draw(Shader* shader);

	// メッシュコンポーネントで使用されるメッシュ/テクスチャインデックスを設定する
	virtual void SetMesh(Mesh* mesh) { mMesh = mesh; }
	virtual Mesh* GetMesh() { return mMesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }

	bool GetIsSkeletal() const { return mIsSkeletal; }

protected:
	Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
	bool mIsSkeletal;
};