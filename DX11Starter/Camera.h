#pragma once
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{
public:
	Camera(float width, float height);
	~Camera();

	XMFLOAT4X4 View();
	XMFLOAT4X4 Proj();
	void Camera::cameraUpdate(float dt);
	void OnResize(float width, float height);
	void RotateByPitch(float p);
	void RotateByYaw(float y);

private:
	float pitch;
	float yaw;


	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;

	XMFLOAT3 pos;
	XMFLOAT3 dir;
	XMFLOAT3 up;
	XMFLOAT3 look;

};



