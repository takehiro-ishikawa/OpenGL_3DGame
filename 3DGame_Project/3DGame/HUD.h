#pragma once
#include "UIScreen.h"
#include <vector>

#define HEALTH_POSITION Vector2(-350.0f, -300.0f)

#pragma region �v���g�^�C�v�錾
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

// �e�N�X�`���摜�̃t�@�C���p�X
#define HEALTHBAR_BACK_FILEPATH  "Assets/Textures/UI/HealthBarBack.png"  // �̗̓Q�[�W�w�i
#define HEALTHBAR_FILEPATH       "Assets/Textures/UI/HealthBar.png"      // �̗̓Q�[�W
#define HEALTHBAR_EMPTY_FILEPATH "Assets/Textures/UI/HealthBarEmpty.png" // �̗̓Q�[�W��������
#define HUD_BACK                 "Assets/Textures/UI/HUD_Back.png"       // HUD�w�i

class HUD : public UIScreen
{
public:
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void SetNormalizeHealth(float value) { mNormalizeHealth = value; }

	void AddTargetComponent(TargetComponent* tc);
	void RemoveTargetComponent(TargetComponent* tc);

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	// �̗̓Q�[�W�֘A
	void DrawHealthBar(Shader* shader); // �̗̓Q�[�W��`�悷��
	float mNormalizeHealth;             // ���K�����ꂽ�̗͂̒l
	Texture* mHealthBarBack;            // �̗̓Q�[�W�̔w�i
	Texture* mHealthBar;                // �̗̓Q�[�W�{��
	Texture* mHealthBarEmpty;           // �̗̓Q�[�W��������

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
