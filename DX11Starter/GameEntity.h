#pragma once
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"

using namespace DirectX;

class GameEntity
{
public:
	GameEntity(Mesh * meshObject, Material * mat);
	~GameEntity();
	



	void SetTranslation(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 sca);
	void SetRotation(float rot);

	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	Mesh *GetMesh();

	XMFLOAT4X4 GetMatrix();
	Material* GetMaterial();
	void PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix, ID3D11ShaderResourceView* someSRV, ID3D11SamplerState* sampler);




private:



	Mesh* meshObject;
	Material* newMaterial;

	// Texture stuff
	ID3D11ShaderResourceView* metalSRV1;
	ID3D11ShaderResourceView* brickSRV1;
	ID3D11ShaderResourceView* bambooSRV1;
	ID3D11SamplerState* sampler;
	

	////// Setting shader resources
	//virtual bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) = 0;
	//virtual bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState) = 0;



};

