#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;




// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)

	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	cam = new Camera(width, height);
	vertexShader = 0;
	pixelShader = 0;


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{

	delete vertexShader;
	delete pixelShader;

	delete cam;
	delete material;

	delete sphere;
	delete helix;
	delete cube;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	light = { XMFLOAT4(+0.1f, +0.1f, +0.1f, 1.0f), XMFLOAT4(+0.0f, +0.0f, +1.0f, +0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	light2 = { XMFLOAT4(+0.1f, +0.1f, +0.1f, 1.0f), XMFLOAT4(+1.0f, +0.0f, +0.0f, +0.0f), XMFLOAT3(+1.0f, +0.0f, 0.0f) };
	
	
	LoadShaders();
	CreateMatrices();
	CreateMeshes();

	CreateWICTextureFromFile(
		device,
		context, 
		L"Assets/Textures/metal.jpg",
		0,  
		&metalSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/brick.jpg", 0, &brickSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/bamboo.jpg", 0, &bambooSRV);

	//sampler stuff
	D3D11_SAMPLER_DESC sd = {}; //making it zero
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sd, &stateSampler);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

														 // Create the View matrix
														 // - In an actual game, recreate this matrix every time the camera 
														 //    moves (potentially every frame)
														 // - We're using the LOOK TO function, which takes the position of the
														 //    camera and the direction vector along which to look (as well as "up")
														 // - Another option is the LOOK AT function, to look towards a specific
														 //    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

														// Create the Projection matrix
														// - This should match the window's aspect ratio, and also update anytime
														//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}



void Game::CreateMeshes()
{

	Vertex vertices1[] =
	{


		//for meshA
	{ XMFLOAT3(+1.0f, +2.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+3.0f, +2.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+3.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	};
	Vertex vertices[] =
	{


		//for meshB
	{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+1.5f, -1.5f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+1.0f, -1.5f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },




	};
	Vertex vertices2[] =
	{


		//for meshC
	{ XMFLOAT3(-1.0f, 2.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(+0.0f, +0.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },
	{ XMFLOAT3(-2.0f, 0.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f), },




	};

	UINT indices[] = { 0, 1, 2, 2, 3, 0 };

	material = new Material(vertexShader, pixelShader, metalSRV, stateSampler);

	//material = new Material(vertexShader, pixelShader, brickSRV, sampler);

	//material = new Material(vertexShader, pixelShader, bambooSRV, sampler);

	Mesh* sphereMesh = new Mesh("Assets/Models/sphere.obj", device);
	Mesh* helixMesh = new Mesh("Assets/Models/helix.obj", device);
	Mesh* cubeMesh = new Mesh("Assets/Models/cube.obj", device);


	// Make some entities
	sphere = new GameEntity(sphereMesh, material);
	helix = new GameEntity(helixMesh, material);
	cube = new GameEntity(cubeMesh, material);

	sphere->position = XMFLOAT3(-1, 0, 0);
	sphere->scale = XMFLOAT3(1, 1, 1);

	helix->position = XMFLOAT3(2, 0, 0);
	helix->scale = XMFLOAT3(1, 1, 1);
	//cube->position = XMFLOAT3(4, 2, 0);

	


}

void Game::DrawEntities()
{
	auto vMatrix = cam->View();
	auto pMatrix = cam->Proj();
	
	pixelShader->SetData(
		"light",  // The name of the (eventual) variable in the shader
		&light,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light2",  // The name of the (eventual) variable in the shader
		&light2,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy

	sphere->PrepareMaterial(vMatrix, pMatrix, metalSRV, stateSampler);
	DrawStuff(sphere->GetMesh());

	helix->PrepareMaterial(vMatrix, pMatrix, brickSRV, stateSampler);
	DrawStuff(helix->GetMesh());

	//cube->PrepareMaterial(vMatrix, pMatrix);
	//DrawStuff(cube->GetMesh());

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

}

void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	cam->OnResize(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{

	cam->cameraUpdate(deltaTime);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//sphere->SetRotation((0, 1, 0));
	
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//vertexShader->SetMatrix4x4("world", worldMatrix);
	//vertexShader->SetMatrix4x4("view", viewMatrix);
	//vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	//vertexShader->CopyAllBufferData();

	//// Set the vertex and pixel shaders to use for the next Draw() command
	////  - These don't technically need to be set every frame...YET
	////  - Once you start applying different shaders to different objects,
	////    you'll need to swap the current shaders before each draw

	DrawEntities();



	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001) 
	{ 
		float diffX = x - prevMousePos.x;
		float diffY = y - prevMousePos.y;
		cam->RotateByPitch(0.05f*diffY*XM_PI/180);
		cam->RotateByYaw(0.05f*diffX*XM_PI/180);
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}

void Game::DrawStuff(Mesh* someMesh)
{
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//for Mesh 
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	auto vertexBuffer = someMesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(someMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	context->DrawIndexed(
	someMesh->GetIndexCount(),
		0,
		0);
}


#pragma endregion