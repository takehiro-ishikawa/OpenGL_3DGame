#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mState(ActorState::EActive)
	, mPosition(Vector3::Zero)
	, mRotation(Quaternion::Identity)
	, mScale(1.0f)
	, mGame(game)
	, mRecomputeWorldTransform(true)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	// �R���|�[�l���g���폜����K�v������
	// ~Component��RemoveComponent���Ăяo�����߁A�ʂ̃X�^�C���̃��[�v���K�v�ł�
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::EActive)
	{
		// ���[���h�s����v�Z
		ComputeWorldTransform();

		// ���g�ƃR���|�[�l���g���X�V
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// �Ăу��[���h�s����v�Z
		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const struct InputState& state)
{
	if (mState == ActorState::EActive)
	{
		// �R���|�[�l���g�̍ŏ��̃v���Z�X����
		for (auto comp : mComponents)
		{
			comp->ProcessInput(state);
		}

		// ���g�̓��͏���
		ActorInput(state);
	}
}

void Actor::ActorInput(const struct InputState& state)
{
}

void Actor::RotateToNewForward(const Vector3& forward)
{
	// ���̕����i�P��X�j�ƐV���������̍������߂�
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// ������ X��?
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������ -X��?
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// �N���X�ςœ���������ɉ�]
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		// �܂��X�P�[�����O�A���ɉ�]�A���ꂩ�畽�s�ړ�
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// �R���|�[�l���g�ɒʒm���郏�[���h�g�����X�t�H�[�����X�V�����
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	// �\�[�g���ꂽ�x�N�g���ő}���_��������
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}�����܂�
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}