#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include <GL/glew.h>
#include "SkeletalMeshComponent.h"
#include "GBuffer.h"
#include "PointLightComponent.h"

Renderer::Renderer(Game* game)
	:mGame(game)
	, mSpriteShader(nullptr)
	, mMeshShader(nullptr)
	, mSkinnedShader(nullptr)
	, mGGlobalShader(nullptr)
	, mGBuffer(nullptr)
	, mGPointLightShader(nullptr)
	, mCursorPosition(Vector2::Zero)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	// 画面の縦横幅を記録
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// OpenGL属性を設定する
	// コアOpenGLプロファイルを使用する
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// バージョン3.3を指定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBAチャネルあたり8ビットのカラーバッファを要求する
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);    // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);  // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);   // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);  // Alpha
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // デプスバッファ

	// ダブルバッファリングを有効にする
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGLにハードウェアアクセラレーションの使用を強制する
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("3DGame_Study", 250, 40,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// OpenGLコンテキストを作成する
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEWを初期化する
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// 一部のプラットフォームでは、GLEWは無害なエラーコードを出力します。
	// クリアする
	glGetError();

	// シェーダーを作成/コンパイルできることを確認する
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// スプライトを描画するための矩形を作成する
	CreateSpriteVerts();

	// Gバッファの作成
	mGBuffer = new GBuffer();

	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);

	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	mPointLightMesh = GetMesh("Assets/Models/Sphere.fbx");

	// ビューポート行列の設定
	mViewPort.mat[0][0] = SCREEN_WIDTH / 2; 
	mViewPort.mat[0][1] = 0; 
	mViewPort.mat[0][2] = 0; 
	mViewPort.mat[0][3] = 0;
	mViewPort.mat[1][0] = 0; 
	mViewPort.mat[1][1] = -SCREEN_HEIGHT / 2; 
	mViewPort.mat[1][2] = 0; 
	mViewPort.mat[1][3] = 0;
	mViewPort.mat[2][0] = 0; 
	mViewPort.mat[2][1] = 0; 
	mViewPort.mat[2][2] = 1; 
	mViewPort.mat[2][3] = 0;
	mViewPort.mat[3][0] = SCREEN_WIDTH / 2; 
	mViewPort.mat[3][1] = SCREEN_HEIGHT / 2; 
	mViewPort.mat[3][2] = 0; 
	mViewPort.mat[3][3] = 1;

	return true;
}

void Renderer::Shutdown()
{
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}

	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	mSkinnedShader->Unload();
	delete mSkinnedShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// テクスチャを破棄する
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// メッシュを破棄する
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// 3DシーンをGバッファに描画
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);

	// フレームバッファを 0（画面のフレームバッファ）に戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Gバッファを使って描画する
	DrawFromGBuffer();

	// すべてのスプライトコンポーネントを描画する
	// デプスバッファリングを無効にする/アルファブレンドを有効にする
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// シェーダー/ vaoをアクティブに設定する
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mSpriteShader);
		}
	}

	// 全てのUIScreenを描画する
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}

	// バッファを交換します
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// ソートされたベクトルで挿入点を見つける
	// （自身よりも描画順序が高い最初の要素）
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレータの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComps.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
}

void Renderer::AddPointLight(PointLightComponent* light)
{
	mPointLights.emplace_back(light);
}

void Renderer::RemovePointLight(PointLightComponent* light)
{
	auto iter = std::find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;

	// ロード済みか確認
	auto iter = mTextures.find(fileName);
	// ロード済み
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	// 未ロード
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;

	// ロード済みか確認
	auto iter = mMeshes.find(fileName);
	// ロード済み
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	// 未ロード
	else
	{
		m = new Mesh();
		
		if (m->Load(mGame->GetFBXData(fileName), this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj,
	float viewPortScale, bool lit)
{
	// これから書き込むフレームバッファに設定
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// スケールに基づいてビューポートサイズを設定
	glViewport(0, 0,
		static_cast<int>(mScreenWidth * viewPortScale),
		static_cast<int>(mScreenHeight * viewPortScale)
	);

	// カラーバッファ/深度バッファをクリア
	glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 全てのメッシュコンポーネントを描画
	// デプスバッファリングを有効にする/アルファブレンドを無効にする
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// メッシュシェーダーをアクティブに設定する
	mMeshShader->SetActive();
	// ビュー射影行列を更新する
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);
	// 照明のユニフォームを更新する
	if (lit)
	{
		SetLightUniforms(mMeshShader, view);
	}
	for (auto mc : mMeshComps)
	{
		if (mc->GetVisible())
		{
			mc->Draw(mMeshShader);
		}
	}

	// スキンメッシュがあればここで描画する
	mSkinnedShader->SetActive();

	// ビュー射影行列を更新
	mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);

	// 照明のユニフォームを更新する
	if (lit)
	{
		SetLightUniforms(mSkinnedShader, view);
	}

	for (auto sk : mSkeletalMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mSkinnedShader);
		}
	}
}

void Renderer::DrawFromGBuffer()
{
	// グローバルライティングパスでは深度テストを禁止
	glDisable(GL_DEPTH_TEST);

	// Gバッファシェーダーをアクティブにする
	mGGlobalShader->SetActive();

	// スプライト頂点シェーダーをアクティブにする
	mSpriteVerts->SetActive();

	// サンプリングするGバッファテクスチャを設定
	mGBuffer->SetTexturesActive();

	// ライティングのuniformを設定
	SetLightUniforms(mGGlobalShader, mView);

	// 矩形のための三角形を描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Gバッファの深度バッファの内容を
	// デフォルトのフレームバッファにコピー
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// 深度テストを有効にするが、
	// 深度バッファへの書き込みは無効にする
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// 点光源シェーダーとメッシュをアクティブにする
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();

	// ビュー射影行列を設定
	mGPointLightShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Ｇバッファのテクスチャをサンプリング用に設定
	mGBuffer->SetTexturesActive();

	// 点光源の色は既存の色に加算する
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// 点光源を描画する
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mGPointLightShader, mPointLightMesh);
	}
}

bool Renderer::LoadShaders()
{
	/* -------- スプライトシェーダーを作成する -------- */
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();

	// ビュー射影行列を設定する
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);


	/* -------- メッシュシェーダーを作成する -------- */
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();

	// 頂点シェーダーにビュー射影行列を設定する
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(FIELD_OF_VIEW),
		mScreenWidth, mScreenHeight, NEAR_PLANE, FAR_PLANE);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);


	/* -------- スキンのシェーダーを作成する -------- */
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		SDL_Log("Failed to load SkinnedShader.");
		return false;
	}

	mSkinnedShader->SetActive();

	// ビュー射影行列を設定する
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(FIELD_OF_VIEW),
		mScreenWidth, mScreenHeight, NEAR_PLANE, FAR_PLANE);
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);


	/* -------- グローバルライティングシェーダーを作成する -------- */
	mGGlobalShader = new Shader();

	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag"))
	{
		return false;
	}

	// GBufferのサンプラーにインデックスを割り当てる
	mGGlobalShader->SetActive();
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);

	// ビュー射影はスプライトと同じ
	mGGlobalShader->SetMatrixUniform("uViewProj", spriteViewProj);

	// ワールド変換で全画面へのスケーリングと y の反転を行う
	Matrix4 gbufferWorld = Matrix4::CreateScale(mScreenWidth, -mScreenHeight, 1.0f);

	mGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);


	/* -------- 点光源シェーダーを作成する -------- */
	mGPointLightShader = new Shader();

	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag"))
	{
		return false;
	}
	
	// サンプラーにインデックスを割り当てる
	mGPointLightShader->SetActive();
	mGPointLightShader->SetIntUniform("uGDiffuse", 0);
	mGPointLightShader->SetIntUniform("uGNormal", 1);
	mGPointLightShader->SetIntUniform("uGWorldPos", 2);
	mGPointLightShader->SetVector2Uniform("uScreenDimensions", Vector2(mScreenWidth, mScreenHeight));

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // 左上
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // 右上
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // 右下
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // 右上
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::Layout::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader, const Matrix4& view)
{
	// カメラの位置はビューを反転して求める
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// 環境光
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	// 平行光源
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// screenPointをデバイス座標（-1から+1の間）に変換
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;

	// 非射影行列によるベクトルの変換
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

Vector2 Renderer::Convert3DtoScreenPos(const Vector3& WorldPos)
{
	// 3D座標を同次座標にする
	float posX = WorldPos.x, posY = WorldPos.y, posZ = WorldPos.z, posW = 1;

	// ビュー行列を掛ける
	posX = posX * mView.mat[0][0] + posY * mView.mat[1][0] + posZ * mView.mat[2][0] + posW * mView.mat[3][0];
	posY = posX * mView.mat[0][1] + posY * mView.mat[1][1] + posZ * mView.mat[2][1] + posW * mView.mat[3][1];
	posZ = posX * mView.mat[0][2] + posY * mView.mat[1][2] + posZ * mView.mat[2][2] + posW * mView.mat[3][2];
	posW = posX * mView.mat[0][3] + posY * mView.mat[1][3] + posZ * mView.mat[2][3] + posW * mView.mat[3][3];

	// 射影行列を掛ける
	posX = posX * mProjection.mat[0][0] + posY * mProjection.mat[1][0] + posZ * mProjection.mat[2][0] + posW * mProjection.mat[3][0];
	posY = posX * mProjection.mat[0][1] + posY * mProjection.mat[1][1] + posZ * mProjection.mat[2][1] + posW * mProjection.mat[3][1];
	posZ = posX * mProjection.mat[0][2] + posY * mProjection.mat[1][2] + posZ * mProjection.mat[2][2] + posW * mProjection.mat[3][2];
	posW = posX * mProjection.mat[0][3] + posY * mProjection.mat[1][3] + posZ * mProjection.mat[2][3] + posW * mProjection.mat[3][3];

	// ビューポート行列を掛ける
	posX = posX * mViewPort.mat[0][0] + posY * mViewPort.mat[1][0] + posZ * mViewPort.mat[2][0] + posW * mViewPort.mat[3][0];
	posY = posX * mViewPort.mat[0][1] + posY * mViewPort.mat[1][1] + posZ * mViewPort.mat[2][1] + posW * mViewPort.mat[3][1];
	posZ = posX * mViewPort.mat[0][2] + posY * mViewPort.mat[1][2] + posZ * mViewPort.mat[2][2] + posW * mViewPort.mat[3][2];
	posW = posX * mViewPort.mat[0][3] + posY * mViewPort.mat[1][3] + posZ * mViewPort.mat[2][3] + posW * mViewPort.mat[3][3];

	return Vector2(posX / posW - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - posY / posW);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// 始点を近接平面での画面の中心にする
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);

	// 終点を近接平面と遠方平面の間の画面の中心に定める
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);

	// 方向ベクトルを定める
	outDir = end - outStart;
	outDir.Normalize();
}