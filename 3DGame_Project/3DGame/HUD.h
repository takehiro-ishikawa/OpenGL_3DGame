#pragma once
#include "UIScreen.h"
#include <vector>

#define HEALTH_POSITION Vector2(-350.0f, -300.0f)
#define FPS_POSITION Vector2(-470.0f, 320.0f)
#define FPS_FONT_SIZE 24

#pragma region �v���g�^�C�v�錾
class Game;
class Shader;
class Texture;
class TargetComponent;
#pragma endregion

// �e�N�X�`���摜�̃t�@�C���p�X
#define HEALTHBAR_BACK_FILEPATH  "Assets/Textures/UI/HealthBarBack.png"    // �̗̓Q�[�W�w�i
#define HEALTHBAR_FILEPATH       "Assets/Textures/UI/HealthBar.png"        // �̗̓Q�[�W
#define HEALTHBAR_EMPTY_FILEPATH "Assets/Textures/UI/HealthBarEmpty.png"   // �̗̓Q�[�W��������
#define HUD_BACK                 "Assets/Textures/UI/HUD_Back.png"         // HUD�w�i
#define GUIDE_KEYBOARD           "Assets/Textures/UI/Guide_Keyboard.png"   // �L�[�{�[�h����K�C�h
#define GUIDE_CONTROLLER         "Assets/Textures/UI/Guide_Controller.png" // �R���g���[������K�C�h

class HUD : public UIScreen
{
public:
	HUD(Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void SetNormalizeHealth(float value) { mNormalizeHealth = value; }

protected:
	void DrawHealthBar(Shader* shader); // �̗̓Q�[�W��`�悷��
	void DrawFrameRate(Shader* shader); // ���݂�FPS��`�悷��

	float mNormalizeHealth;             // ���K�����ꂽ�̗͂̒l
	Texture* mHealthBarBack;            // �̗̓Q�[�W�̔w�i
	Texture* mHealthBar;                // �̗̓Q�[�W�{��
	Texture* mHealthBarEmpty;           // �̗̓Q�[�W��������

	Texture* mBack;            // �w�i
	Texture* mGuideKeyBoard;   // �L�[�{�[�h����K�C�h
	Texture* mGuideController; // �R���g���[������K�C�h
	Texture* mFrameRate;
};
