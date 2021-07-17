// Request GLSL 3.3
#version 330

// ���[���h���W�ƃr���[�ˉe�s���uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// �s��p���b�g��uniform
uniform mat4 uMatrixPalette[96];

// ���_���C�A�E�g
layout(location = 0) in vec3 inPosition;    // �ʒu
layout(location = 1) in vec3 inNormal;      // �@��
layout(location = 2) in vec2 inTexCoord;    // �e�N�X�`�����W
layout(location = 3) in uvec4 inSkinBones;  // �e�����󂯂�4�̃{�[���̃C���f�b�N�X
layout(location = 4) in vec4 inSkinWeights; // �e�����󂯂�4�̃{�[���̏d��

// �t���O�����g�V�F�[�_�[�ւ̏o��
out vec2 fragTexCoord; // �C�ӂ̒��_�o�́i�ʒu�ȊO�j
out vec3 fragNormal;   // �@�� (���[���h���)
out vec3 fragWorldPos; // �ʒu (���[���h���)

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ�����
	vec4 pos = vec4(inPosition, 1.0);

	// �ʒu�̃X�L�j���O    
	vec4 skinnedPos = pos * uMatrixPalette[inSkinBones.x] * inSkinWeights.x;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// �ʒu�����[���h��Ԃɕϊ�����
	skinnedPos = skinnedPos * uWorldTransform;

	// ���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = skinnedPos.xyz;

	// �N���b�v��Ԃɕϊ�����
	gl_Position = skinnedPos * uViewProj;

	// ���_�@���̃X�L�j���O
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		          + (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		          + (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		          + (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// �@�������[���h��Ԃɕϊ��iw = 0�j
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}