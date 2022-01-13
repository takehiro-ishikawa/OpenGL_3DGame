#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "Math.h"
#include <fbxsdk.h>
#include "FBXData.h"
#include <iostream>

Mesh::Mesh()
	:mBox(Vector3::Infinity, Vector3::NegInfinity)
	, mVertexArray(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(FBXData* fbxFile, Renderer* renderer)
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
	VertexArray::Layout layout;
	if (fbxFile->GetIsSkeletal())
	{
		// �ʒu�A�@���A�{�[���̃C���f�b�N�X�ԍ��A�E�F�C�g�AUV���W
		vertSize = 10;
		layout = VertexArray::Layout::PosNormSkinTex;
		vertices.reserve(fbxFile->GetNumVertexCount() * (sizeof(float) * 8 + sizeof(uint8_t) * 8));
	}
	else
	{
		// �ʒu�A�@���AUV���W
		vertSize = 8;
		layout = VertexArray::Layout::PosNormTex;
		vertices.reserve(fbxFile->GetNumVertexCount() * sizeof(float) * 8);
	}
	
	std::vector<Vector3> positions = fbxFile->GetVertices();            // �ʒu���W
	std::vector<Vector3> normals = fbxFile->GetNormals();               // �@��
	std::vector<Vector2> uvs = fbxFile->GetUVs();                       // UV���W
	std::vector<VertexBoneInfo> VBInfo = fbxFile->GetVertexBoneInfos(); // �{�[���̔ԍ��A�E�F�C�g

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

		// �{�[���������b�V���̏ꍇ
		if (fbxFile->GetIsSkeletal())
		{
			// �e����^����{�[���̃C���f�b�N�X�ԍ�
			v.b[0] = static_cast<uint8_t>(VBInfo[i].BoneIndex[0]);
			v.b[1] = static_cast<uint8_t>(VBInfo[i].BoneIndex[1]);
			v.b[2] = static_cast<uint8_t>(VBInfo[i].BoneIndex[2]);
			v.b[3] = static_cast<uint8_t>(VBInfo[i].BoneIndex[3]);
			vertices.emplace_back(v);

			// �E�F�C�g
			v.b[0] = static_cast<uint8_t>(VBInfo[i].BoneWeight[0]);
			v.b[1] = static_cast<uint8_t>(VBInfo[i].BoneWeight[1]);
			v.b[2] = static_cast<uint8_t>(VBInfo[i].BoneWeight[2]);
			v.b[3] = static_cast<uint8_t>(VBInfo[i].BoneWeight[3]);
			vertices.emplace_back(v);
		}

		// ���E���̔��a��AABB�̐ݒ�
		mRadius = Math::Max(mRadius, positions[i].LengthSq());
		mBox.UpdateMinMax(positions[i]);
	}
	// ----------------------------------------------------------------------------

	// ���_�z����쐬����
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, fbxFile->GetIndices().data(), static_cast<unsigned>(fbxFile->GetNumIndexCount()));

	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}