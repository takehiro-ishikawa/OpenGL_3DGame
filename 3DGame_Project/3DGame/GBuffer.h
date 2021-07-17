#pragma once
#include "vector"

// �v���g�^�C�v�錾
class Texture;

class GBuffer
{
public:

	// G�o�b�t�@�Ɋi�[�����f�[�^�̎��
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	GBuffer();
	~GBuffer();

	// G�o�b�t�@�̐����Ɣj��
	bool Create(int width, int height);
	void Destroy();

	// �w�肷��^�C�v�̃e�N�X�`�����擾
	Texture* GetTexture(Type type);

	// �t���[���o�b�t�@�I�u�W�F�N�g�� ID ���擾
	unsigned int GetBufferID() { return mBufferID; }

	// G�o�b�t�@�̊e�e�N�X�`����Ή�����e�N�X�`���C���f�b�N�X�Ƀo�C���h����
	void SetTexturesActive();
	
private:

	// G�o�b�t�@�Ɋ��蓖�Ă�ꂽ�e�N�X�`��
	std::vector<Texture*> mTextures;

	// �t���[���o�b�t�@�I�u�W�F�N�g��ID
	unsigned int mBufferID;
};