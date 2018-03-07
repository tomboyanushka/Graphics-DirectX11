#include "Camera.h"
#include "Windows.h"




Camera::Camera(float width, float height)
{
	pitch = 0.0f;
	yaw = 0.0f;
	this->pos = XMFLOAT3(0, 0, -5);
	this->dir = XMFLOAT3(0, 0, 1);
	this->up = XMFLOAT3(0,1,0);
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
	XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P));
}


Camera::~Camera()
{

}

XMFLOAT4X4 Camera::View()
{

	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(pitch, yaw, 0.0f);
	XMVECTOR pos = XMVectorSet(this->pos.x, this->pos.y, this->pos.z, 0);
	XMVECTOR dir = XMVectorSet(this->dir.x, this->dir.y, this->dir.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	dir = XMVector3Rotate(dir, rotate);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     
		dir,    
		up);    
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	return viewMatrix;
}

XMFLOAT4X4 Camera::Proj()
{
	return projMatrix;
}

void Camera::cameraUpdate(float dt)
{
	auto posV = XMLoadFloat3(&pos);
	auto dirV = XMLoadFloat3(&dir);

	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(pitch, yaw, 0.0f);
	dirV = XMVector3Rotate(dirV, rotate);
	auto upV = XMLoadFloat3(&up);
	if (GetAsyncKeyState('W') & 0x8000)
		posV = posV + dirV*1.f*dt;

	if (GetAsyncKeyState('S') & 0x8000)
		posV = (posV - dirV * 1.f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		posV = posV + (XMVector3Cross(dirV, upV))*dt;

	if (GetAsyncKeyState('D') & 0x8000)
		posV = posV - (XMVector3Cross(dirV, upV)) * dt;

	if (GetAsyncKeyState('X') & 0x8000)
		posV = posV + XMVectorSet(0.0f,-5.0f*dt,0.0f,0.0f);
		

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		posV = posV + XMVectorSet(0.0f, 5.0f*dt, 0.0f, 0.0f);
		
	XMStoreFloat3(&pos, posV);

}

void Camera::OnResize(float width, float height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P));
}

void Camera::RotateByPitch(float p)
{
	pitch = pitch + p;
}

void Camera::RotateByYaw(float y)
{
	yaw = yaw + y;
}




