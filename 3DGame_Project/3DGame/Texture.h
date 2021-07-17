#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	// SDL_Surface からテクスチャに変換する
	void CreateFromSurface(struct SDL_Surface* surface);

	// レンダリング用のテクスチャを作成する
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