#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

InstancedMeshComponent::InstancedMeshComponent(Actor* owner, const std::string& fileName)
	:Component(owner)
{
	InstancedData* data = mOwner->GetGame()->GetRenderer()->GetInstancedData(fileName);
	data->AddInstance(mOwner->GetWorldTransform());
}

InstancedMeshComponent::~InstancedMeshComponent()
{

}