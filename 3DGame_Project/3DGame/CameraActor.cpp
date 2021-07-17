#include "CameraActor.h"
#include "PointCamera.h"

CameraActor::CameraActor(Game* game)
:Actor(game)
{
	SetPosition(Vector3(0, 0, -200));

	mCameraComp = new PointCamera(this);
	mCameraComp->SetPitch(Math::Pi);
}