#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	// SDL_Surface ����e�N�X�`���ɕϊ�����
	void CreateFromSurface(struct SDL_Surface* surface);

	// �����_�����O�p�̃e�N�X�`�����쐬����
	void CreateForRendering(int width, int height, unsigned int format);

	void SetActive(int index = 0);

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	unsigned int GetTextureID() const { return mTextureID; }

private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
};