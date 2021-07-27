#include "CameraActor.h"
#include "PointCamera.h"

CameraActor::CameraActor(Game* game)
:Actor(game)
{
	SetPosition(Vector3(0, 0, 400));

	mCameraComp = new PointCamera(this);
}