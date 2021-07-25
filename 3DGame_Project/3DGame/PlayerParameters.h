#pragma once
#include "Math.h"

// �v���C���[�Ɋ֌W����e��p�����[�^�[��ݒ肷��

#define PLAYER_FILEPATH             "Assets/Player.fbx" // �v���C���[���f���̃t�@�C���p�X
#define PLAYER_ANIMATION_IDLE       "Player_Idle"              // �v���C���[��"�ҋ@"�A�j���[�V������
#define PLAYER_ANIMATION_WALK       "Player_Walk"              // �v���C���[��"���s"�A�j���[�V������
#define PLAYER_ANIMATION_RUN        "Player_Run"               // �v���C���[��"�_�b�V��"�A�j���[�V������
#define PLAYER_ANIMATION_SHOOT      "Player_Shoot"             // �v���C���[��"�ˌ�"�A�j���[�V������
#define PLAYER_ANIMATION_SHOOT_WALK "Player_ShootAndWalk"      // �v���C���[��"�ˌ�&���s"�A�j���[�V������

#define PLAYER_WIDTH  1.4f // AABB�̕�
#define PLAYER_DEPTH  1.4f // AABB�̉��s
#define PLAYER_HEIGHT 3.2f // AABB�̍���

#define PLAYER_WALK_SPEED   400.0f // ���s���x
#define PLAYER_RUN_SPEED    800.0f // �_�b�V�����x
#define PLAYER_ROTATE_SPEED 10.0f  // ��]���x
#define PLAYER_JUMP_SPEED   800.0f // �W�����v�����x

#define PLAYER_MAX_HEALTH 100.0f // �̗͂̍ő�l

#define PLAYER_SHOOT_POSITION_OFFSET Vector3(50.0f, 60.0f, 130.0f);

#define CAMERA_NORMAL_DIST       600.0f // �ʏ펞�̏��L�A�N�^�[�Ƃ̋���
#define CAMERA_AIM_DIST          400.0f // �ˌ����̏��L�A�N�^�[�Ƃ̋���
#define CAMERA_NORMAL_YAWSPEED   Math::Pi * 25
#define CAMERA_NORMAL_PITCHSPEED Math::Pi * 8
#define CAMERA_AIM_YAWSPEED      Math::Pi * 10
#define CAMERA_AIM_PITCHSPEED    Math::Pi * 5
#define CAMERA_NORMAL_OFFSETPOS  Vector3(  0,    0, 300.0f) // �ʏ펞�̃J�����ʒu�̃I�t�Z�b�g�l
#define CAMERA_AIM_OFFSETPOS     Vector3(100.0f, 0, 250.0f)