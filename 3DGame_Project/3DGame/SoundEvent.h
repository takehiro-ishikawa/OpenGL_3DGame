#pragma once
#include <string>
#include "Math.h"

// �v���g�^�C�v�錾
class AudioSystem;

// �Q�[�����Ŗ炳���P�̃T�E���h
class SoundEvent
{
public:
	SoundEvent();

	// �Ή����� FMOD �C�x���g�C���X�^���X�����݂����� true ��Ԃ�
	bool IsValid();

	// �C�x���g�����X�^�[�g����
	void Restart();

	// �C�x���g���X�g�b�v����
	void Stop(bool allowFadeOut = true);

	// �Z�b�^�[/�Q�b�^�[
	bool  GetPaused() const;
	void  SetPaused(bool pause);

	float GetVolume() const;
	void  SetVolume(float value);

	float GetPitch() const;
	void  SetPitch(float value);

	float GetParameter(const std::string& name);
	void  SetParameter(const std::string& name, float value);

	// �ʒu�֘A
	bool Is3D() const; // �C�x���g��3D�Ȃ�true��Ԃ��A�����łȂ����false
	void Set3DAttributes(const Matrix4& worldTrans);

protected:
	// ���̃R���X�g���N�^�[��ی삵�AAudioSystem���t�����h�ɂ���
	// AudioSystem���������̃R���X�g���N�^�[�ɃA�N�Z�X�ł���悤�ɂ���B
	friend AudioSystem;
	SoundEvent(AudioSystem* system, unsigned int id);

private:
	AudioSystem* mSystem;
	unsigned int mID;
};