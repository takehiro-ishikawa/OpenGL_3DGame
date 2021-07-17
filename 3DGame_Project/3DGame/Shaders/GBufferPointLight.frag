#version 330

// ���_�V�F�[�_�[����̓���
in vec2 fragTexCoord; // �e�N�X�`�����W

// �J���[�o�b�t�@�ւ̏o�͐F�ɑΉ�
layout(location = 0) out vec4 outColor;

// G�o�b�t�@�Ƃ͈قȂ�e�N�X�`��
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// �_�����̍\����
struct PointLight
{
	// ���̈ʒu
	vec3 mWorldPos;

	// �g�U�F
	vec3 mDiffuseColor;

	// ���̔��a
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;

// ��ʂ̕�/�������i�[
uniform vec2 uScreenDimensions;

void main()
{
	// G�o�b�t�@���T���v�����O������W���v�Z
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	// G�o�b�t�@����T���v�����O
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	
	// �@������сA�\�ʂ�������܂ł̃x�N�g�����v�Z
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	// Phong �g�U���ː������v�Z
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// �����ƃ��[���h�ʒu���W�Ƃ̊Ԃ̋��������߂�
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);

		// smoothstep �ŁA�����ƊO���̔��a�̊Ԃ�
		// �͈�[0,1]�̋P�x�l���v�Z����
		float intensity = smoothstep(uPointLight.mInnerRadius,
									 uPointLight.mOuterRadius, dist);

		// ���̊g�U���˂͋P�x�Ɉˑ�����
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
								vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL;
	}

	// �e�N�X�`���̐F��Phong�̊g�U���˂��|���čŏI�I�ȐF�ɂ���
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}