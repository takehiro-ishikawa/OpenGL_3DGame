#pragma once
#include <fbxsdk.h>
#include <iostream>
#include "SkeletalAnimation.h"

// �v���g�^�C�v�錾
class BoneTransform;

// ���_�ɉe����^����{�[���̏��
typedef struct
{
	std::vector<uint8_t> BoneIndex;  // �{�[���̃C���f�b�N�X�ԍ�(�{�[������1�`255�ȓ�)
	std::vector<uint8_t> BoneWeight; // �{�[���̏d��(1�`255�ȓ�)
}VertexBoneInfo;

// FBXSDK��p����FBX�t�@�C���̃f�[�^���Ǘ�����
class FBXData
{
public:
	FBXData(const char* filename);
	~FBXData();

	void ComputeVertexData();                                     // ���b�V���̒��_�����v�Z����
	bool ComputeBindPoses(std::vector<BoneTransform>& bindPoses); // �X�P���g���̃{�[���z����v�Z���ĕԂ�

	// �A�j���[�V�������擾����
	bool ComputeAnimation(const std::string& animationName, AnimInfo& animInfo,
		std::vector<std::vector<BoneTransform>>& tracks);

	// �Q�b�^�[
	int GetNumIndexCount() { return mNumIndexCount; }
	int GetNumVertexCount() { return mNumVertexCount; }
	std::vector<unsigned int> GetIndices() { return mIndices; }
	std::vector<Vector3>& GetVertices() { return mVertices; }
	std::vector<Vector3>& GetNormals() { return mNormals; }
	std::vector<Vector2>& GetUVs() { return mUVs; }
	std::vector<VertexBoneInfo>& GetVertexBoneInfos() { return mVertexBoneInfos; }
	bool GetIsSkeletal() { return mIsSkeletal; }
	std::string GetTextureName() { return mTextureName; }

private:

	// FBX�̃��b�V�������擾
	FbxMesh* GetFBXMesh(FbxNode* root_node);

	// ���b�V���̃e�N�X�`�����擾����
	void ComputeTextureName();

	// ���_�C���f�b�N�X�̎擾
	void ComputeIndexList();

	// ���_�ʒu���W�̎擾
	void ComputePositionList();

	// �@�������擾
	void ComputeNormalList();

	// UV�����擾
	void ComputeUVList(int uvNo);

	// �e�N�X�`���̖��O�𑊑΃p�X�ɏC������
	void FixTextureName(const char* textureName);

	std::string FixAnimationName(const char* animationName);

	// �{�[�����e����^���钸�_�̏����擾����
	void ComputeBoneVertexInfo();

	// ���b�p�[�֐�
	// FBXAMatrix��Matrix4�ɕϊ�����
	Matrix4& ConvertFBXMatrix(const FbxAMatrix& fbxMat);
	// Matrix4��BoneTransform�ɕϊ�����
	BoneTransform& ConvertMatToBT(const Matrix4& mat);

	// FBXSDK�֘A
	FbxManager* mManager;
	FbxImporter* mImporter;
	FbxScene* mScene;

	FbxNode* mRoot_node; // ���[�g�m�[�h
	FbxMesh* mFbxMesh;   // ���b�V�����

	// ���b�V�����
	std::vector<unsigned int> mIndices;           // ���_�C���f�b�N�X
	int	mNumIndexCount;                           // ���_�C���f�b�N�X���̃C���f�b�N�X�̐�
    std::vector<Vector3> mVertices;               // ���_�ʒu���W�f�[�^
	int mNumVertexCount;                          // ���_��
	std::vector<Vector3> mNormals;                // �@�����
	std::vector<Vector2> mUVs;                    // UV���
	std::string mTextureName;                     // �e�N�X�`����

	// �X�P���g�����
	bool mIsSkeletal;                             // ���i���������Ă��邩�H
	std::vector<VertexBoneInfo> mVertexBoneInfos; // ���_�ɉe����^����{�[���̏��
};