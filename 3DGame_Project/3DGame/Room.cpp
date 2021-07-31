#include "Room.h"
#include "PlaneActor.h"

Room::Room(Game* game, Vector3 position, Vector3 size)
	:Actor(game)
	, mSize(size)
{
	// �����̒��S���W��ݒ�
	SetPosition(position);
	mMinPosition = Vector3(position.x - mSize.x / 2, position.y - mSize.y / 2, position.z);
	mMaxPosition = Vector3(position.x + mSize.x / 2, position.y + mSize.y / 2, position.z + mSize.z);
	
	// �ŏ��ɏ���~���ʒu
	float startX = mMinPosition.x + PLANE_SIZE / 2;
	float startY = mMinPosition.y + PLANE_SIZE / 2;
	
	// ���̐���
	GeneratePlanes(mMinPosition.z);

	// �ǂ̐���
	GenerateWall();

	// �V��̐����iZ�����̃T�C�Y��0�Ȃ琶�����Ȃ��j
	if (mSize.z > 0)
	{
		GeneratePlanes(mMaxPosition.z);
	}
}

void Room::GeneratePlanes(float posZ)
{
	Actor* a;
	int numX = static_cast<int>(mSize.x / PLANE_SIZE);
	int numY = static_cast<int>(mSize.y / PLANE_SIZE);
	float startX = mMinPosition.x + PLANE_SIZE / 2;
	float startY = mMinPosition.y + PLANE_SIZE / 2;

	for (int i = 0; i < numX; i++)
	{
		for (int j = 0; j < numY; j++)
		{
			a = new PlaneActor(GetGame());
			Vector3 pos(startX + i * PLANE_SIZE, startY + j * PLANE_SIZE, posZ);
			a->SetPosition(pos);
		}
	}
}

void Room::GenerateWall()
{
	Actor* a;
	Quaternion q;
	int numX = static_cast<int>(mSize.x / PLANE_SIZE);
	int numY = static_cast<int>(mSize.y / PLANE_SIZE);
	int numZ = static_cast<int>(mSize.z / PLANE_SIZE);
	float startX = mMinPosition.x + PLANE_SIZE / 2;
	float startY = mMinPosition.y + PLANE_SIZE / 2;
	float startZ = mMinPosition.z + PLANE_SIZE / 2;

	// ���E�̕�
	for (int i = 0; i < numX; i++)
	{
		for (int j = 0; j < numZ; j++)
		{
			a = new PlaneActor(GetGame());
			a->SetPosition(Vector3(startX + i * PLANE_SIZE, mMaxPosition.y, startZ + j * PLANE_SIZE));
			q = Quaternion(Vector3::UnitX, Math::PiOver2);
			a->SetRotation(q);

			a = new PlaneActor(GetGame());
			a->SetPosition(Vector3(startX + i * PLANE_SIZE, mMinPosition.y, startZ + j * PLANE_SIZE));
			q = Quaternion(Vector3::UnitX, -Math::PiOver2);
			a->SetRotation(q);
		}
	}
	// �O��̕�
	for (int i = 0; i < numY; i++)
	{
		for (int j = 0; j < numZ; j++)
		{
			a = new PlaneActor(GetGame());
			a->SetPosition(Vector3(mMaxPosition.x, startY + i * PLANE_SIZE, startZ + j * PLANE_SIZE));
			q = Quaternion(Vector3::UnitY, -Math::PiOver2);
			a->SetRotation(q);

			a = new PlaneActor(GetGame());
			a->SetPosition(Vector3(mMinPosition.x, startY + i * PLANE_SIZE, startZ + j * PLANE_SIZE));
			q = Quaternion(Vector3::UnitY, Math::PiOver2);
			a->SetRotation(q);
		}
	}
}