#pragma once
#include "UIScreen.h"

// プロトタイプ宣言
class Game;

class DialogBox : public UIScreen
{
public:
	// （低いドローオーダーは、さらに後に対応する）
	DialogBox(Game* game, const std::string& text,
		std::function<void()> onOK);
	~DialogBox();
};
