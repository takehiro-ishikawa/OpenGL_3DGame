#pragma once
#include "UIScreen.h"

// �v���g�^�C�v�錾
class Game;

class DialogBox : public UIScreen
{
public:
	// �i�Ⴂ�h���[�I�[�_�[�́A����Ɍ�ɑΉ�����j
	DialogBox(Game* game, const std::string& text,
		std::function<void()> onOK);
	~DialogBox();
};
