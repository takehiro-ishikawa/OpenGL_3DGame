#include "Bullet.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BulletMove.h"
#include "AudioComponent.h"
#include "PointLightComponent.h"

Bullet::Bullet(Game* game, CharacterTag target)
	:Actor(game)
	,mLifeSpan(BULLET_LIFE)
{
	SetScale(1.0f);
	
	mMeshComp = new MeshComponent(this, false);
	mMoveComp = new BulletMove(this, target);
	mAudioComp = new AudioComponent(this);
	mPointLight = new PointLightComponent(this);
}

void Bullet::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(Actor::ActorState::EDead);
	}
}

void Bullet::SetPlayer(Actor* player)
{
	mMoveComp->SetPlayer(player);
}

void Bullet::HitTarget()
{

}