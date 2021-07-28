#pragma once
#include "Actor.h"

// プロトタイプ宣言
class Game;

// 部屋を作成するクラス
class Room : public Actor
{
public:
	Room(Game* game, Vector3 position, Vector3 size);
	
private:
	
	// 部屋のサイズ
	Vector3 mSize;
};