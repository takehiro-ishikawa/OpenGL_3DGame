#version 330

// ���_�V�F�[�_�[����̓���
in vec2 fragTexCoord; // �e�N�X�`�����W

// �J���[�o�b�t�@�ւ̏o�͐F�ɑΉ�
layout(location = 0) out vec4 outColor;

// G�o�b�t�@�̃e�N�X�`��
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// ���s�����p�̍\����
struct DirectionalLight
{
	// ���̕���
	vec3 mDirection;

	// �g�U���ːF
	vec3 mDiffuseColor;

	// ���ʔ��ːF
	vec3 mSpecColor;
};

// ���C�e�B���O�p uniform
// �J�����̈ʒu�i���[���h��ԁj
uniform vec3 uCameraPos;

// �����̋���
uniform vec3 uAmbientLight;

// ���s����
uniform DirectionalLight uDirLight;

void main()
{
    // �A���x�h / �@�� / ���[���h�ʒu���W��G�o�b�t�@����T���v�����O
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	// �\�ʖ@��N
	vec3 N = normalize(gbufferNorm);

	// �\�ʂ�������ւ̃x�N�g��L
	vec3 L = normalize(-uDirLight.mDirection);

	// �\�ʂ���J�����ւ̃x�N�g��V
	vec3 V = normalize(uCameraPos - gbufferWorldPos);

	// N�ɑ΂���-L�̔���R
	vec3 R = normalize(reflect(-L, N));

	// �t�H���̔��˂��v�Z����
	vec3 Phong = uAmbientLight;

	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * dot(N, L);
		Phong += Diffuse;
	}

	// 0�`1 RGB�l�̊ԂŃ��C�g���N�����v
	Phong = clamp(Phong, 0.0, 1.0);

	// �ŏI�I�ȐF�́A�A���x�h�~Phong�̔��ˌ��iarpha = 1�j
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}