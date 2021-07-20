#include "StateMachine.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

StateMachine::StateMachine()
	:mCurrentState(nullptr)
{
}

void StateMachine::ChangeState(const std::string& name)
{
	// 最初に現在の状態を終了させる
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// 連想配列から新しい状態を見つける
	auto iter = mStateMap.find(name);
	// 見つけた場合
	if (iter != mStateMap.end())
	{
		// その状態に遷移する
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	// 見つからなかった場合
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void StateMachine::RegisterState(State* state)
{
	mStateMap.emplace(state->GetName(), state);
}
