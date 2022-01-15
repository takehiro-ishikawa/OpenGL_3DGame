#pragma once
#include <vector>
#include <string>
#include "PhysWorld.h"
#include "Component.h"

#pragma region �v���g�^�C�v�錾
class Actor;
class Shader;
class Mesh;
class Renderer;
class Texture;
class FBXData;
#pragma endregion

// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A���_���C�A�E�g���܂Ƃ߂ĊǗ�����
class VertexArray
{
public:
	// �T�|�[�g����Ă��钸�_���C�A�E�g
	enum class Layout
	{
		PosNormTex,    // ���_�ʒu�A���_�@���A�e�N�X�`�����W
		PosNormSkinTex // ���_�ʒu�A���_�@���A�X�L�j���O�̃{�[���A�X�L�j���O�̏d�݁A�e�N�X�`�����W
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts()   const { return mNumVerts; }

private:
	// ���_�o�b�t�@�ɂ��钸�_�̐�
	unsigned int mNumVerts;

	// �C���f�b�N�X�o�b�t�@���̃C���f�b�N�X�̐�
	unsigned int mNumIndices;

	// OpenGL ID
	unsigned int mVertexBuffer;  // ���_�o�b�t�@
	unsigned int mIndexBuffer;   // �C���f�b�N�X�o�b�t�@
	unsigned int mVertexArray;   // ���_�z��I�u�W�F�N�g
};

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

// 1�̃��b�V���f�[�^���Ǘ�
class Mesh
{
public:
	Mesh();
	~Mesh();

	// ���b�V���̃��[�h/�A�����[�h
	virtual bool Load(FBXData* fbxFile, Renderer* renderer);
	virtual void Unload();

	// �Q�b�^�[
	VertexArray* GetVertexArray() { return mVertexArray; }           // ���̃��b�V���Ɋ֘A�t�����Ă��钸�_�z����擾
	Texture* GetTexture(size_t index);                               // �C���f�b�N�X����e�N�X�`�����擾
	const std::string& GetShaderName() const { return mShaderName; } // �V�F�[�_�[�̖��O���擾
	float GetRadius() const { return mRadius; }                      // �I�u�W�F�N�g��Ԃł̋��E���̔��a���擾
	const AABB& GetBox() const { return mBox; }                      // �I�u�W�F�N�g��Ԃ�AABB���擾
	float GetSpecPower() const { return mSpecPower; }                // ���b�V���̋��ʔ��˒l���擾

protected:
	// AABB collision
	AABB mBox;

	// ���b�V���̃e�N�X�`���Q
	std::vector<Texture*> mTextures;

	// ���b�V���̒��_�z��
	class VertexArray* mVertexArray;

	// �V�F�[�_�[�̖��O
	std::string mShaderName;

	// �I�u�W�F�N�g��Ԃł̋��E���̔��a���L�^
	float mRadius;

	// �\�ʂ̋��ʔ��˒l
	float mSpecPower;
};


// ���b�V���̃R���|�[�l���g
class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner, bool isSkeletal);
	~MeshComponent();

	// ���̃��b�V���R���|�[�l���g��`�悷��
	virtual void Draw(Shader* shader);

	// ���b�V���R���|�[�l���g�Ŏg�p����郁�b�V��/�e�N�X�`���C���f�b�N�X��ݒ肷��
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

// �C���X�^���V���O�`��p�̃��b�V���R���|�[�l���g
class InstancedMeshComponent : public Component
{
public:
	InstancedMeshComponent(Actor* owner, const std::string& fileName);
	~InstancedMeshComponent();

private:

};

#define MAX_INSTANCE 10000    // �C���X�^���V���O�`�悷��C���X�^���X�̍ő吔
#define MAX_UNIFORM_ARRAY 100 // �V�F�[�_�[�̃��j�t�H�[���z��̍ő�v�f��

class InstancedData
{
public:
	InstancedData();
	~InstancedData();

	void Unload();

	void SetMesh(Mesh* mesh) { mMesh = mesh; }

	void AddInstance(const Matrix4& worldMat);
	void Draw(Shader* shader);

private:

	Mesh* mMesh;
	std::vector<Matrix4> mWorldMatrices;
};