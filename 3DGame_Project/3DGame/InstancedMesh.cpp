#include "Mesh.h"
#include "Renderer.h"
#include "FBXData.h"
#include "Texture.h"
#include <GL/glew.h>

InstancedMesh::InstancedMesh()
	:Mesh()
{
	mWorldMatrices.reserve(10000);
}

InstancedMesh::~InstancedMesh(){}

bool InstancedMesh::Load(FBXData* fbxFile, Renderer* renderer)
{
	// ���b�V���̒��_�����v�Z
	fbxFile->ComputeVertexData();

	// �e�N�X�`�����擾
	std::string texName = fbxFile->GetTextureName();
	Texture* t = renderer->GetTexture(texName);

	// �e�N�X�`�������݂��Ȃ����̂������ꍇ
	if (t == nullptr)
	{
		// �f�t�H���g�̃e�N�X�`�����g�p����
		t = renderer->GetTexture("Assets/Textures/Default.png");
	}
	mTextures.emplace_back(t);

	// ���_�z��Ɋ��蓖�Ă�f�[�^�̍쐬
	std::vector<Vertex> vertices;

	// ���_����擾�������ݒ�
	size_t vertSize;
	// �ʒu�A�@���AUV���W
	vertSize = 8;
	vertices.reserve(fbxFile->GetNumVertexCount() * sizeof(float) * 8);

	std::vector<Vector3> positions = fbxFile->GetVertices();            // �ʒu���W
	std::vector<Vector3> normals = fbxFile->GetNormals();               // �@��
	std::vector<Vector2> uvs = fbxFile->GetUVs();                       // UV���W

	for (int i = 0; i < fbxFile->GetNumVertexCount(); i++)
	{
		Vertex v;

		// �ʒu���W
		v.f = static_cast<float>(positions[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(positions[i].y);
		vertices.emplace_back(v);
		v.f = static_cast<float>(positions[i].z);
		vertices.emplace_back(v);

		// �@��
		v.f = static_cast<float>(normals[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(normals[i].y);
		vertices.emplace_back(v);
		v.f = static_cast<float>(normals[i].z);
		vertices.emplace_back(v);

		// UV���W
		v.f = static_cast<float>(uvs[i].x);
		vertices.emplace_back(v);
		v.f = static_cast<float>(uvs[i].y);
		vertices.emplace_back(v);

		// ���E���̔��a��AABB�̐ݒ�
		mRadius = Math::Max(mRadius, positions[i].LengthSq());
		mBox.UpdateMinMax(positions[i]);
	}
	// ----------------------------------------------------------------------------

	// ���_�z����쐬����
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		fbxFile->GetIndices().data(), static_cast<unsigned>(fbxFile->GetNumIndexCount()), mWorldMatrices.data());

	return true;
}

void InstancedMesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;

	mWorldMatrices.clear();
}

void InstancedMesh::AddInstance(const Matrix4& worldMat)
{
	WorldMatrix mat;
	mat.mat0  = worldMat.mat[0][0];
	mat.mat1  = worldMat.mat[0][1];
	mat.mat2  = worldMat.mat[0][2];
	mat.mat3  = worldMat.mat[0][3];
	mat.mat4  = worldMat.mat[1][0];
	mat.mat5  = worldMat.mat[1][1];
	mat.mat6  = worldMat.mat[1][2];
	mat.mat7  = worldMat.mat[1][3];
	mat.mat8  = worldMat.mat[2][0];
	mat.mat9  = worldMat.mat[2][1];
	mat.mat10 = worldMat.mat[2][2];
	mat.mat11 = worldMat.mat[2][3];
	mat.mat12 = worldMat.mat[3][0];
	mat.mat13 = worldMat.mat[3][1];
	mat.mat14 = worldMat.mat[3][2];
	mat.mat15 = worldMat.mat[3][3];

	mWorldMatrices.emplace_back(mat);
}

void InstancedMesh::Draw()
{
	if (mWorldMatrices.size() <= 0) return;

	mTextures[0]->SetActive();

	mVertexArray->SetActive();

	glDrawElementsInstanced(GL_TRIANGLE_STRIP, mVertexArray->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr, mWorldMatrices.size());

	glBindVertexArray(0);
}