#version 330

// ワールド座標とビュー射影行列のuniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 頂点レイアウト宣言
// 属性0は位置、1は法線、2はテクスチャ座標
layout(location = 0) in vec3 inPosition; // 位置
layout(location = 1) in vec3 inNormal;   // 法線
layout(location = 2) in vec2 inTexCoord; // テクスチャ座標

// 任意の頂点出力（位置以外）
out vec2 fragTexCoord;

void main()
{
	// 位置を同次座標に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// 頂点座標をクリップ空間に変換する
	gl_Position = pos * uWorldTransform * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに渡します
	fragTexCoord = inTexCoord;
}