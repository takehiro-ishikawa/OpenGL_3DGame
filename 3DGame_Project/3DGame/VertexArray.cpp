#include "VertexArray.h"
#include <GL/glew.h>
#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="verts">���_�f�[�^�̃R�s�[��</param>
/// <param name="numVerts">���_�o�b�t�@�ɂ��钸�_�̐�</param>
/// <param name="layout">�K�p���钸�_���C�A�E�g�̎��</param>
/// <param name="indices">�C���f�b�N�X�f�[�^�̃R�s�[��</param>
/// <param name="numIndices">�C���f�b�N�X�o�b�t�@���̃C���f�b�N�X�̐�</param>
VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// ���_�z��I�u�W�F�N�g���쐬����
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 8 * sizeof(float);
	if (layout == Layout::PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(uint8_t);
	}

	// ���_�o�b�t�@���쐬����
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@���쐬����
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// ���_�������w�肷��
	if (layout == Layout::PosNormTex)
	{
		// �ʒu���W�F3��float
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// �@���x�N�g���F3��float
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// �e�N�X�`�����W�F2��float
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if(layout == Layout::PosNormSkinTex)
	{
		// �ʒu���W�F3��float
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// �@���x�N�g���F3��float
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// �e�N�X�`�����W�F2��float
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		
		// �X�L�j���O�{�[���F���� 0�`255
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 8));
		
		// �X�L�j���O�̏d�݁F���� 0�`255(0.0����1.0��float�l�ɕϊ�)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 8 + sizeof(uint8_t) * 4));
	}
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}