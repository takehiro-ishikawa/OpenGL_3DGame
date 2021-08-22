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
	// ��ʂ̏c�������L�^
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// OpenGL������ݒ肷��
	// �R�AOpenGL�v���t�@�C�����g�p����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// �o�[�W����3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA�`���l��������8�r�b�g�̃J���[�o�b�t�@��v������
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);    // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);  // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);   // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);  // Alpha
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // �f�v�X�o�b�t�@

	// �_�u���o�b�t�@�����O��L���ɂ���
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL�Ƀn�[�h�E�F�A�A�N�Z�����[�V�����̎g�p����������
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("3DGame_Study", 250, 40,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// OpenGL�R���e�L�X�g���쐬����
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW������������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// �ꕔ�̃v���b�g�t�H�[���ł́AGLEW�͖��Q�ȃG���[�R�[�h���o�͂��܂��B
	// �N���A����
	glGetError();

	// �V�F�[�_�[���쐬/�R���p�C���ł��邱�Ƃ��m�F����
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// �X�v���C�g��`�悷�邽�߂̋�`���쐬����
	CreateSpriteVerts();

	// G�o�b�t�@�̍쐬
	mGBuffer = new GBuffer();

	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);

	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	mPointLightMesh = GetMesh("Assets/Models/Sphere.fbx");

	// �r���[�|�[�g�s��̐ݒ�
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
	// �e�N�X�`����j������
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// ���b�V����j������
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// 3D�V�[����G�o�b�t�@�ɕ`��
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);

	// �t���[���o�b�t�@�� 0�i��ʂ̃t���[���o�b�t�@�j�ɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// G�o�b�t�@���g���ĕ`�悷��
	DrawFromGBuffer();

	// ���ׂẴX�v���C�g�R���|�[�l���g��`�悷��
	// �f�v�X�o�b�t�@�����O�𖳌��ɂ���/�A���t�@�u�����h��L���ɂ���
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// �V�F�[�_�[/ vao���A�N�e�B�u�ɐݒ肷��
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mSpriteShader);
		}
	}

	// �S�Ă�UIScreen��`�悷��
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}

	// �o�b�t�@���������܂�
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// �\�[�g���ꂽ�x�N�g���ő}���_��������
	// �i���g�����`�揇���������ŏ��̗v�f�j
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

	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}������
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

	// ���[�h�ς݂��m�F
	auto iter = mTextures.find(fileName);
	// ���[�h�ς�
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	// �����[�h
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

	// ���[�h�ς݂��m�F
	auto iter = mMeshes.find(fileName);
	// ���[�h�ς�
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	// �����[�h
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
	// ���ꂩ�珑�����ރt���[���o�b�t�@�ɐݒ�
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// �X�P�[���Ɋ�Â��ăr���[�|�[�g�T�C�Y��ݒ�
	glViewport(0, 0,
		static_cast<int>(mScreenWidth * viewPortScale),
		static_cast<int>(mScreenHeight * viewPortScale)
	);

	// �J���[�o�b�t�@/�[�x�o�b�t�@���N���A
	glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �S�Ẵ��b�V���R���|�[�l���g��`��
	// �f�v�X�o�b�t�@�����O��L���ɂ���/�A���t�@�u�����h�𖳌��ɂ���
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// ���b�V���V�F�[�_�[���A�N�e�B�u�ɐݒ肷��
	mMeshShader->SetActive();
	// �r���[�ˉe�s����X�V����
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);
	// �Ɩ��̃��j�t�H�[�����X�V����
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

	// �X�L�����b�V��������΂����ŕ`�悷��
	mSkinnedShader->SetActive();

	// �r���[�ˉe�s����X�V
	mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);

	// �Ɩ��̃��j�t�H�[�����X�V����
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
	// �O���[�o�����C�e�B���O�p�X�ł͐[�x�e�X�g���֎~
	glDisable(GL_DEPTH_TEST);

	// G�o�b�t�@�V�F�[�_�[���A�N�e�B�u�ɂ���
	mGGlobalShader->SetActive();

	// �X�v���C�g���_�V�F�[�_�[���A�N�e�B�u�ɂ���
	mSpriteVerts->SetActive();

	// �T���v�����O����G�o�b�t�@�e�N�X�`����ݒ�
	mGBuffer->SetTexturesActive();

	// ���C�e�B���O��uniform��ݒ�
	SetLightUniforms(mGGlobalShader, mView);

	// ��`�̂��߂̎O�p�`��`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// G�o�b�t�@�̐[�x�o�b�t�@�̓��e��
	// �f�t�H���g�̃t���[���o�b�t�@�ɃR�s�[
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// �[�x�e�X�g��L���ɂ��邪�A
	// �[�x�o�b�t�@�ւ̏������݂͖����ɂ���
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// �_�����V�F�[�_�[�ƃ��b�V�����A�N�e�B�u�ɂ���
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();

	// �r���[�ˉe�s���ݒ�
	mGPointLightShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// �f�o�b�t�@�̃e�N�X�`�����T���v�����O�p�ɐݒ�
	mGBuffer->SetTexturesActive();

	// �_�����̐F�͊����̐F�ɉ��Z����
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// �_������`�悷��
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mGPointLightShader, mPointLightMesh);
	}
}

bool Renderer::LoadShaders()
{
	/* -------- �X�v���C�g�V�F�[�_�[���쐬���� -------- */
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();

	// �r���[�ˉe�s���ݒ肷��
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);


	/* -------- ���b�V���V�F�[�_�[���쐬���� -------- */
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();

	// ���_�V�F�[�_�[�Ƀr���[�ˉe�s���ݒ肷��
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(FIELD_OF_VIEW),
		mScreenWidth, mScreenHeight, NEAR_PLANE, FAR_PLANE);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);


	/* -------- �X�L���̃V�F�[�_�[���쐬���� -------- */
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		SDL_Log("Failed to load SkinnedShader.");
		return false;
	}

	mSkinnedShader->SetActive();

	// �r���[�ˉe�s���ݒ肷��
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(FIELD_OF_VIEW),
		mScreenWidth, mScreenHeight, NEAR_PLANE, FAR_PLANE);
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);


	/* -------- �O���[�o�����C�e�B���O�V�F�[�_�[���쐬���� -------- */
	mGGlobalShader = new Shader();

	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag"))
	{
		return false;
	}

	// GBuffer�̃T���v���[�ɃC���f�b�N�X�����蓖�Ă�
	mGGlobalShader->SetActive();
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);

	// �r���[�ˉe�̓X�v���C�g�Ɠ���
	mGGlobalShader->SetMatrixUniform("uViewProj", spriteViewProj);

	// ���[���h�ϊ��őS��ʂւ̃X�P�[�����O�� y �̔��]���s��
	Matrix4 gbufferWorld = Matrix4::CreateScale(mScreenWidth, -mScreenHeight, 1.0f);

	mGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);


	/* -------- �_�����V�F�[�_�[���쐬���� -------- */
	mGPointLightShader = new Shader();

	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag"))
	{
		return false;
	}
	
	// �T���v���[�ɃC���f�b�N�X�����蓖�Ă�
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
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // ����
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // �E��
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // �E��
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // �E��
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::Layout::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader, const Matrix4& view)
{
	// �J�����̈ʒu�̓r���[�𔽓]���ċ��߂�
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// ����
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	// ���s����
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// screenPoint���f�o�C�X���W�i-1����+1�̊ԁj�ɕϊ�
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;

	// ��ˉe�s��ɂ��x�N�g���̕ϊ�
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

Vector2 Renderer::Convert3DtoScreenPos(const Vector3& WorldPos)
{
	// 3D���W�𓯎����W�ɂ���
	float posX = WorldPos.x, posY = WorldPos.y, posZ = WorldPos.z, posW = 1;

	// �r���[�s����|����
	posX = posX * mView.mat[0][0] + posY * mView.mat[1][0] + posZ * mView.mat[2][0] + posW * mView.mat[3][0];
	posY = posX * mView.mat[0][1] + posY * mView.mat[1][1] + posZ * mView.mat[2][1] + posW * mView.mat[3][1];
	posZ = posX * mView.mat[0][2] + posY * mView.mat[1][2] + posZ * mView.mat[2][2] + posW * mView.mat[3][2];
	posW = posX * mView.mat[0][3] + posY * mView.mat[1][3] + posZ * mView.mat[2][3] + posW * mView.mat[3][3];

	// �ˉe�s����|����
	posX = posX * mProjection.mat[0][0] + posY * mProjection.mat[1][0] + posZ * mProjection.mat[2][0] + posW * mProjection.mat[3][0];
	posY = posX * mProjection.mat[0][1] + posY * mProjection.mat[1][1] + posZ * mProjection.mat[2][1] + posW * mProjection.mat[3][1];
	posZ = posX * mProjection.mat[0][2] + posY * mProjection.mat[1][2] + posZ * mProjection.mat[2][2] + posW * mProjection.mat[3][2];
	posW = posX * mProjection.mat[0][3] + posY * mProjection.mat[1][3] + posZ * mProjection.mat[2][3] + posW * mProjection.mat[3][3];

	// �r���[�|�[�g�s����|����
	posX = posX * mViewPort.mat[0][0] + posY * mViewPort.mat[1][0] + posZ * mViewPort.mat[2][0] + posW * mViewPort.mat[3][0];
	posY = posX * mViewPort.mat[0][1] + posY * mViewPort.mat[1][1] + posZ * mViewPort.mat[2][1] + posW * mViewPort.mat[3][1];
	posZ = posX * mViewPort.mat[0][2] + posY * mViewPort.mat[1][2] + posZ * mViewPort.mat[2][2] + posW * mViewPort.mat[3][2];
	posW = posX * mViewPort.mat[0][3] + posY * mViewPort.mat[1][3] + posZ * mViewPort.mat[2][3] + posW * mViewPort.mat[3][3];

	return Vector2(posX / posW - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - posY / posW);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// �n�_���ߐڕ��ʂł̉�ʂ̒��S�ɂ���
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);

	// �I�_���ߐڕ��ʂƉ������ʂ̊Ԃ̉�ʂ̒��S�ɒ�߂�
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);

	// �����x�N�g�����߂�
	outDir = end - outStart;
	outDir.Normalize();
}