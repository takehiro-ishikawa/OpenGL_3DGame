#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "AIComponent.h"


Enemy::Enemy(Game* game)
	:Character(game)
{
	SetScale(120.0f);
	SetPosition(GetPosition());

	// ���b�V���̐���
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Enemy01.fbx"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Enemy01.fbx"));

	// �A�j���[�V�����̃��[�h
	mMeshComp->PlayAnimation(game->GetAnimation("Vigilant", "Assets/Enemy01.fbx"), 1.0f);

	// AABB�̐ݒ�
	AABB box = mMeshComp->GetMesh()->GetBox();
	box.SetMinMax(ENEMY_WIDTH, ENEMY_DEPTH, ENEMY_HEIGHT);
	mBoxComp->SetObjectBox(box);
	mBoxComp->SetShouldRotate(false);

	// ���R���|�[�l���g
	mAIComp = new AIComponent(this);
}