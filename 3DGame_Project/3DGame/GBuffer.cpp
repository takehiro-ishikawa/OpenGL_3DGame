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
	// フレームバッファオブジェクトを作成してmBufferIDに保存
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

	// フレームバッファ用の深度バッファを追加
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// Gバッファの出力用にテクスチャを作成
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();

		// 各テクスチャは、32ビットfloat3成分のフォーマット
		tex->CreateForRendering(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);

		// このテクスチャをカラー出力にアタッチする
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			tex->GetTextureID(), 0);
	}

	// カラーアタッチメントの配列を作成
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// 描画先バッファのリストを設定
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
		attachments.data());

	// 全て問題なく動くか？
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