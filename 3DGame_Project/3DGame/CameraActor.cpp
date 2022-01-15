#include "CameraActor.h"
#include "CameraComponent.h"

CameraActor::CameraActor(Game* game)
:Actor(game)
{
	SetPosition(Vector3(-10, 0, 7));
	SetRotation(Quaternion(Vector3::UnitY, 0.3f));

	mCameraComp = new PointCamera(this);
}