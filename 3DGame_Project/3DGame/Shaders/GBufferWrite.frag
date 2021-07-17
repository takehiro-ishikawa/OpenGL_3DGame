
#version 330

// ���_�V�F�[�_�[����̓���
in vec2 fragTexCoord; // �e�N�X�`�����W
in vec3 fragNormal;   // �@��(���[���h���)
in vec3 fragWorldPos; // �ʒu(���[���h���)

// G�o�b�t�@�ւ̏o��
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// �A���x�h�e�N�X�`���̃T���v���[
uniform sampler2D uTexture;

void main()
{
	// �A���x�h���e�N�X�`�����瓾��
	outDiffuse = texture(uTexture, fragTexCoord).xyz;

	// �@��/���[���h�ʒu���W�́A���̂܂ܓn��
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}
