#include "FBXData.h"
#include <SDL/SDL_log.h>
#include "BoneTransform.h"

using namespace std;

FBXData::FBXData(const char* fileName)
:mNumIndexCount(0)
,mNumVertexCount(0)
,mIsSkeletal(false)
{
	// マネージャを生成
	mManager = FbxManager::Create();

	// IOSettingを生成
	mIoSettings = FbxIOSettings::Create(mManager, IOSROOT);

	// Importerを生成
	mImporter = FbxImporter::Create(mManager, "");
	if (mImporter->Initialize(fileName, -1, mManager->GetIOSettings()) == false) {
		// インポートエラー
		SDL_Log("FBX Failed:Importer");
	}

	// SceneオブジェクトにFBXファイル内の情報を流し込む
	mScene = FbxScene::Create(mManager, "");
	mImporter->Import(mScene);
	//mImporter->Destroy(); // シーンを流し込んだらImporterは解放してOK

	//三角ポリゴン化
	FbxGeometryConverter geometryConverter(mManager);
	geometryConverter.Triangulate(mScene, true);

	// ルートノードを取得
	mRoot_node = mScene->GetRootNode();
	if (mRoot_node == nullptr)
	{
		// 取得失敗
		SDL_Log("FBX Failed:RootNode");
	}

	// メッシュ情報を取得
	mFbxMesh = GetFBXMesh(mRoot_node);
	if (mFbxMesh == nullptr)
	{
		// 取得失敗
		SDL_Log("FBX Failed:FBXMesh");
	}

	// 骨格情報の有無を調べる
	if (mFbxMesh != nullptr && mFbxMesh->GetDeformerCount() > 0)
	{
		mIsSkeletal = true;
	}
}

FBXData::~FBXData()
{
	// マネージャ解放
	mManager->Destroy();
}

FbxMesh* FBXData::GetFBXMesh(fbxsdk::FbxNode* root_node)
{
	FbxMesh* mesh = nullptr;

	for (int i = 0; i < root_node->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* node = root_node->GetChild(i);

		for (int j = 0; j < node->GetNodeAttributeCount(); j++)
		{
			FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(j);

			FbxNodeAttribute::EType type = attribute->GetAttributeType();

			// ノードアトリビュートがメッシュ情報だった場合
			if (type == FbxNodeAttribute::eMesh)
			{
				mesh = node->GetMesh();
			}
		}
	}

	return mesh;
}

void FBXData::FixTextureName(const char* textureName)
{
	int n = 0, endNum = 0;
	string texStr = "";

	// FBXから受け取ったテクスチャ画像のファイルパスがC:から始まるものだったので
	// Assetsフォルダからのパスに修正する
	while (textureName[n] != '\0')
	{
		if (textureName[n] == '\\') texStr = "";
		else texStr += textureName[n];
		n++;
	}

	string FixedName = "Assets/Textures/" + texStr;

    mTextureName = FixedName;
}

std::string FBXData::FixAnimationName(const char* animationName)
{
	int n = 0, endNum = 0;
	string animStr = "";

	// FBXから受け取ったテクスチャ画像のファイルパスがC:から始まるものだったので
	// Assetsフォルダからのパスに修正する
	while (animationName[n] != '\0')
	{
		if (animationName[n] == '|') animStr = "";
		else animStr += animationName[n];
		n++;
	}

	return animStr;
}

void FBXData::ComputeTextureName()
{
	FbxNode* node = mFbxMesh->GetNode();
	if (node == 0) 
	{
		SDL_Log("Failed to get Texture : Node not found");
		return;
	}

	// マテリアルの数
	int materialNum_ = node->GetMaterialCount();
	if (materialNum_ == 0) 
	{
		SDL_Log("Failed to get Texture : No material");
		return;
	}

	// マテリアル情報を取得
	for (int i = 0; i < materialNum_; ++i) 
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		if (material != 0) 
		{
			// ディフューズプロパティを検索
			fbxsdk::FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			// 0番目のテクスチャオブジェクト取得
			fbxsdk::FbxFileTexture* texture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(property.GetSrcObject(FbxCriteria::ObjectType(fbxsdk::FbxFileTexture::ClassId), 0));

			// テクスチャファイルパスを取得して修正
			FixTextureName(texture->GetFileName());
			std::cout << texture->GetFileName() << std::endl;
			return;
		}
	}
}

void FBXData::ComputeIndexList()
{
	auto polygonCount = mFbxMesh->GetPolygonCount();

	mIndices.reserve(polygonCount * 3);

	for (int i = 0; i < polygonCount; ++i)
	{
		mIndices.push_back(mFbxMesh->GetPolygonVertex(i, 0));
		mIndices.push_back(mFbxMesh->GetPolygonVertex(i, 1));
		mIndices.push_back(mFbxMesh->GetPolygonVertex(i, 2));
	}
}

void FBXData::ComputePositionList()
{
	// コントロールポイントがいわゆる位置座標
	mNumVertexCount = mFbxMesh->GetControlPointsCount();

	mVertices.reserve(mNumVertexCount);
	Vector3 tmp = Vector3(0, 0, 0);
	for (int i = 0; i < mNumVertexCount; i++)
	{
		mVertices.push_back(tmp);
	}

	int max = 0;
	for (int i = 0; i < mIndices.size(); i++)
	{
		auto controlPoint = mFbxMesh->GetControlPointAt(mIndices[i]);

		if (controlPoint[3] == 0.0f)
		{
			mVertices[mIndices[i]] = Vector3(controlPoint[0], controlPoint[2], controlPoint[1]);
		}

		if (max <= mIndices[i]) max = mIndices[i];
	}
}

void FBXData::ComputeNormalList()
{
	// 法線セット数を取得
	auto elementCount = mFbxMesh->GetElementNormalCount();
  
    // 法線要素が1のみ対応
	if (elementCount != 1)
	{
		SDL_Log("Failed to get normal : The number of normal elements is not 1");
		return;
	}
  
    auto element = mFbxMesh->GetElementNormal();
    auto mappingMode = element->GetMappingMode();
    auto referenceMode = element->GetReferenceMode();
    const auto& indexArray = element->GetIndexArray();
    const auto& directArray = element->GetDirectArray();
  

    // リファレンスモードはeDirctかeIndexDirectのみ対応
    if((referenceMode != FbxGeometryElement::eDirect) && (referenceMode != FbxGeometryElement::eIndexToDirect))
	{
		SDL_Log("Failed to get normal : Reference mode is not applicable");
		return;
	}
    
	mNormals.reserve(mIndices.size());
	Vector3 tmp = Vector3(0, 0, 0);
	for (int i = 0; i < mIndices.size(); i++)
	{
		mNormals.push_back(tmp);
	}
	
	// メッシュの表面に定義されている「コントロール点」に対して法線が定義されている場合
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// コントロールポイントでマッピング
		for (auto index : mIndices)
		{
			auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)? 
				index : 
				indexArray.GetAt(index);
			auto normal = directArray.GetAt(normalIndex);
			mNormals[mIndices[index]] = Vector3(normal[0], normal[2], normal[1]);
		}
	}
	// ポリゴンの頂点に対して法線が定義されている場合
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// ポリゴンの頂点インデックスでマッピング
		auto indexByPolygonVertex = 0;
		auto polygonCount = mFbxMesh->GetPolygonCount();
		
		for (int i = 0; i < polygonCount; i++)
		{
			auto polygonSize = mFbxMesh->GetPolygonSize(i);

			for (int j = 0; j < polygonSize; j++)
			{
				auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)? 
					indexByPolygonVertex : 
					indexArray.GetAt(indexByPolygonVertex);
				auto normal = directArray.GetAt(normalIndex);

				mNormals[mIndices[indexByPolygonVertex]] = Vector3(normal[0], normal[2], normal[1]);
				
				indexByPolygonVertex++;
			}
		}
	}
	else
	{
		// それ以外のマッピングモードには対応しない
	}
}

void FBXData::ComputeUVList(int uvNo)
{
	auto elementCount = mFbxMesh->GetElementUVCount();
	if (uvNo + 1 > elementCount)
	{
		SDL_Log("Failed to Get UV : UV wasn't one");
		return;
	}

	auto ele = mFbxMesh->GetElementVertexCrease();
	auto element = mFbxMesh->GetElementUV(uvNo);
	auto mappingMode = element->GetMappingMode();
	auto referenceMode = element->GetReferenceMode();
	const auto& indexArray = element->GetIndexArray();
	const auto& directArray = element->GetDirectArray();
	
	// eDirctかeIndexDirectのみ対応
	if ((referenceMode != FbxGeometryElement::eDirect) && (referenceMode != FbxGeometryElement::eIndexToDirect))
	{
		return;
	}

	mUVs.reserve(mNumVertexCount);

	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// コントロールポイントでマッピング
		for (auto index : mIndices)
		{
			auto uvIndex = (referenceMode == FbxGeometryElement::eDirect)
				? index
				: indexArray.GetAt(index);
			auto uv = directArray.GetAt(uvIndex);
			
			mUVs[mIndices[index]] = Vector2(uv[0], uv[1]);
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// UVが独自のインデックスバッファを持っているので複雑な処理になる
		// UV情報の取得と同時に、頂点インデックスの中身をUV独自のインデックス配列とすり替える

		// mVerticesとmVertexBoneInfosのコピーを作る
		mNumVertexCount = directArray.GetCount();
		vector<Vector3> tmpVertices(mVertices.size());
		vector<VertexBoneInfo> tmpBoneInfo(mVertexBoneInfos.size());
		tmpVertices = mVertices;
		mVertices.resize(mNumVertexCount);
		if (mIsSkeletal)
		{
			tmpBoneInfo = mVertexBoneInfos;
			mVertexBoneInfos.resize(mNumVertexCount);
		}

		Vector2 tmp = Vector2(0, 0);
		for (int i = 0; i < mNumVertexCount; i++)
		{
			mUVs.push_back(tmp);
		}

		// mVerticesとmVertexBoneInfosをUVインデックスの内容と合うように書き換える
		mUVs.reserve(mNumVertexCount);
		for (int i = 0; i < indexArray.GetCount(); i++)
		{
			mVertices[indexArray.GetAt(i)] = tmpVertices[mIndices[i]];
			if(mIsSkeletal) mVertexBoneInfos[indexArray.GetAt(i)] = tmpBoneInfo[mIndices[i]];
			
			auto uv = directArray.GetAt(indexArray.GetAt(i));
			
			mUVs[indexArray.GetAt(i)] = Vector2(uv[0], uv[1]);

			mIndices[i] = indexArray.GetAt(i);
		}
		mNumIndexCount = mIndices.size();
	}
	else
	{
		// それ以外のマッピングモードには対応しない
	}
	
	return;
}

void FBXData::ComputeBoneVertexInfo()
{
	mVertexBoneInfos.resize(mNumVertexCount);

	// スキンの数を取得
	int skinCount = mFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (skinCount <= 0)
	{
		SDL_Log("Failed to Get Skeletal Information : No skeletal information");
		return;
	}

	for (int i = 0; i < skinCount; ++i) 
	{
		// i番目のスキンを取得
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		// ボーンの数を取得
		int clusterNum = skin->GetClusterCount();

		if (clusterNum > 256)
		{
			SDL_Log("Failed to Get Skeletal Information : There are more than 256 bones");
			return;
		}

		for (int j = 0; j < clusterNum; ++j) 
		{
			// j番目のクラスタ(ボーン)を取得
			FbxCluster* cluster = skin->GetCluster(j);

			int pointNum = cluster->GetControlPointIndicesCount();
			int* pointAry = cluster->GetControlPointIndices();
			double* weightAry = cluster->GetControlPointWeights();

			for (int k = 0; k < pointNum; k++) 
			{
				// 頂点インデックスとウェイトを取得
				int index = pointAry[k];
				uint8_t weight = (float)weightAry[k] / 1.0f * 255; // 0～255の整数に変換しておく

				mVertexBoneInfos[index].BoneIndex.emplace_back(j);
				mVertexBoneInfos[index].BoneWeight.emplace_back(weight);
			}
		}
	}

	// mVertexBoneInfosの各要素の要素数を４つにする
	// (頂点は必ず４つのボーン情報を参照するので関連するボーン数が４つに満たない箇所を0で埋めておく必要がある)
	for (int i = 0; i < mVertexBoneInfos.size(); i++)
	{
		while (mVertexBoneInfos[i].BoneIndex.size() < 4)
		{
			mVertexBoneInfos[i].BoneIndex.emplace_back(0);
			mVertexBoneInfos[i].BoneWeight.emplace_back(0);
		}
	}
}

void FBXData::ComputeVertexData()
{
	// 頂点インデックスとインデックス数を取得
	ComputeIndexList();
	mNumIndexCount = mIndices.size();

	// 頂点位置座標と頂点数を取得
	ComputePositionList();

	// 骨格情報を持っているならボーンとウェイトの情報を取得
	if (mIsSkeletal)
	{
		ComputeBoneVertexInfo();
	}

	// UV情報を取得(0番目のUVだけ)
	ComputeUVList(0);

	// 法線情報を取得
	ComputeNormalList();

	// テクスチャ名を取得
	ComputeTextureName();
}

bool FBXData::ComputeBindPoses(std::vector<class BoneTransform>& bindPoses)
{
	// スキンの数を取得
	int skinCount = mFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (skinCount <= 0)
	{
		SDL_Log("Failed to Get Skeletal Information : No skeletal information");
		return false;
	}

	for (int i = 0; i < skinCount; ++i)
	{
		// i番目のスキンを取得
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		// ボーンの数を取得
		int clusterNum = skin->GetClusterCount();

		if (clusterNum > 256)
		{
			SDL_Log("Failed to Get Skeletal Information : There are more than 256 bones");
			return false;
		}

		bindPoses.reserve(clusterNum);

		// ボーンの数だけ繰り返す
		for (int j = 0; j < clusterNum; ++j)
		{
			// j番目のクラスタ(ボーン)を取得
			FbxCluster* cluster = skin->GetCluster(j);

			// クラスタのバインドポーズ行列を取得
			FbxAMatrix fbxMat;
			cluster->GetTransformLinkMatrix(fbxMat);
			Matrix4 bindMat = ConvertFBXMatrix(fbxMat);

			// 配列に格納
			BoneTransform bone = ConvertMatToBT(bindMat);
			bindPoses.emplace_back(bone);
		}

		return true;
	}
}

Matrix4& FBXData::ConvertFBXMatrix(const FbxAMatrix& fbxMat)
{
	Matrix4 tempMat;

	tempMat.mat[0][0] = -fbxMat[0][0]; // -
	tempMat.mat[0][1] = fbxMat[0][1]; // 
	tempMat.mat[0][2] = -fbxMat[0][2]; // -
	tempMat.mat[0][3] = -fbxMat[0][3]; // -

	tempMat.mat[1][0] = -fbxMat[1][0];
	tempMat.mat[1][1] = fbxMat[1][1];
	tempMat.mat[1][2] = -fbxMat[1][2];
	tempMat.mat[1][3] = -fbxMat[1][3];

	tempMat.mat[2][0] = -fbxMat[2][0];
	tempMat.mat[2][1] = fbxMat[2][1];
	tempMat.mat[2][2] = -fbxMat[2][2];
	tempMat.mat[2][3] = -fbxMat[2][3];

	tempMat.mat[3][0] = -fbxMat[3][0];
	tempMat.mat[3][1] = fbxMat[3][1];
	tempMat.mat[3][2] = -fbxMat[3][2];
	tempMat.mat[3][3] = -fbxMat[3][3];

	return tempMat;
}

BoneTransform& FBXData::ConvertMatToBT(const Matrix4& mat)
{
	BoneTransform temp;

	// 向き
	Quaternion q = Quaternion::CreateFromMatrix(mat);
	temp.mRotation.x = q.x;
	temp.mRotation.y = q.y;
	temp.mRotation.z = q.z;
	temp.mRotation.w = q.w;

	// 位置
	Vector3 pos = mat.GetTranslation();
	temp.mTranslation.x = pos.x;
	temp.mTranslation.y = pos.y;
	temp.mTranslation.z = pos.z;

	temp.mMatrix = mat;

	return temp;
}

bool FBXData::ComputeAnimation(const std::string& animationName, AnimInfo& animInfo,
	std::vector<std::vector<BoneTransform>>& tracks)
{
	// アニメーション数を取得
	int animStackCount = mImporter->GetAnimStackCount();

	// 1フレームあたりの時間を設定
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	// アニメーションの数だけ繰り返す
	for (int i = 0; i < animStackCount; i++)
	{
		// アニメーションを取得しシーンを変更
		FbxAnimStack* animStack = mScene->GetSrcObject<FbxAnimStack>(i);
		mScene->SetCurrentAnimationStack(animStack);

		// 取得したアニメーションの名前が使用したいアニメーションの名前と同じ場合のみ処理を行う
		string animName = FixAnimationName(animStack->GetName());
		if (animName != animationName) continue;
		std::cout << animName << std::endl;

		// アニメーションのフレーム数を取得
		auto startTime = animStack->GetLocalTimeSpan().GetStart().Get(); //アニメーション開始時間
		auto stopTime = animStack->GetLocalTimeSpan().GetStop().Get();   //アニメーション終了時間
		int frames = (stopTime - startTime) / oneFrameValue;
		animInfo.NumFrames = frames;

		// アニメーションの長さを取得
		animInfo.Duration = (float)(animStack->GetLocalTimeSpan().GetStop().GetSecondDouble() -
			       animStack->GetLocalTimeSpan().GetStart().GetSecondDouble());

		// アニメーションの各フレームの長さを取得
		FbxTime time; time.Set(oneFrameValue);
		animInfo.FrameDuration = (float)time.GetSecondDouble();

		// ボーン数だけ繰り返す
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(0, FbxDeformer::eSkin);
		int clusterNum = skin->GetClusterCount();
		tracks.resize(clusterNum);
		animInfo.NumBones = clusterNum;
		for (int j = 0; j < clusterNum; j++)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			// フレーム数だけ繰り返す
			for (int k = 0; k < frames; k++)
			{
				// kフレーム目の行列を取得
				FbxTime time = k * oneFrameValue;
				FbxAMatrix fbxMat = cluster->GetLink()->EvaluateGlobalTransform(time);
				Matrix4 mat = ConvertFBXMatrix(fbxMat);

				// 配列に格納
				BoneTransform bone = ConvertMatToBT(mat);
				tracks[j].emplace_back(bone);
			}
		}

		std::cout << "成功" << std::endl;
		return true;
	}

	std::cout << "失敗" << std::endl;
	return false;
}