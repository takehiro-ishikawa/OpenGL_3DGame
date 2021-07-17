// Request GLSL 3.3
#version 330

// ���_�V�F�[�_�[�����Tex���W����
in vec2 fragTexCoord;

// ����́A�J���[�o�b�t�@�ւ̏o�͐F�ɑΉ����܂�
out vec4 outColor;

// ����̓e�N�X�`���T���v�����O�Ɏg�p����܂�
uniform sampler2D uTexture;

void main()
{
	// �e�N�X�`������̃T���v���J���[
    outColor = texture(uTexture, fragTexCoord);
}