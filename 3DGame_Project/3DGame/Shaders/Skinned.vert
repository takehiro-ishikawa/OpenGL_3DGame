// Request GLSL 3.3
#version 330

// ワールド座標とビュー射影行列のuniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 行列パレットのuniform
uniform mat4 uMatrixPalette[96];

// 頂点レイアウト
layout(location = 0) in vec3 inPosition;    // 位置
layout(location = 1) in vec3 inNormal;      // 法線
layout(location = 2) in vec2 inTexCoord;    // テクスチャ座標
layout(location = 3) in uvec4 inSkinBones;  // 影響を受ける4つのボーンのインデックス
layout(location = 4) in vec4 inSkinWeights; // 影響を受ける4つのボーンの重み

// フラグメントシェーダーへの出力
out vec2 fragTexCoord; // 任意の頂点出力（位置以外）
out vec3 fragNormal;   // 法線 (ワールド空間)
out vec3 fragWorldPos; // 位置 (ワールド空間)

void main()
{
	// 位置を同次座標系に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// 位置のスキニング    
	vec4 skinnedPos = pos * uMatrixPalette[inSkinBones.x] * inSkinWeights.x;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	    skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// 位置をワールド空間に変換する
	skinnedPos = skinnedPos * uWorldTransform;

	// ワールド空間の位置を保存
	fragWorldPos = skinnedPos.xyz;

	// クリップ空間に変換する
	gl_Position = skinnedPos * uViewProj;

	// 頂点法線のスキニング
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		          + (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		          + (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		          + (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// 法線をワールド空間に変換（w = 0）
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}