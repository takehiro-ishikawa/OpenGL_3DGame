#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(class Game* game)
	:mGame(game)
	,mSystem(nullptr)
	,mLowLevelSystem(nullptr)
{

}

AudioSystem::~AudioSystem()
{

}

bool AudioSystem::Initialize()
{
	// エラーログの設定
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // エラーだけログを取る
		FMOD_DEBUG_MODE_TTY     // stdoutに出力
	);

	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);

	// resultがFMOD_OKかチェック
	if (result != FMOD_OK)
	{
		//                                              ↓エラーコードを人間が読める形式に変換する
		SDL_Log("FMOD システムの作成に失敗しました:%s", FMOD_ErrorString(result));
		return false;
	}

	// FMODシステムを初期化する
	result = mSystem->initialize(
		512,                       // 最大同時発音数
		FMOD_STUDIO_INIT_NORMAL,   // デフォルトの設定
	    FMOD_INIT_CHANNEL_LOWPASS, // ローパスフィルターを初期化する（サウンドオクルージョンを使用する為）
		nullptr                    // 通常はnullptr
	);

	// resultがFMOD_OKかチェック
	if (result != FMOD_OK)
	{
		//                                              ↓エラーコードを人間が読める形式に変換する
		SDL_Log("Failed to create FMOD system:%s", FMOD_ErrorString(result));
		return false;
	}

	// Low Levelシステムポインタを取得して保存する
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// マスターバンクをロードする（strings が先）
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	// 全てのバンクのアンロード
	UnloadAllBanks();

	// FMODシステムをシャットダウン
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::Update(float deltaTime)
{
	// 停止したイベントインスタンスを検索する
	std::vector<unsigned int> done;

	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;

		// イベントの状態を取得
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// イベントを解放して id を終了リストに追加
			e->release();
			done.emplace_back(iter.first);
		}
	}

	// 終了したイベントインスタンスを連想配列から削除
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	// FMODを更新
	mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name)
{
	// 多重読み込みの防止
	if (mBanks.find(name) != mBanks.end()) return;

	// バンクをロード
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),                 // ファイル名または空白
		FMOD_STUDIO_LOAD_BANK_NORMAL, // 通常の読み込み
		&bank                         // バンクへのポインタを保存
	);

	const int maxPathLength = 512;

	if (result == FMOD_OK)
	{
		// バンクを連想配列に追加
		mBanks.emplace(name, bank);

		// ストリーミング以外のサンプルデータをすべてロード
		bank->loadSampleData();

		// このバンクにあるイベントの数を取得
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			// バンクにあるイベント記述子のリストを取得
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];

				// このイベントのパスを取得して (例：event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);

				// イベント連想配列に追加
				mEvents.emplace(eventName, e);
			}
		}

		// このバンクのバスの数を取得する
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// このバンクのバスのリストを取得する
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];

				// このバスのパスを取得する（bus：/ SFXなど）
				bus->getPath(busName, 512, nullptr);

				// バスマップに追加
				mBuses.emplace(busName, bus);
			}
		}
	}
	else
	{
		SDL_Log("Failed to load FMOD bank");
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	// ロードされていない場合は無視する
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// まず、このバンクからすべてのイベントを削除する
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// このバンクのEventDescriptionを取得する
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// イベントのリストを取得する
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// このイベントのパスを取得する
			e->getPath(eventName, 512, nullptr);
			// このイベントを削除する
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	//// このバンクのバスの数を取得します
	//int numBuses = 0;
	//bank->getBusCount(&numBuses);
	//if (numBuses > 0)
	//{
	//	// このバンクのバスのリストを取得する
	//	std::vector<FMOD::Studio::Bus*> buses(numBuses);
	//	bank->getBusList(buses.data(), numBuses, &numBuses);
	//	char busName[512];
	//	for (int i = 0; i < numBuses; i++)
	//	{
	//		FMOD::Studio::Bus* bus = buses[i];
	//		// このバスのパスを取得する（bus：/ SFXなど）
	//		bus->getPath(busName, 512, nullptr);
	//		// このバスを削除する
	//		auto busi = mBuses.find(busName);
	//		if (busi != mBuses.end())
	//		{
	//			mBuses.erase(busi);
	//		}
	//	}
	//}

	// サンプルデータとバンクをアンロードする
	bank->unloadSampleData();
	bank->unload();

	// バンクの連想配列から削除する
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		// サンプルデータをアンロードしてから、バンク自体をアンロードする
		iter.second->unloadSampleData();
		iter.second->unload();
	}

	mBanks.clear();

	// バンクがないということは、イベントがないことを意味します
	mEvents.clear();
}

 SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	 unsigned int retID = 0;

	// イベントの存在を確認
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		// イベントのインスタンスを作成
		FMOD::Studio::EventInstance* event = nullptr;

		iter->second->createInstance(&event);

		if (event)
		{
			// イベントインスタンスを開始
			event->start();

			// 次のIDを取得し、連想配列に追加
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}

	return SoundEvent(this, retID);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// Convert from our coordinates (+x forward, +y right, +z up)
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	// ベクトルを得るためのビュー行列の逆行列を計算する
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;

	// 位置と方向をセットする
	listener.position = VecToFMOD(invView.GetTranslation());

	// 逆ビューでは、第３行が前方向
	listener.forward = VecToFMOD(invView.GetZAxis());

	// 逆ビューでは、第２行が上方向
	listener.up = VecToFMOD(invView.GetYAxis());

	// 速度はゼロにセットする（ドップラー効果を使う時は修正）
	listener.velocity = { 0.0f, 0.0f, 0.0f };

	// FMODに送る（０は、リスナーが1人だけという意味）
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}