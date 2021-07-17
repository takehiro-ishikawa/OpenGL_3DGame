#pragma once
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"

// �v���g�^�C�v�錾
class Game;
class Texture;

class Font
{
public:
	Font(Game* game);
	~Font();

	// �t�@�C���̃��[�h/�A�����[�h
	bool Load(const std::string& fileName);
	void Unload();

	// ��������e�N�X�`���ɕ`��i�R�X�g�������̂ŕ����񂪕ς�鎞�ɂ����Ăяo���j
	Texture* RenderText(const std::string& textKey,
		const Vector3& color = Color::White,
		int pointSize = 30);

private:
	// �|�C���g�T�C�Y�ƃt�H���g�f�[�^�̘A�z�z��
	std::unordered_map<int, TTF_Font*> mFontData;
	Game* mGame;
};
