#include "Renderer.h"
#include <GL/glew.h>
#include "Texture.h"

GBuffer::GBuffer()
	:mBufferID(0)
{

}

GBuffer::~GBuffer()
{

}

bool GBuffer::Create(int width, int height)
{
	// �t���[���o�b�t�@�I�u�W�F�N�g���쐬����mBufferID�ɕۑ�
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

	// �t���[���o�b�t�@�p�̐[�x�o�b�t�@��ǉ�
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// G�o�b�t�@�̏o�͗p�Ƀe�N�X�`�����쐬
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();

		// �e�e�N�X�`���́A32�r�b�gfloat3�����̃t�H�[�}�b�g
		tex->CreateForRendering(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);

		// ���̃e�N�X�`�����J���[�o�͂ɃA�^�b�`����
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			tex->GetTextureID(), 0);
	}

	// �J���[�A�^�b�`�����g�̔z����쐬
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// �`���o�b�t�@�̃��X�g��ݒ�
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
		attachments.data());

	// �S�Ė��Ȃ��������H
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();
		return false;
	}

	return true;
}

void GBuffer::Destroy()
{
	glDeleteFramebuffers(1, &mBufferID);
	for (Texture* t : mTextures)
	{
		t->Unload();
		delete t;
	}
}

Texture* GBuffer::GetTexture(Type type)
{
	if (mTextures.size() > 0)
	{
		return mTextures[type];
	}
	else
	{
		return nullptr;
	}
}

void GBuffer::SetTexturesActive()
{
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		mTextures[i]->SetActive(i);
	}
}