#include "FBXData.h"
#include <SDL/SDL_log.h>
#include "BoneTransform.h"

using namespace std;

FBXData::FBXData(const char* fileName)
:mNumIndexCount(0)
,mNumVertexCount(0)
,mIsSkeletal(false)
{
	// �}�l�[�W���𐶐�
	mManager = FbxManager::Create();

	// IOSetting�𐶐�
	mIoSettings = FbxIOSettings::Create(mManager, IOSROOT);

	// Importer�𐶐�
	mImporter = FbxImporter::Create(mManager, "");
	if (mImporter->Initialize(fileName, -1, mManager->GetIOSettings()) == false) {
		// �C���|�[�g�G���[
		SDL_Log("FBX Failed:Importer");
	}

	// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	mScene = FbxScene::Create(mManager, "");
	mImporter->Import(mScene);
	//mImporter->Destroy(); // �V�[���𗬂����񂾂�Importer�͉������OK

	//�O�p�|���S����
	FbxGeometryConverter geometryConverter(mManager);
	geometryConverter.Triangulate(mScene, true);

	// ���[�g�m�[�h���擾
	mRoot_node = mScene->GetRootNode();
	if (mRoot_node == nullptr)
	{
		// �擾���s
		SDL_Log("FBX Failed:RootNode");
	}

	// ���b�V�������擾
	mFbxMesh = GetFBXMesh(mRoot_node);
	if (mFbxMesh == nullptr)
	{
		// �擾���s
		SDL_Log("FBX Failed:FBXMesh");
	}

	// ���i���̗L���𒲂ׂ�
	if (mFbxMesh != nullptr && mFbxMesh->GetDeformerCount() > 0)
	{
		mIsSkeletal = true;
	}
}

FBXData::~FBXData()
{
	// �}�l�[�W�����
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

			// �m�[�h�A�g���r���[�g�����b�V����񂾂����ꍇ
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

	// FBX����󂯎�����e�N�X�`���摜�̃t�@�C���p�X��C:����n�܂���̂������̂�
	// Assets�t�H���_����̃p�X�ɏC������
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

	// FBX����󂯎�����e�N�X�`���摜�̃t�@�C���p�X��C:����n�܂���̂������̂�
	// Assets�t�H���_����̃p�X�ɏC������
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

	// �}�e���A���̐�
	int materialNum_ = node->GetMaterialCount();
	if (materialNum_ == 0) 
	{
		SDL_Log("Failed to get Texture : No material");
		return;
	}

	// �}�e���A�������擾
	for (int i = 0; i < materialNum_; ++i) 
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		if (material != 0) 
		{
			// �f�B�t���[�Y�v���p�e�B������
			fbxsdk::FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			// 0�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾
			fbxsdk::FbxFileTexture* texture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(property.GetSrcObject(FbxCriteria::ObjectType(fbxsdk::FbxFileTexture::ClassId), 0));

			// �e�N�X�`���t�@�C���p�X���擾���ďC��
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
	// �R���g���[���|�C���g��������ʒu���W
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
	// �@���Z�b�g�����擾
	auto elementCount = mFbxMesh->GetElementNormalCount();
  
    // �@���v�f��1�̂ݑΉ�
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
  

    // ���t�@�����X���[�h��eDirct��eIndexDirect�̂ݑΉ�
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
	
	// ���b�V���̕\�ʂɒ�`����Ă���u�R���g���[���_�v�ɑ΂��Ė@������`����Ă���ꍇ
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// �R���g���[���|�C���g�Ń}�b�s���O
		for (auto index : mIndices)
		{
			auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)? 
				index : 
				indexArray.GetAt(index);
			auto normal = directArray.GetAt(normalIndex);
			mNormals[mIndices[index]] = Vector3(normal[0], normal[2], normal[1]);
		}
	}
	// �|���S���̒��_�ɑ΂��Ė@������`����Ă���ꍇ
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// �|���S���̒��_�C���f�b�N�X�Ń}�b�s���O
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
		// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
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
	
	// eDirct��eIndexDirect�̂ݑΉ�
	if ((referenceMode != FbxGeometryElement::eDirect) && (referenceMode != FbxGeometryElement::eIndexToDirect))
	{
		return;
	}

	mUVs.reserve(mNumVertexCount);

	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// �R���g���[���|�C���g�Ń}�b�s���O
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
		// UV���Ǝ��̃C���f�b�N�X�o�b�t�@�������Ă���̂ŕ��G�ȏ����ɂȂ�
		// UV���̎擾�Ɠ����ɁA���_�C���f�b�N�X�̒��g��UV�Ǝ��̃C���f�b�N�X�z��Ƃ���ւ���

		// mVertices��mVertexBoneInfos�̃R�s�[�����
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

		// mVertices��mVertexBoneInfos��UV�C���f�b�N�X�̓��e�ƍ����悤�ɏ���������
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
		// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
	}
	
	return;
}

void FBXData::ComputeBoneVertexInfo()
{
	mVertexBoneInfos.resize(mNumVertexCount);

	// �X�L���̐����擾
	int skinCount = mFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (skinCount <= 0)
	{
		SDL_Log("Failed to Get Skeletal Information : No skeletal information");
		return;
	}

	for (int i = 0; i < skinCount; ++i) 
	{
		// i�Ԗڂ̃X�L�����擾
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		// �{�[���̐����擾
		int clusterNum = skin->GetClusterCount();

		if (clusterNum > 256)
		{
			SDL_Log("Failed to Get Skeletal Information : There are more than 256 bones");
			return;
		}

		for (int j = 0; j < clusterNum; ++j) 
		{
			// j�Ԗڂ̃N���X�^(�{�[��)���擾
			FbxCluster* cluster = skin->GetCluster(j);

			int pointNum = cluster->GetControlPointIndicesCount();
			int* pointAry = cluster->GetControlPointIndices();
			double* weightAry = cluster->GetControlPointWeights();

			for (int k = 0; k < pointNum; k++) 
			{
				// ���_�C���f�b�N�X�ƃE�F�C�g���擾
				int index = pointAry[k];
				uint8_t weight = (float)weightAry[k] / 1.0f * 255; // 0�`255�̐����ɕϊ����Ă���

				mVertexBoneInfos[index].BoneIndex.emplace_back(j);
				mVertexBoneInfos[index].BoneWeight.emplace_back(weight);
			}
		}
	}

	// mVertexBoneInfos�̊e�v�f�̗v�f�����S�ɂ���
	// (���_�͕K���S�̃{�[�������Q�Ƃ���̂Ŋ֘A����{�[�������S�ɖ����Ȃ��ӏ���0�Ŗ��߂Ă����K�v������)
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
	// ���_�C���f�b�N�X�ƃC���f�b�N�X�����擾
	ComputeIndexList();
	mNumIndexCount = mIndices.size();

	// ���_�ʒu���W�ƒ��_�����擾
	ComputePositionList();

	// ���i���������Ă���Ȃ�{�[���ƃE�F�C�g�̏����擾
	if (mIsSkeletal)
	{
		ComputeBoneVertexInfo();
	}

	// UV�����擾(0�Ԗڂ�UV����)
	ComputeUVList(0);

	// �@�������擾
	ComputeNormalList();

	// �e�N�X�`�������擾
	ComputeTextureName();
}

bool FBXData::ComputeBindPoses(std::vector<class BoneTransform>& bindPoses)
{
	// �X�L���̐����擾
	int skinCount = mFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (skinCount <= 0)
	{
		SDL_Log("Failed to Get Skeletal Information : No skeletal information");
		return false;
	}

	for (int i = 0; i < skinCount; ++i)
	{
		// i�Ԗڂ̃X�L�����擾
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		// �{�[���̐����擾
		int clusterNum = skin->GetClusterCount();

		if (clusterNum > 256)
		{
			SDL_Log("Failed to Get Skeletal Information : There are more than 256 bones");
			return false;
		}

		bindPoses.reserve(clusterNum);

		// �{�[���̐������J��Ԃ�
		for (int j = 0; j < clusterNum; ++j)
		{
			// j�Ԗڂ̃N���X�^(�{�[��)���擾
			FbxCluster* cluster = skin->GetCluster(j);

			// �N���X�^�̃o�C���h�|�[�Y�s����擾
			FbxAMatrix fbxMat;
			cluster->GetTransformLinkMatrix(fbxMat);
			Matrix4 bindMat = ConvertFBXMatrix(fbxMat);

			// �z��Ɋi�[
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

	// ����
	Quaternion q = Quaternion::CreateFromMatrix(mat);
	temp.mRotation.x = q.x;
	temp.mRotation.y = q.y;
	temp.mRotation.z = q.z;
	temp.mRotation.w = q.w;

	// �ʒu
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
	// �A�j���[�V���������擾
	int animStackCount = mImporter->GetAnimStackCount();

	// 1�t���[��������̎��Ԃ�ݒ�
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	// �A�j���[�V�����̐������J��Ԃ�
	for (int i = 0; i < animStackCount; i++)
	{
		// �A�j���[�V�������擾���V�[����ύX
		FbxAnimStack* animStack = mScene->GetSrcObject<FbxAnimStack>(i);
		mScene->SetCurrentAnimationStack(animStack);

		// �擾�����A�j���[�V�����̖��O���g�p�������A�j���[�V�����̖��O�Ɠ����ꍇ�̂ݏ������s��
		string animName = FixAnimationName(animStack->GetName());
		if (animName != animationName) continue;
		std::cout << animName << std::endl;

		// �A�j���[�V�����̃t���[�������擾
		auto startTime = animStack->GetLocalTimeSpan().GetStart().Get(); //�A�j���[�V�����J�n����
		auto stopTime = animStack->GetLocalTimeSpan().GetStop().Get();   //�A�j���[�V�����I������
		int frames = (stopTime - startTime) / oneFrameValue;
		animInfo.NumFrames = frames;

		// �A�j���[�V�����̒������擾
		animInfo.Duration = (float)(animStack->GetLocalTimeSpan().GetStop().GetSecondDouble() -
			       animStack->GetLocalTimeSpan().GetStart().GetSecondDouble());

		// �A�j���[�V�����̊e�t���[���̒������擾
		FbxTime time; time.Set(oneFrameValue);
		animInfo.FrameDuration = (float)time.GetSecondDouble();

		// �{�[���������J��Ԃ�
		FbxSkin* skin = (FbxSkin*)mFbxMesh->GetDeformer(0, FbxDeformer::eSkin);
		int clusterNum = skin->GetClusterCount();
		tracks.resize(clusterNum);
		animInfo.NumBones = clusterNum;
		for (int j = 0; j < clusterNum; j++)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			// �t���[���������J��Ԃ�
			for (int k = 0; k < frames; k++)
			{
				// k�t���[���ڂ̍s����擾
				FbxTime time = k * oneFrameValue;
				FbxAMatrix fbxMat = cluster->GetLink()->EvaluateGlobalTransform(time);
				Matrix4 mat = ConvertFBXMatrix(fbxMat);

				// �z��Ɋi�[
				BoneTransform bone = ConvertMatToBT(mat);
				tracks[j].emplace_back(bone);
			}
		}

		std::cout << "����" << std::endl;
		return true;
	}

	std::cout << "���s" << std::endl;
	return false;
}