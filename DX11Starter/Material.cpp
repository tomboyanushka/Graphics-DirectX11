#include "Material.h"



Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shader, ID3D11SamplerState* sample)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}


Material::~Material()
{
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView * Material::GetTexture()
{
	return textureShader;
}

ID3D11SamplerState * Material::GetSampler()
{
	return sample;
}

