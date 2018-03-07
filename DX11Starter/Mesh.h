#pragma once


#include "DXCore.h"
#include "Vertex.h"
#include "SimpleShader.h"

#include <DirectXMath.h>
#include <d3d11.h>

class Mesh
{
public:
	Mesh();
	Mesh(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount, ID3D11Device *device);
	Mesh(const char* file, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();


	//A constructer that creates the buffers from the appropriate arrays
	void CreateBasicGeom(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount, ID3D11Device *device);

private:
	// Buffers to hold actual geometry data
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount;
	int indexCount;
};






