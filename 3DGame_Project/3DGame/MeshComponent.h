#pragma once
#include "Component.h"
#include <cstddef>

#pragma region �v���g�^�C�v�錾
class Actor;
class Shader;
class Mesh;
#pragma endregion

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