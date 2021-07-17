#pragma once
#include <string>
#include "Math.h"

// プロトタイプ宣言
class AudioSystem;

// ゲーム中で鳴らされる１つのサウンド
class SoundEvent
{
public:
	SoundEvent();

	// 対応する FMOD イベントインスタンスが存在したら true を返す
	bool IsValid();

	// イベントをリスタートする
	void Restart();

	// イベントをストップする
	void Stop(bool allowFadeOut = true);

	// セッター/ゲッター
	bool  GetPaused() const;
	void  SetPaused(bool pause);

	float GetVolume() const;
	void  SetVolume(float value);

	float GetPitch() const;
	void  SetPitch(float value);

	float GetParameter(const std::string& name);
	void  SetParameter(const std::string& name, float value);

	// 位置関連
	bool Is3D() const; // イベントが3Dならtrueを返し、そうでなければfalse
	void Set3DAttributes(const Matrix4& worldTrans);

protected:
	// このコンストラクターを保護し、AudioSystemをフレンドにする
	// AudioSystemだけがこのコンストラクターにアクセスできるようにする。
	friend AudioSystem;
	SoundEvent(AudioSystem* system, unsigned int id);

private:
	AudioSystem* mSystem;
	unsigned int mID;
};