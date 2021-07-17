// Request GLSL 3.3
#version 330

// 頂点シェーダーからのTex座標入力
in vec2 fragTexCoord;

// これは、カラーバッファへの出力色に対応します
out vec4 outColor;

// これはテクスチャサンプリングに使用されます
uniform sampler2D uTexture;

void main()
{
	// テクスチャからのサンプルカラー
    outColor = texture(uTexture, fragTexCoord);
}