// Request GLSL 3.3
#version 330

// ���_�V�F�[�_�[����̓���
in vec2 fragTexCoord; // Tex coord
in vec3 fragNormal;   // �@���i���[���h��ԁj
in vec3 fragWorldPos; // �ʒu�i���[���h��ԁj

// ����́A�J���[�o�b�t�@�ւ̏o�͐F�ɑΉ����܂�
out vec4 outColor;

// ����̓e�N�X�`���T���v�����O�Ɏg�p����܂�
uniform sampler2D uTexture;

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

// �\�ʂ̋��ʔ��ˎw��
uniform float uSpecPower;

// �����̋���
uniform vec3 uAmbientLight;

// ���s�����i����1�����j
uniform DirectionalLight uDirLight;


void main()
{
	// �\�ʖ@��N
	vec3 N = normalize(fragNormal);

	// �\�ʂ�������ւ̃x�N�g��L
	vec3 L = normalize(-uDirLight.mDirection);

	// �\�ʂ���J�����ւ̃x�N�g��V
	vec3 V = normalize(uCameraPos - fragWorldPos);

	// N�ɑ΂���-L�̔���R
	vec3 R = normalize(reflect(-L, N));

	// �t�H���̔��˂��v�Z����
	vec3 Phong = uAmbientLight;                                                      // ������
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;                              // �g�U���ː���
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower); // ���ʔ��ː���
		Phong += Diffuse + Specular;
	}

	// �ŏI�I�ȐF�́A�e�N�X�`���̐F�~Phong�̔��ˌ��iarpha = 1�j
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}