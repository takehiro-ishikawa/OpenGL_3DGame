#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

AIComponent::AIComponent(class Actor* owner)
	:Component(owner)
	, mCurrentState(nullptr)
{
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const std::string& name)
{
	// �ŏ��Ɍ��݂̏�Ԃ��I��������
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// �A�z�z�񂩂�V������Ԃ�������
	auto iter = mStateMap.find(name);
	// �������ꍇ
	if (iter != mStateMap.end())
	{
		// ���̏�ԂɑJ�ڂ���
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	// ������Ȃ������ꍇ
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}
