#include "Field.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "BoxComponent.h"

Container::Container(Game* game, Vector3 position, float scale)
	:Actor(game)
{
	SetPosition(position);
	SetScale(scale);

	ComputeWorldTransform();

	mInstancedMeshComp = new InstancedMeshComponent(this, "Assets/Models/Container.fbx");
	mBoxComp = new BoxComponent(this);
}