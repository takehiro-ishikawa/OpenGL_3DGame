// Request GLSL 3.3
#version 330

// ビュー射影行列のuniform
uniform mat4 uViewProj;

// 頂点レイアウト
layout(location = 0) in vec3 inPosition;  // 位置
layout(location = 1) in vec3 inNormal;    // 法線
layout(location = 2) in vec2 inTexCoord;  // テクスチャ座標

layout(location = 3) in mat4 inWorldTransform; // ワールド変換行列

// フラグメントシェーダーへの出力
out vec2 fragTexCoord; // 任意の頂点出力（位置以外）
out vec3 fragNormal;   // 法線 (ワールド空間)
out vec3 fragWorldPos; // 位置 (ワールド空間)

void main()
{
	// 位置を同次座標系に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// 位置をワールド空間に変換する
	pos = pos * inWorldTransform;

	// ワールド空間の位置を保存
	fragWorldPos = pos.xyz;

	// クリップ空間に変換する
	gl_Position = pos * uViewProj;

	// 法線をワールド空間に変換（w = 0）
	fragNormal = (vec4(inNormal, 0.0f) * inWorldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}