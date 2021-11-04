#pragma once
#include "VectorAndMatrix.h"

class Camera
{
public:
	Vec3 pos;	// Location of camera in world space
	Vec3 vLookDir;	// Direction vector along the direction camera points
	float fYaw;
	float sensitivity;
	float speed;

	Vec3 up = { 0,1,0 };

	mat4x4 matView;

	Camera() {
		pos = Vec3(0.0f);
		vLookDir = Vec3(0.0f);
		fYaw = 0;
		sensitivity = 0;
		speed = 0;
	}

	Camera(Vec3 p, Vec3 lookDir, float cameraSensitivity, float cameraSpeed) {
		pos = p;
		vLookDir = lookDir;
		fYaw = 0;
		sensitivity = cameraSensitivity;
		speed = cameraSpeed;
	}

	void UpdateInput() {
		if (GetKeyState(VK_UP) < 0)
			pos.y += speed;	// Travel Upwards

		if (GetKeyState(VK_DOWN) < 0)
			pos.y -= speed;	// Travel Downwards

		// Dont use these two in FPS mode, it is confusing :P
		if (GetKeyState(VK_LEFT) < 0)
			pos.x -= speed;	// Travel Along X-Axis

		if (GetKeyState(VK_RIGHT) < 0)
			pos.x += speed;	// Travel Along X-Axis

		Vec3 vForward = vLookDir * speed;

		// Standard FPS Control scheme, but turn instead of strafe
		if (GetKeyState(L'W') < 0)
			pos = pos + vForward;

		if (GetKeyState(L'S') < 0)
			pos = pos - vForward;

		if (GetKeyState(L'A') < 0)
			fYaw -= sensitivity;

		if (GetKeyState(L'D') < 0)
			fYaw += sensitivity;

		Vec3 baseLookDir = { 0, 0, 1 };
		vLookDir = baseLookDir * mat4x4::RotationY(fYaw);
		mat4x4 matCamera = mat4x4::PointAt(pos, pos + vLookDir, up);
		matView = matCamera.QuickInverse();
	}
};

