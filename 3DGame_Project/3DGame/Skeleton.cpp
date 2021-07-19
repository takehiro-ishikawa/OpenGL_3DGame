#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "MatrixPalette.h"
#include "FBXAnalyze.h"

bool Skeleton::Load(const std::string& fileName)
{
	FBXAnalyze* fbxFile = new FBXAnalyze(fileName.c_str());
	
	// スケルトンのボーン配列を取得
	fbxFile->ComputeBindPoses(mBones);

	// 全てのボーンのグローバルな逆バインドポーズ行列を計算する
	ComputeGlobalInvBindPose();

	// 解放処理
	delete fbxFile;

	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	// ボーンの数で配列の要素数を変更する。これにより、自動的にIDが記入される
	mGlobalInvBindPoses.resize(GetNumBones());

	// 各行列の逆行列を求める
	for (size_t i = 0; i < mGlobalInvBindPoses.size(); i++)
	{
		mGlobalInvBindPoses[i] = mBones[i].mMatrix;
		mGlobalInvBindPoses[i].Invert();
	}
}