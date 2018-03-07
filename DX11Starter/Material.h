#pragma once
#include "SimpleShader.h"

class Material
{
public:
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shader, ID3D11SamplerState* sample);
	~Material();
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	ID3D11ShaderResourceView* GetTexture();
	ID3D11SamplerState* GetSampler();


private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11ShaderResourceView* textureShader;
	ID3D11SamplerState* sample;
	
};

