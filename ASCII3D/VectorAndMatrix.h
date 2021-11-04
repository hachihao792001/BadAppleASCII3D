#pragma once
#include "Global.h"

struct Vec3;

struct Vec2
{
	float u = 0;
	float v = 0;
	float w = 1;

	Vec2() {
		u = 0; v = 0; w = 1;
	}

	Vec2(float _u, float _v) {
		u = _u;
		v = _v;
		w = 1;
	}

	Vec2(int _u, int _v) {
		u = (float)_u;
		v = (float)_v;
		w = 1;
	}

	Vec2(float _u, float _v, float _w) {
		u = _u;
		v = _v;
		w = _w;
	}

	bool operator==(Vec2 const& v) {
		return u == v.u && this->v == v.v && w == v.w;
	}
};

struct mat4x4
{
	float m[4][4] = { 0 };

	mat4x4 operator*(mat4x4 const& m2)
	{
		mat4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m[r][0] * m2.m[0][c] + m[r][1] * m2.m[1][c] + m[r][2] * m2.m[2][c] + m[r][3] * m2.m[3][c];
		return matrix;
	}

	mat4x4 QuickInverse() // Only for Rotation/Translation Matrices
	{
		mat4x4 matrix;
		matrix.m[0][0] = m[0][0]; matrix.m[0][1] = m[1][0]; matrix.m[0][2] = m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m[0][1]; matrix.m[1][1] = m[1][1]; matrix.m[1][2] = m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m[0][2]; matrix.m[2][1] = m[1][2]; matrix.m[2][2] = m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static mat4x4 Identity()
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static mat4x4 RotationX(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[1][2] = sinf(fAngleRad);
		matrix.m[2][1] = -sinf(fAngleRad);
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static mat4x4 RotationY(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][2] = sinf(fAngleRad);
		matrix.m[2][0] = -sinf(fAngleRad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static mat4x4 RotationZ(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][1] = sinf(fAngleRad);
		matrix.m[1][0] = -sinf(fAngleRad);
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static mat4x4 Translation(float x, float y, float z)
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
	}

	static mat4x4 Projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		mat4x4 matrix;
		matrix.m[0][0] = fAspectRatio * fFovRad;
		matrix.m[1][1] = fFovRad;
		matrix.m[2][2] = fFar / (fFar - fNear);
		matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	static mat4x4 PointAt(Vec3& pos, Vec3 target, Vec3& up);
};

struct Vec3
{
	float x = 0, y = 0, z = 0, w = 1;

	Vec3() {
		x = 0; y = 0; z = 0; w = 1;
	}

	Vec3(float f) {
		x = f;
		y = f;
		z = f;
		w = 1;
	}

	Vec3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
		w = 1;
	}

	Vec3(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	operator Vec2() {
		return Vec2{ x, y, w };
	}

	bool operator==(Vec3 v) {
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	Vec3 operator+(Vec3 const& v) {
		return { x + v.x, y + v.y, z + v.z };
	}

	Vec3 operator-(Vec3 const& v2)
	{
		return { x - v2.x, y - v2.y, z - v2.z };
	}

	Vec3 operator*(float k)
	{
		return { x * k, y * k, z * k };
	}

	Vec3 operator/(float k)
	{
		return { x / k, y / k, z / k };
	}

	float Magnitude()
	{
		return sqrtf(Dot(*this, *this));
	}

	Vec3 GetNormalized()
	{
		float l = Magnitude();
		return { x / l, y / l, z / l };
	}

	void Normalize() {
		float l = Magnitude();
		x /= l;
		y /= l;
		z /= l;
	}

	static float Dot(Vec3 v1, Vec3 v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static Vec3 CrossProduct(Vec3& v1, Vec3& v2)
	{
		Vec3 v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	Vec3 operator*(mat4x4 m)
	{
		Vec3 v;
		v.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
		v.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
		v.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
		v.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];
		return v;
	}
};
