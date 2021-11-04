#include "VectorAndMatrix.h"

mat4x4 mat4x4::PointAt(Vec3& pos, Vec3 target, Vec3& up)
{
	// Calculate new forward direction
	Vec3 newForward = target - pos;
	newForward.Normalize();

	// Calculate new Up direction
	Vec3 a = newForward * Vec3::Dot(up, newForward);
	Vec3 newUp = up - a;
	newUp.Normalize();

	// New Right direction is easy, its just cross product
	Vec3 newRight = Vec3::CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}