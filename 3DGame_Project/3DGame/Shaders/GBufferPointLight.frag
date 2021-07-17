#version 330

// 頂点シェーダーからの入力
in vec2 fragTexCoord; // テクスチャ座標

// カラーバッファへの出力色に対応
layout(location = 0) out vec4 outColor;

// Gバッファとは異なるテクスチャ
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// 点光源の構造体
struct PointLight
{
	// 光の位置
	vec3 mWorldPos;

	// 拡散色
	vec3 mDiffuseColor;

	// 光の半径
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;

// 画面の幅/高さを格納
uniform vec2 uScreenDimensions;

void main()
{
	// Gバッファをサンプリングする座標を計算
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	// Gバッファからサンプリング
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	
	// 法線および、表面から光源までのベクトルを計算
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	// Phong 拡散反射成分を計算
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// 光源とワールド位置座標との間の距離を求める
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);

		// smoothstep で、内側と外側の半径の間の
		// 範囲[0,1]の輝度値を計算する
		float intensity = smoothstep(uPointLight.mInnerRadius,
									 uPointLight.mOuterRadius, dist);

		// 光の拡散反射は輝度に依存する
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
								vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL;
	}

	// テクスチャの色にPhongの拡散反射を掛けて最終的な色にする
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}