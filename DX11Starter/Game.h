#pragma once

#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Game : public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();
	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);
	void DrawStuff(Mesh* someMesh);
private:




	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateMatrices();
	void CreateMeshes();




	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Texture stuffs :
	ID3D11ShaderResourceView* metalSRV;
	ID3D11ShaderResourceView* brickSRV;
	ID3D11ShaderResourceView* bambooSRV;
	ID3D11SamplerState* stateSampler;





	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;


	Mesh* sphereMesh;
	Mesh* helixMesh;
	Mesh* cubeMesh;

	GameEntity* sphere;
	GameEntity* helix;
	GameEntity* cube;

	DirectionalLight light;
	DirectionalLight light2;

	Camera* cam;
	Material* material;
	POINT prevMousePos;
	void DrawEntities();

};
