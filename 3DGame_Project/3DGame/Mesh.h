#pragma once
#include <vector>
#include <string>
#include "Collision.h"

#pragma region �v���g�^�C�v�錾
class Renderer;
class VertexArray;
class Texture;
#pragma endregion


class Mesh
{
public:
	Mesh();
	~Mesh();

	// ���b�V���̃��[�h/�A�����[�h
	virtual bool Load(const std::string& fileName, Renderer* renderer);
	virtual void Unload();

	// �Q�b�^�[
	VertexArray* GetVertexArray() { return mVertexArray; }           // ���̃��b�V���Ɋ֘A�t�����Ă��钸�_�z����擾
	Texture* GetTexture(size_t index);                               // �C���f�b�N�X����e�N�X�`�����擾
	const std::string& GetShaderName() const { return mShaderName; } // �V�F�[�_�[�̖��O���擾
	float GetRadius() const { return mRadius; }                      // �I�u�W�F�N�g��Ԃł̋��E���̔��a���擾
	const AABB& GetBox() const { return mBox; }                      // �I�u�W�F�N�g��Ԃ�AABB���擾
	float GetSpecPower() const { return mSpecPower; }                // ���b�V���̋��ʔ��˒l���擾

private:

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