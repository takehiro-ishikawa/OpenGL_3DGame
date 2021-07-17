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
	// コンポーネントを削除する必要がある
	// ~ComponentはRemoveComponentを呼び出すため、別のスタイルのループが必要です
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::EActive)
	{
		// ワールド行列を計算
		ComputeWorldTransform();

		// 自身とコンポーネントを更新
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// 再びワールド行列を計算
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
		// コンポーネントの最初のプロセス入力
		for (auto comp : mComponents)
		{
			comp->ProcessInput(state);
		}

		// 自身の入力処理
		ActorInput(state);
	}
}

void Actor::ActorInput(const struct InputState& state)
{
}

void Actor::RotateToNewForward(const Vector3& forward)
{
	// 元の方向（単位X）と新しい方向の差を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// 方向は Xか?
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 方向は -Xか?
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// クロス積で得た軸周りに回転
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

		// まずスケーリング、次に回転、それから平行移動
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// コンポーネントに通知するワールドトランスフォームが更新される
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	// ソートされたベクトルで挿入点を見つける
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

	// イテレータの位置の前に要素を挿入します
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