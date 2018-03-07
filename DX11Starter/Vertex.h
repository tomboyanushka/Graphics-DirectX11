#pragma once

#include <DirectXMath.h>
using namespace DirectX;

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	XMFLOAT3 Position;	    // The position of the vertex
	//DirectX::XMFLOAT4 Color;        // The color of the vertex
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
};