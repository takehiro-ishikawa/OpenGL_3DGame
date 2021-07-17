#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum class ActorState
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Game����Ăяo�����Update�֐��i�I�[�o�[���C�h�s�j
	void Update(float deltaTime);
	// �A�N�^�[�Ɏ��S�ẴR���|�[�l���g���X�V����i�I�[�o�[���C�h�s�j
	void UpdateComponents(float deltaTime);
	// �A�N�^�[�ŗL�̍X�V�R�[�h�i�I�[�o�[���C�h�\�j
	virtual void UpdateActor(float deltaTime);

	// �Q�[������Ăяo�����ProcessInput�֐��i�I�[�o�[���C�h�s�j
	void ProcessInput(const struct InputState& state);
	// �A�N�^�[�ŗL�̓��̓R�[�h�i�I�[�o�[���C�h�\�j
	virtual void ActorInput(const struct InputState& state);

	// ���[���h�s��
	void ComputeWorldTransform();                                        // ���[���h�s����v�Z
	const Matrix4& GetWorldTransform() const { return mWorldTransform; } // ���[���h�s����擾

	// �Q�b�^�[/�Z�b�^�[
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }

	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }

	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }
	
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // �O���x�N�g��
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // �E���x�N�g��
	Vector3 GetUpward() const { return Vector3::Transform(Vector3::UnitZ, mRotation); }  // ����x�N�g��

	ActorState GetState() const { return mState; }      // �A�N�^�[�̏��
	void SetState(ActorState state) { mState = state; } //     �V

	class Game* GetGame() { return mGame; }        // Game�ւ̃|�C���^

	// �A�N�^�[�̐i�s������V���������ւƉ�]������
	void RotateToNewForward(const Vector3& forward);

	// �R���|�[�l���g�̒ǉ�/�폜
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// �A�N�^�[�̏��
	ActorState mState;

	// Transform
	Matrix4 mWorldTransform;       // ���[���h���W
	Vector3 mPosition;             // �ʒu
	Quaternion mRotation;          // ��]
	float mScale;                  // �X�P�[��
	bool mRecomputeWorldTransform; // ���[���h�s����Čv�Z����K�v�����邩

	std::vector<class Component*> mComponents; // ���g�̃R���|�[�l���g�B
	class Game* mGame;                         // Game�ւ̃|�C���^
};