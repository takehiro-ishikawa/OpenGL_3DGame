#include "Field.h"
#include "Game.h"
#include "Renderer.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game, float scale)
	:Actor(game)
{
	SetScale(scale);
	MeshComponent* mc = new MeshComponent(this, false);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Models/GrayFloor.fbx");
	mc->SetMesh(mesh);

	// コリジョンボックスを追加
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());
}

PlaneActor::~PlaneActor()
{
}