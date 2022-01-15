#pragma once
#include <vector>
#include <unordered_map>
#include "SDL/SDL.h"
#include "Math.h"

#pragma region プロトタイプ宣言
class Game;
class Mesh;
class Texture;
class VertexArray;
class Shader;
class GBuffer;
class SpriteComponent;
class MeshComponent;
class PointLightComponent;
class SkeletalMeshComponent;
class InstancedData;
#pragma endregion

// 平行光源用
struct DirectionalLight
{
	// 光の方向
	Vector3 mDirection;

	// 拡散反射色
	Vector3 mDiffuseColor;

	// 鏡面反射色
	Vector3 mSpecColor;
};

// 全ての描画処理を管理
class Renderer
{
public:
	Renderer(Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	// 描画する
	// 最初にメッシュシェーダーで全ての3Dメッシュコンポーネントを描画し、
	// 次にスプライトシェーダーで全てのスプライトコンポーネントを描画する
	void Draw();

	// SpriteComponentの追加と削除
	void AddSprite(SpriteComponent* sprite);
	void RemoveSprite(SpriteComponent* sprite);

	// MeshComponentの追加と削除
	void AddMeshComp(MeshComponent* mesh);
	void RemoveMeshComp(MeshComponent* mesh);

	// PointLightComponentの追加と削除
	void AddPointLight(PointLightComponent* light);
	void RemovePointLight(PointLightComponent* light);

	Texture* GetTexture(const std::string& fileName); // テクスチャの取得
	Mesh* GetMesh(const std::string& fileName);       // メッシュの取得

	// ビュー行列を設定する
	void SetViewMatrix(const Matrix4& view) { mView = view; } 

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; } // 環境光を設定する
	DirectionalLight& GetDirectionalLight() { return mDirLight; }             // 平行光源を取得する

	// スクリーン空間座標を指定すると、
	// 現在の3Dビュー/射影行列に基づいて、それをワールド空間に逆射影する
	// 期待される範囲:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1]  0はカメラに近く、1は遠くにある
	Vector3 Unproject(const Vector3& screenPoint) const;

	// 画面ベクトルの開始点と方向を取得する
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

	// ゲーム画面の幅を取得
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
	
	void SetPointLightMesh(Mesh* mesh) { mPointLightMesh = mesh; }

	Vector2 GetCursorPosition() { return mCursorPosition; }
	void SetCursorPosition(Vector2 position) { mCursorPosition = position; }

	InstancedData* GetInstancedData(const std::string& fileName);

	// 3D座標をスクリーン座標に変換する
	Vector2 Convert3DtoScreenPos(const Vector3& WorldPos);

private:

	// レンダリング処理のヘルパー関数
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj,
		float viewPortScale = 1.0f, bool lit = true);

	void DrawFromGBuffer();

	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(Shader* shader, const Matrix4& view);

	std::unordered_map<std::string, Texture*> mTextures; // ロードされたテクスチャのマップ
	std::unordered_map<std::string, Mesh*> mMeshes;      // ロードされたメッシュのマップ

	// 描画する全てのスプライトコンポーネント
	std::vector<SpriteComponent*> mSprites;

	// 描画する全てのメッシュコンポーネント
	std::vector<MeshComponent*> mMeshComps;
	std::vector<SkeletalMeshComponent*> mSkeletalMeshes;

	// 描画する全てのインスタンシングメッシュデータ
	std::unordered_map<std::string, InstancedData*> mInstancedData;

	// Game
	Game* mGame;

	// スプライトの頂点配列
	VertexArray* mSpriteVerts; 

	// 使用するシェーダー
	Shader* mSpriteShader;        // 画像用
	Shader* mMeshShader;          // メッシュ用
	Shader* mSkinnedShader;       // スキン用
	Shader* mInstancedMeshShader; // インスタンシング描画するメッシュ用

	// 3Dシェーダー用行列
	Matrix4 mView;       // ビュー行列
	Matrix4 mProjection; // 射影行列
	Matrix4 mViewPort;   // ビューポート行列

	// 照明データ
	Vector3 mAmbientLight;      // 環境光
	DirectionalLight mDirLight; // 平行光源

	// 画面の幅/高さ
	float mScreenWidth;
	float mScreenHeight;

	// Window
	SDL_Window* mWindow;
	// OpenGL コンテクスト
	SDL_GLContext mContext;

	// Gバッファ
	GBuffer* mGBuffer;
	Shader* mGGlobalShader;

	// 点光源
	Shader* mGPointLightShader;
	std::vector<PointLightComponent*> mPointLights;
	Mesh* mPointLightMesh;

	// カーソル
	Vector2 mCursorPosition;
};

class GBuffer
{
public:

	// Gバッファに格納されるデータの種類
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	GBuffer();
	~GBuffer();

	// Gバッファの生成と破棄
	bool Create(int width, int height);
	void Destroy();

	// 指定するタイプのテクスチャを取得
	Texture* GetTexture(Type type);

	// フレームバッファオブジェクトの ID を取得
	unsigned int GetBufferID() { return mBufferID; }

	// Gバッファの各テクスチャを対応するテクスチャインデックスにバインドする
	void SetTexturesActive();

private:

	// Gバッファに割り当てられたテクスチャ
	std::vector<Texture*> mTextures;

	// フレームバッファオブジェクトのID
	unsigned int mBufferID;
};