#pragma once
#include "Actor.h"

// プロトタイプ宣言
class Game;
class MeshComponent;
class BoxComponent;

#define PLANE_SIZE 500.0f // 床のサイズ

// 床オブジェクト
class PlaneActor : public Actor
{
public:
	PlaneActor(Game* game, float scale = 1.0f);
	~PlaneActor();
	BoxComponent* GetBox() { return mBox; }

private:
	BoxComponent* mBox;
};

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

// フィールド上に置くコンテナ
class Container : public Actor
{
public:
	Container(Game* game);

private:
	MeshComponent* mMeshComp;
	BoxComponent* mBoxComp;
};