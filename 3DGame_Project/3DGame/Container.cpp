#include "Field.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "BoxComponent.h"

Container::Container(Game* game)
	:Actor(game)
{
	SetScale(100.0f);

	mMeshComp = new MeshComponent(this, false);
	mBoxComp = new BoxComponent(this);

	mMeshComp->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Models/Container.fbx"));
	mBoxComp->SetObjectBox(mMeshComp->GetMesh()->GetBox());
}