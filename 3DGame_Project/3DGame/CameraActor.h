#pragma once
#include "Actor.h"

// プロトタイプ宣言
class PointCamera;

class CameraActor : public Actor
{
public:
	CameraActor(Game* game);

private:

	PointCamera* mCameraComp;
};