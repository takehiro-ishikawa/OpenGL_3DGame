#pragma once
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"

// プロトタイプ宣言
class Game;
class Texture;

class Font
{
public:
	Font(Game* game);
	~Font();

	// ファイルのロード/アンロード
	bool Load(const std::string& fileName);
	void Unload();

	// 文字列をテクスチャに描画（コストが高いので文字列が変わる時にだけ呼び出す）
	Texture* RenderText(const std::string& textKey,
		const Vector3& color = Color::White,
		int pointSize = 30);

private:
	// ポイントサイズとフォントデータの連想配列
	std::unordered_map<int, TTF_Font*> mFontData;
	Game* mGame;
};
