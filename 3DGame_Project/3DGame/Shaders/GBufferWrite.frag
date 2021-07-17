
#version 330

// 頂点シェーダーからの入力
in vec2 fragTexCoord; // テクスチャ座標
in vec3 fragNormal;   // 法線(ワールド空間)
in vec3 fragWorldPos; // 位置(ワールド空間)

// Gバッファへの出力
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// アルベドテクスチャのサンプラー
uniform sampler2D uTexture;

void main()
{
	// アルベドをテクスチャから得る
	outDiffuse = texture(uTexture, fragTexCoord).xyz;

	// 法線/ワールド位置座標は、そのまま渡す
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}
