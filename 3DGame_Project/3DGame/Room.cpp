#include "Room.h"
#include "PlaneActor.h"

Room::Room(Game* game, Vector3 position, Vector3 size)
	:Actor(game)
	, mSize(size)
{
	// �����̒��S���W��ݒ�
	SetPosition(position);
	
	int numX = static_cast<int>(size.x / PLANE_SIZE);
	int numY = static_cast<int>(size.y / PLANE_SIZE);
	Vector3 offsetPos = GetPosition();
	Vector2 startPos;
	startPos.x = -mSize.x / 2 + PLANE_SIZE / 2 + offsetPos.x;
	startPos.y = -mSize.y / 2 + PLANE_SIZE / 2 + offsetPos.y;

	Actor* a;
	Quaternion q;
	// ���̐���
	for (int i = 0; i < numX; i++)
	{
		for (int j = 0; j < numY; j++)
		{
			a = new PlaneActor(GetGame());
			Vector3 pos(startPos.x + i * PLANE_SIZE, startPos.y + j * PLANE_SIZE, 0);
			a->SetPosition(pos);
		}
	}

	// �ǂ̐���
	// ���E�̕�
	for (int i = 0; i < numX; i++)
	{
		a = new PlaneActor(GetGame());
		a->SetPosition(Vector3(startPos.x + i * PLANE_SIZE, offsetPos.y + mSize.y / 2, PLANE_SIZE / 2));
		q = Quaternion(Vector3::UnitX, Math::PiOver2);
		a->SetRotation(q);

		a = new PlaneActor(GetGame());
		a->SetPosition(Vector3(startPos.x + i * PLANE_SIZE, offsetPos.y - mSize.y / 2, PLANE_SIZE / 2));
		q = Quaternion(Vector3::UnitX, -Math::PiOver2);
		a->SetRotation(q);
	}
	// �O��̕�
	for (int i = 0; i < numY; i++)
	{
		a = new PlaneActor(GetGame());
		a->SetPosition(Vector3(offsetPos.x + mSize.x / 2, startPos.y + i * PLANE_SIZE, PLANE_SIZE / 2));
		q = Quaternion(Vector3::UnitY, -Math::PiOver2);
		a->SetRotation(q);

		a = new PlaneActor(GetGame());
		a->SetPosition(Vector3(offsetPos.x - mSize.x / 2, startPos.y + i * PLANE_SIZE, PLANE_SIZE / 2));
		q = Quaternion(Vector3::UnitY, Math::PiOver2);
		a->SetRotation(q);
	}

	// �V��̐���

}