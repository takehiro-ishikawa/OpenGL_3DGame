#pragma once
#include "Actor.h"

// �v���g�^�C�v�錾
class PointCamera;

class CameraActor : public Actor
{
public:
	CameraActor(Game* game);

private:

	PointCamera* mCameraComp;
};