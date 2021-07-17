#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

#pragma region プロトタイプ宣言
class Actor;
class Shader;
class Skeleton;
class Animation;
#pragma endregion

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(Actor* owner);
	
	// メッシュコンポーネントを描画
	void Draw(Shader* shader) override;

	void Update(float deltaTime) override;

	// セッター
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// アニメーションを再生し。アニメーションの長さを返す
	float PlayAnimation(const Animation* anim, float playRate = 1.0f);

protected:

	// 行列パレットを計算する
	void ComputeMatrixPalette();

	MatrixPalette mPalette;            // 行列パレット
	const Skeleton* mSkeleton;   // 自分が割り当てられるスケルトン
	const Animation* mAnimation; // 今再生しているアニメーション
	float mAnimPlayRate;               // アニメーションの再生レート（1.0が通常のスピード）
	float mAnimTime;                   // アニメーションの現在の時刻
};
