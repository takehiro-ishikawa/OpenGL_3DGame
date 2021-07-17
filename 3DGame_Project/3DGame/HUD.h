#pragma once
#include "UIScreen.h"
#include <vector>

#pragma region �v���g�^�C�v�錾
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

class HUD : public UIScreen
{
public:
	//�i�Ⴂdraw order�́A����Ƀo�b�N�ɑΉ����܂��j
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void AddTargetComponent(TargetComponent* tc);
	void RemoveTargetComponent(TargetComponent* tc);

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	Texture* mHealthBar;
	Texture* mRadar;

	Texture* mBlipTex;
	Texture* mRadarArrow;

	Texture* mBack;

	// �Q�[�����̑S�Ă�TargetComponent
	std::vector<TargetComponent*> mTargetComps;

	// ���[�_�[���S����P�_�ւ�2D���΃I�t�Z�b�g
	std::vector<Vector2> mBlips;

	// ���[�_�[�͈̔͂Ɣ��a
	float mRadarRange;
	float mRadarRadius;

	// �\�������G�𑨂��Ă��邩
	bool mTargetEnemy;
};