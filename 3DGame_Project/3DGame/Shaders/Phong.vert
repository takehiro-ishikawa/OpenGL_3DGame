// Request GLSL 3.3
#version 330

// ���[���h���W�ƃr���[�ˉe�s���uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// ���_���C�A�E�g
layout(location = 0) in vec3 inPosition; // �ʒu
layout(location = 1) in vec3 inNormal;   // �@��
layout(location = 2) in vec2 inTexCoord; // �e�N�X�`�����W

// �t���O�����g�V�F�[�_�[�ւ̏o��
out vec2 fragTexCoord; // �C�ӂ̒��_�o�́i�ʒu�ȊO�j
out vec3 fragNormal;   // �@�� (���[���h���)
out vec3 fragWorldPos; // �ʒu (���[���h���)

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ�����
	vec4 pos = vec4(inPosition, 1.0);

	// �ʒu�����[���h��Ԃɕϊ�����
	pos = pos * uWorldTransform;

	// ���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = pos.xyz;

	// �N���b�v��Ԃɕϊ�����
	gl_Position = pos * uViewProj;

	// �@�������[���h��Ԃɕϊ��iw = 0�j
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}