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

	
	void SetViewMatrix(const Matrix4& view) { mView = view; } // �r���[�s���ݒ肷��

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

	// ���[�h���ꂽ�e�N�X�`���̃}�b�v
	std::unordered_map<std::string, Texture*> mTextures;

	// ���[�h���ꂽ���b�V���̃}�b�v
	std::unordered_map<std::string, Mesh*> mMeshes;

	// �`�悷��S�ẴX�v���C�g�R���|�[�l���g
	std::vector<SpriteComponent*> mSprites;

	// �`�悷��S�Ẵ��b�V���R���|�[�l���g
	std::vector<MeshComponent*> mMeshComps;
	std::vector<SkeletalMeshComponent*> mSkeletalMeshes;

	// Game
	Game* mGame;

	// �X�v���C�g�̒��_�z��
	VertexArray* mSpriteVerts; 

	Shader* mSpriteShader;  // �X�v���C�g�̃V�F�[�_�[
	Shader* mMeshShader;    // ���b�V���̃V�F�[�_�[
	Shader* mSkinnedShader; // �X�L���̃V�F�[�_�[

	// �r���[/�ˉe 3D�V�F�[�_�[�p
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
