#pragma once


#include <DirectXMath.h>
using namespace DirectX;

// --------------------------------------------------------
// A custom light definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct DirectionalLight
{
	XMFLOAT4 AmbientColor;	    
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};