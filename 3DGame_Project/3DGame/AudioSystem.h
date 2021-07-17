#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

#pragma region プロトタイプ宣言
class Game;
class SoundEvent;
#pragma endregion

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;             // イベント、サンプルデータ、ストリーミングデータが入るコンテナ
		class EventDescription; // イベントの型のようなもの
		class EventInstance;    // 型のインスタンス
		class System;
		class Bus;
	};
};

// 全てのサウンドを管理
class AudioSystem
{
public:
	AudioSystem(Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);

	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void SetListener(const Matrix4& viewMatrix);

	// バスの制御
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

protected:
	// SoundEventだけがこの関数にアクセスできるようにする。
	friend SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	// イベントインスタンスに使用する次のIDを追跡する
	static unsigned int sNextID;

	Game* mGame;

	// FMOD studioシステム
	FMOD::Studio::System* mSystem;

	// FMOD Low-levelシステム（必要時のために）
	FMOD::System* mLowLevelSystem;

	// ロードされたバンクの連想配列
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;

	// バスの連想配列
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;

	// イベント名から EventDescription への連想配列
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;

	// イベントインスタンスへの連想配列
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
};