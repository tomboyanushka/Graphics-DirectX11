#include "GameEntity.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include "Material.h"
#include "SimpleShader.h"


using namespace DirectX;



GameEntity::GameEntity(Mesh* meshObject, Material* mat)
{
	this->meshObject = meshObject;
	this->newMaterial = mat;
	//Const uint64_t = 0xffffffffffffffff;
	//first we declare the floats
	position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.5f, 0.5f, 0.0f);

	//then we convert them to vectors
	XMVECTOR vPos = XMLoadFloat3(&position);
	XMVECTOR vRot = XMLoadFloat3(&rotation);
	XMVECTOR vScale = XMLoadFloat3(&scale);

	//then we convert them to matrices
	XMMATRIX mPos = XMMatrixTranslationFromVector(vPos);
	XMMATRIX mRot = XMMatrixRotationRollPitchYawFromVector(vRot);
	XMMATRIX mScale = XMMatrixScalingFromVector(vScale);


	XMMATRIX wake = XMMatrixIdentity();

	//calculate the world matrix
	XMMATRIX world = mPos * mRot * mScale;

	//store it
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));

}



void MoveAbsolute(float currentPosition)
{
	

}


GameEntity::~GameEntity()
{
}

void GameEntity::SetTranslation(XMFLOAT3 pos)
{
	position = pos;
}

void GameEntity::SetScale(XMFLOAT3 sca)
{
	scale = sca;
}

void GameEntity::SetRotation(float rot)
{
	rotation.z = rot;
}

Mesh * GameEntity::GetMesh()
{
	return meshObject;
}

XMFLOAT4X4 GameEntity::GetMatrix()
{
	//then we convert them to vectors
	XMVECTOR vPos = XMLoadFloat3(&position);
	XMVECTOR vRot = XMLoadFloat3(&rotation);
	XMVECTOR vScale = XMLoadFloat3(&scale);

	//then we convert them to matrices
	XMMATRIX mPos = XMMatrixTranslationFromVector(vPos);
	XMMATRIX mRot = XMMatrixRotationZ(rotation.z);
	XMMATRIX mScale = XMMatrixScalingFromVector(vScale);


	XMMATRIX wake = XMMatrixIdentity();

	//calculate the world matrix
	XMMATRIX world = mPos * mRot * mScale;

	//store it
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	return worldMatrix;
}

Material * GameEntity::GetMaterial()
{
	return newMaterial;
}

void GameEntity::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix, ID3D11ShaderResourceView* someSRV, ID3D11SamplerState* sampler)
{
	auto vs = newMaterial->GetVertexShader();
	auto ps = newMaterial->GetPixelShader();

	//Basically, newMaterial->GetSRV() and newMaterial->GetSampler() should replace someSRV and sampler and remove the function params ^
	ps->SetShaderResourceView("MetalText", someSRV);
	//ps->SetShaderResourceView("BrickText", brickSRV1);
	//ps->SetShaderResourceView("BambooText", bambooSRV);
	ps->SetSamplerState("Samp", sampler);

	vs->SetMatrix4x4("world", GetMatrix());
	vs->SetMatrix4x4("view", viewMatrix);
	vs->SetMatrix4x4("projection", projMatrix);
	vs->CopyAllBufferData();
	ps->CopyAllBufferData();
	vs->SetShader();
	ps->SetShader();
}
