// Request GLSL 3.3
#version 330

// ワールド座標とビュー射影行列のuniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 頂点レイアウト
// 属性0は位置、1は法線、2はテクスチャ座標です。
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// フラグメントシェーダーへの出力
out vec2 fragTexCoord; // 任意の頂点出力（位置以外）
out vec3 fragNormal;   // 法線 (ワールド空間)
out vec3 fragWorldPos; // 位置 (ワールド空間)

void main()
{
	// 位置を同次座標系に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// 位置をワールド空間に変換する
	pos = pos * uWorldTransform;

	// ワールド空間の位置を保存
	fragWorldPos = pos.xyz;

	// クリップ空間に変換する
	gl_Position = pos * uViewProj;

	// 法線をワールド空間に変換（w = 0）
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}