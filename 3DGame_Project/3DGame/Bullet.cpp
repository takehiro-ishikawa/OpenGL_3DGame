#include "Bullet.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BulletMove.h"
#include "AudioComponent.h"
#include "PointLightComponent.h"

Bullet::Bullet(Game* game)
	:Actor(game)
	,mLifeSpan(BULLET_LIFE)
{
	SetScale(1.0f);
	
	MeshComponent* mc = new MeshComponent(this, false);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Bullet.fbx");
	mc->SetMesh(mesh);
	mMoveComp = new BulletMove(this);
	mMoveComp->SetMoveSpeed(Vector2(0, BULLET_SPEED));

	mAudioComp = new AudioComponent(this);
	mPointLight = new PointLightComponent(this);
	mPointLight->mDiffuseColor = Color::Green;
	mPointLight->mOuterRadius = 200.0f;
	mPointLight->mInnerRadius = 100.0f;
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