#pragma once
#include "Component.h"
#include "SDL/SDL.h"

#pragma region �v���g�^�C�v�錾
class Actor;
class Shader;
class Texture;
#pragma endregion

class SpriteComponent : public Component
{
public:
	// �`�揇���idrawOrder�j���Ⴂ�قǉ����ɒu�����
	SpriteComponent(Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(Shader* shader);
	virtual void SetTexture(Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }

protected:
	// �`�悷��e�N�X�`��
	Texture* mTexture;

	// ��Ƃ̃A���S���Y���Ŏg���`�揇��
	int mDrawOrder;

	// �e�N�X�`���̕��ƍ���
	int mTexWidth;
	int mTexHeight;

	bool mVisible;
};