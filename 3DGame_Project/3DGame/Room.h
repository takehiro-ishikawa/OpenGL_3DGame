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
	void GeneratePlanes(float posZ);
	void GenerateWall();

	Vector3 mSize;        // 部屋のサイズ
	Vector3 mMinPosition; // 部屋内の最小座標
	Vector3 mMaxPosition; // 部屋内の最大座標
};