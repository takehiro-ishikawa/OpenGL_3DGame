#include "Mesh.h"
#include <GL/glew.h>
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="verts">頂点データのコピー元</param>
/// <param name="numVerts">頂点バッファにある頂点の数</param>
/// <param name="layout">適用する頂点レイアウトの種類</param>
/// <param name="indices">インデックスデータのコピー元</param>
/// <param name="numIndices">インデックスバッファ内のインデックスの数</param>
VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// 頂点配列オブジェクトを作成する
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned int vertexSize = 8 * sizeof(float);
	if (layout == Layout::PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(uint8_t);
	}

	// 頂点バッファを作成する
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	// インデックスバッファを作成する
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// 頂点属性を指定する
	if (layout == Layout::PosNormTex)
	{
		// 位置座標：3個のfloat
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// 法線ベクトル：3個のfloat
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));

		// テクスチャ座標：2個のfloat
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if(layout == Layout::PosNormSkinTex)
	{
		// 位置座標：3個のfloat
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// 法線ベクトル：3個のfloat
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));
		
		// テクスチャ座標：2個のfloat
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));

		// スキニングボーン：整数 0〜255
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE, vertexSize,  reinterpret_cast<void*>(sizeof(float) * 8));
		
		// スキニングの重み：整数 0〜255(0.0から1.0のfloat値に変換)
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