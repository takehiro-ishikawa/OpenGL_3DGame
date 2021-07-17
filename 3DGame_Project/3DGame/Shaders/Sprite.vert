// Request GLSL 3.3
#version 330

// ワールド座標とビュー射影行列のuniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 属性0は位置座標、1は法線ベクトル、2はテクスチャ座標
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// 任意の頂点出力（位置以外）
out vec2 fragTexCoord;

void main()
{
	// // 位置座標を同次座標に変換
	vec4 pos = vec4(inPosition, 1.0);

	// 位置をワールド空間に変換した後、クリップ空間に変換する
	gl_Position = pos * uWorldTransform * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}