#pragma once
#include "Actor.h"

#pragma region プロトタイプ宣言
class Game;
class BoxComponent;
class AudioComponent;
#pragma endregion

class Character : public Actor
{
public:
	Character(Game* game);

protected:
	// 「最小の重なり」で衝突をテストする
	void FixCollisions();

	BoxComponent* mBoxComp;
	AudioComponent* mAudioComp;
};