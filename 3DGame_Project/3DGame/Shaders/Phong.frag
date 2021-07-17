// Request GLSL 3.3
#version 330

// 頂点シェーダーからの入力
in vec2 fragTexCoord; // Tex coord
in vec3 fragNormal;   // 法線（ワールド空間）
in vec3 fragWorldPos; // 位置（ワールド空間）

// これは、カラーバッファへの出力色に対応します
out vec4 outColor;

// これはテクスチャサンプリングに使用されます
uniform sampler2D uTexture;

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

// 表面の鏡面反射指数
uniform float uSpecPower;

// 環境光の強さ
uniform vec3 uAmbientLight;

// 平行光源（今は1つだけ）
uniform DirectionalLight uDirLight;


void main()
{
	// 表面法線N
	vec3 N = normalize(fragNormal);

	// 表面から光源へのベクトルL
	vec3 L = normalize(-uDirLight.mDirection);

	// 表面からカメラへのベクトルV
	vec3 V = normalize(uCameraPos - fragWorldPos);

	// Nに対する-Lの反射R
	vec3 R = normalize(reflect(-L, N));

	// フォンの反射を計算する
	vec3 Phong = uAmbientLight;                                                      // 環境成分
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;                              // 拡散反射成分
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower); // 鏡面反射成分
		Phong += Diffuse + Specular;
	}

	// 最終的な色は、テクスチャの色×Phongの反射光（arpha = 1）
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}