#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

// サウンドの呼び出し名
#define SE_OK             "event:/Button_OK"     // OKボタン
#define SE_CANCEL         "event:/Button_Cancel" // キャンセルボタン
#define SE_SELECT         "event:/ButtonSelect"  // ボタン選択
#define SE_SHOOT_P        "event:/PlayerShoot"   // プレイヤーの弾発射音
#define SE_SHOOT_E        "event:/EnemyShoot"    // 敵の弾発射音
#define SE_JUMP           "event:/Jump"          // ジャンプ
#define SE_DASH           "event:/DashStart"     // ダッシュ直後
#define SE_FOOTSTEP_WALK  "event:/FootStep_Walk" // 歩行時の足音
#define SE_FOOTSTEP_DASH  "event:/FootStep_Run"  // ダッシュ時の足音
#define SE_SHOOT_STANDBY  "event:/ShootStandBy"  // 射撃体勢
#define SE_SLASH          "event:/Slash"         // 近接攻撃
#define SE_FOOTSTEP_ENEMY "event:/EnemyFootStep" // 敵の足音
#define SE_ENEMY_DOWN     "event:/EnemyDown"     // 敵の倒れる音
#define SE_ENEMY_DISCOVER "event:/EnemyDiscover" // 敵のプレイヤー発見音

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

	// ビュー行列からリスナーを設定する
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