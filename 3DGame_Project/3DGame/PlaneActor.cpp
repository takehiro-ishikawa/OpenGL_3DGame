#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(25.0f);
	MeshComponent* mc = new MeshComponent(this, false);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Ground.fbx");
	mc->SetMesh(mesh);

	// �R���W�����{�b�N�X��ǉ�
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
	GetGame()->RemovePlane(this);
}