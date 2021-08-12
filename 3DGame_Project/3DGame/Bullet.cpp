#include "Bullet.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BulletMove.h"
#include "AudioComponent.h"
#include "PointLightComponent.h"

Bullet::Bullet(Game* game, Actor* bulletOwner, CharacterTag target, float damage)
	:Actor(game)
	,mDamage(damage)
	,mLifeSpan(BULLET_LIFE)
{	
	mMeshComp = new MeshComponent(this, false);
	mMoveComp = new BulletMove(this, bulletOwner, target, [this](Character* character) {HitTarget(character); });
	mAudioComp = new AudioComponent(this);
	mPointLight = new PointLightComponent(this);
}

void Bullet::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// ê∂ê¨å„ÅAê›íËÇµÇΩéûä‘ï™åoâﬂÇ≈è¡ñ≈
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(Actor::ActorState::EDead);
	}
}

void Bullet::HitTarget(Character* target)
{
	target->Damage(mDamage);
}