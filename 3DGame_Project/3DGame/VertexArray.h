#pragma once

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

	// ���_�o�b�t�@��OpenGL ID
	unsigned int mVertexBuffer;

	// �C���f�b�N�X�o�b�t�@��OpenGL ID
	unsigned int mIndexBuffer;

	// ���_�z��I�u�W�F�N�g��OpenGL ID
	unsigned int mVertexArray;
};
