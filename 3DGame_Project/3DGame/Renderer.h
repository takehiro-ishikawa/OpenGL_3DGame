#pragma once
#include <vector>
#include <unordered_map>
#include "SDL/SDL.h"
#include "Math.h"

#pragma region �v���g�^�C�v�錾
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

// ���s�����p
struct DirectionalLight
{
	// ���̕���
	Vector3 mDirection;

	// �g�U���ːF
	Vector3 mDiffuseColor;

	// ���ʔ��ːF
	Vector3 mSpecColor;
};

// �S�Ă̕`�揈�����Ǘ�
class Renderer
{
public:
	Renderer(Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	// �`�悷��
	// �ŏ��Ƀ��b�V���V�F�[�_�[�őS�Ă�3D���b�V���R���|�[�l���g��`�悵�A
	// ���ɃX�v���C�g�V�F�[�_�[�őS�ẴX�v���C�g�R���|�[�l���g��`�悷��
	void Draw();

	// SpriteComponent�̒ǉ��ƍ폜
	void AddSprite(SpriteComponent* sprite);
	void RemoveSprite(SpriteComponent* sprite);

	// MeshComponent�̒ǉ��ƍ폜
	void AddMeshComp(MeshComponent* mesh);
	void RemoveMeshComp(MeshComponent* mesh);

	// PointLightComponent�̒ǉ��ƍ폜
	void AddPointLight(PointLightComponent* light);
	void RemovePointLight(PointLightComponent* light);

	Texture* GetTexture(const std::string& fileName); // �e�N�X�`���̎擾
	Mesh* GetMesh(const std::string& fileName);       // ���b�V���̎擾

	// �r���[�s���ݒ肷��
	void SetViewMatrix(const Matrix4& view) { mView = view; } 

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; } // ������ݒ肷��
	DirectionalLight& GetDirectionalLight() { return mDirLight; }             // ���s�������擾����

	// �X�N���[����ԍ��W���w�肷��ƁA
	// ���݂�3D�r���[/�ˉe�s��Ɋ�Â��āA��������[���h��Ԃɋt�ˉe����
	// ���҂����͈�:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1]  0�̓J�����ɋ߂��A1�͉����ɂ���
	Vector3 Unproject(const Vector3& screenPoint) const;

	// ��ʃx�N�g���̊J�n�_�ƕ������擾����
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

	// �Q�[����ʂ̕����擾
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
	
	void SetPointLightMesh(Mesh* mesh) { mPointLightMesh = mesh; }

	Vector2 GetCursorPosition() { return mCursorPosition; }
	void SetCursorPosition(Vector2 position) { mCursorPosition = position; }

	InstancedData* GetInstancedData(const std::string& fileName);

	// 3D���W���X�N���[�����W�ɕϊ�����
	Vector2 Convert3DtoScreenPos(const Vector3& WorldPos);

private:

	// �����_�����O�����̃w���p�[�֐�
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj,
		float viewPortScale = 1.0f, bool lit = true);

	void DrawFromGBuffer();

	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(Shader* shader, const Matrix4& view);

	std::unordered_map<std::string, Texture*> mTextures; // ���[�h���ꂽ�e�N�X�`���̃}�b�v
	std::unordered_map<std::string, Mesh*> mMeshes;      // ���[�h���ꂽ���b�V���̃}�b�v

	// �`�悷��S�ẴX�v���C�g�R���|�[�l���g
	std::vector<SpriteComponent*> mSprites;

	// �`�悷��S�Ẵ��b�V���R���|�[�l���g
	std::vector<MeshComponent*> mMeshComps;
	std::vector<SkeletalMeshComponent*> mSkeletalMeshes;

	// �`�悷��S�ẴC���X�^���V���O���b�V���f�[�^
	std::unordered_map<std::string, InstancedData*> mInstancedData;

	// Game
	Game* mGame;

	// �X�v���C�g�̒��_�z��
	VertexArray* mSpriteVerts; 

	// �g�p����V�F�[�_�[
	Shader* mSpriteShader;        // �摜�p
	Shader* mMeshShader;          // ���b�V���p
	Shader* mSkinnedShader;       // �X�L���p
	Shader* mInstancedMeshShader; // �C���X�^���V���O�`�悷�郁�b�V���p

	// 3D�V�F�[�_�[�p�s��
	Matrix4 mView;       // �r���[�s��
	Matrix4 mProjection; // �ˉe�s��
	Matrix4 mViewPort;   // �r���[�|�[�g�s��

	// �Ɩ��f�[�^
	Vector3 mAmbientLight;      // ����
	DirectionalLight mDirLight; // ���s����

	// ��ʂ̕�/����
	float mScreenWidth;
	float mScreenHeight;

	// Window
	SDL_Window* mWindow;
	// OpenGL �R���e�N�X�g
	SDL_GLContext mContext;

	// G�o�b�t�@
	GBuffer* mGBuffer;
	Shader* mGGlobalShader;

	// �_����
	Shader* mGPointLightShader;
	std::vector<PointLightComponent*> mPointLights;
	Mesh* mPointLightMesh;

	// �J�[�\��
	Vector2 mCursorPosition;
};

class GBuffer
{
public:

	// G�o�b�t�@�Ɋi�[�����f�[�^�̎��
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	GBuffer();
	~GBuffer();

	// G�o�b�t�@�̐����Ɣj��
	bool Create(int width, int height);
	void Destroy();

	// �w�肷��^�C�v�̃e�N�X�`�����擾
	Texture* GetTexture(Type type);

	// �t���[���o�b�t�@�I�u�W�F�N�g�� ID ���擾
	unsigned int GetBufferID() { return mBufferID; }

	// G�o�b�t�@�̊e�e�N�X�`����Ή�����e�N�X�`���C���f�b�N�X�Ƀo�C���h����
	void SetTexturesActive();

private:

	// G�o�b�t�@�Ɋ��蓖�Ă�ꂽ�e�N�X�`��
	std::vector<Texture*> mTextures;

	// �t���[���o�b�t�@�I�u�W�F�N�g��ID
	unsigned int mBufferID;
};