#version 330

// 頂点シェーダーからの入力
in vec2 fragTexCoord; // テクスチャ座標

// カラーバッファへの出力色に対応
layout(location = 0) out vec4 outColor;

// Gバッファのテクスチャ
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// 平行光源用の構造体
struct DirectionalLight
{
	// 光の方向
	vec3 mDirection;

	// 拡散反射色
	vec3 mDiffuseColor;

	// 鏡面反射色
	vec3 mSpecColor;
};

// ライティング用 uniform
// カメラの位置（ワールド空間）
uniform vec3 uCameraPos;

// 環境光の強さ
uniform vec3 uAmbientLight;

// 平行光源
uniform DirectionalLight uDirLight;

void main()
{
    // アルベド / 法線 / ワールド位置座標をGバッファからサンプリング
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	// 表面法線N
	vec3 N = normalize(gbufferNorm);

	// 表面から光源へのベクトルL
	vec3 L = normalize(-uDirLight.mDirection);

	// 表面からカメラへのベクトルV
	vec3 V = normalize(uCameraPos - gbufferWorldPos);

	// Nに対する-Lの反射R
	vec3 R = normalize(reflect(-L, N));

	// フォンの反射を計算する
	vec3 Phong = uAmbientLight;

	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * dot(N, L);
		Phong += Diffuse;
	}

	// 0～1 RGB値の間でライトをクランプ
	Phong = clamp(Phong, 0.0, 1.0);

	// 最終的な色は、アルベド×Phongの反射光（arpha = 1）
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}