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

	static mat4x4 PointAt(Vec3& pos, Vec3& target, Vec3& up);
};

struct Vec3
{
	float x = 0, y = 0, z = 0, w = 1;

	Vec3() {
		x = 0; y = 0; z = 0; w = 1;
	}

	Vec3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
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

	static float Dot(Vec3& v1, Vec3& v2)
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

	Vec3 operator*(mat4x4& m)
	{
		Vec3 v;
		v.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
		v.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
		v.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
		v.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];
		return v;
	}
};

Vec3 Vector_IntersectPlane(Vec3& plane_p, Vec3& plane_n, Vec3& lineStart, Vec3& lineEnd, float& t);


struct Triangle
{
	Vec3 vertices[3];
	Vec2 texCoords[3];
	COLORREF color;

	Triangle() {
		for (int i = 0; i < 3; i++) {
			this->vertices[i] = Vec3(0, 0, 0);
			this->texCoords[i] = Vec2(0, 0);
		}
		this->color = WHITE;
	}

	Triangle(Vec3 v0, Vec3 v1, Vec3 v2) {
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;
		texCoords[0] = Vec2(0, 0);
		texCoords[1] = Vec2(0, 0);
		texCoords[2] = Vec2(0, 0);
		this->color = WHITE;
	}


	Triangle(Vec3 v0, Vec3 v1, Vec3 v2, Vec2 t0, Vec2 t1, Vec2 t2, COLORREF color = WHITE) {
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;
		texCoords[0] = t0;
		texCoords[1] = t1;
		texCoords[2] = t2;
		this->color = color;
	}

	Triangle(Vec3 vertices[3], Vec2 texCoords[3], COLORREF color = WHITE) {
		for (int i = 0; i < 3; i++) {
			this->vertices[i] = vertices[i];
			this->texCoords[i] = texCoords[i];
		}
		this->color = color;
	}

	void Copy(Triangle tri) {
		SetVertices(tri.vertices);
		SetTexCoord(tri.texCoords);
		color = tri.color;
	}

	void SetVertices(Vec3 v[3]) {
		for (int i = 0; i < 3; i++)
			vertices[i] = v[i];
	}
	void SetTexCoord(Vec2 t[3]) {
		for (int i = 0; i < 3; i++)
			texCoords[i] = t[i];
	}

	void Normalize() {
		for (int i = 0; i < 3; i++) {
			vertices[i] = vertices[i] / vertices[i].w;
			texCoords[i].u = texCoords[i].u / vertices[i].w;
			texCoords[i].v = texCoords[i].v / vertices[i].w;
			texCoords[i].w = 1.0f / vertices[i].w;
		}
	}

	void MultiplyMatrix(mat4x4 mat) {
		for (int i = 0; i < 3; i++)
			vertices[i] = vertices[i] * mat;
	}

	void Translate(Vec3 t) {
		for (int i = 0; i < 3; i++)
			vertices[i] = vertices[i] + t;
	}

	void Scale(float x, float y) {
		for (int i = 0; i < 3; i++) {
			vertices[i].x *= x;
			vertices[i].y *= y;
		}
	}

	int ClipAgainstPlane(Vec3 plane_p, Vec3 plane_n, Triangle& out_tri1, Triangle& out_tri2)
	{
		// Make sure plane normal is indeed normal
		plane_n.Normalize();

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](Vec3& vertices)
		{
			Vec3 n = vertices.GetNormalized();
			return (plane_n.x * vertices.x + plane_n.y * vertices.y + plane_n.z * vertices.z - Vec3::Dot(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		Vec3* inside_points[3];  int nInsidePointCount = 0;
		Vec3* outside_points[3]; int nOutsidePointCount = 0;
		Vec2* inside_tex[3]; int nInsideTexCount = 0;
		Vec2* outside_tex[3]; int nOutsideTexCount = 0;


		// Get signed distance of each point in triangle to plane
		float d0 = dist(vertices[0]);
		float d1 = dist(vertices[1]);
		float d2 = dist(vertices[2]);

		if (d0 >= 0) {
			inside_points[nInsidePointCount++] = &vertices[0];
			inside_tex[nInsideTexCount++] = &texCoords[0];
		}
		else {
			outside_points[nOutsidePointCount++] = &vertices[0];
			outside_tex[nOutsideTexCount++] = &texCoords[0];
		}

		if (d1 >= 0) {
			inside_points[nInsidePointCount++] = &vertices[1];
			inside_tex[nInsideTexCount++] = &texCoords[1];
		}
		else {
			outside_points[nOutsidePointCount++] = &vertices[1];
			outside_tex[nOutsideTexCount++] = &texCoords[1];
		}

		if (d2 >= 0) {
			inside_points[nInsidePointCount++] = &vertices[2];
			inside_tex[nInsideTexCount++] = &texCoords[2];
		}
		else {
			outside_points[nOutsidePointCount++] = &vertices[2];
			outside_tex[nOutsideTexCount++] = &texCoords[2];
		}

		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1 = *this;

			return 1; // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.color = color;

			// The inside point is valid, so keep that...
			out_tri1.vertices[0] = *inside_points[0];
			out_tri1.texCoords[0] = *inside_tex[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float t;
			out_tri1.vertices[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.texCoords[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.texCoords[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.texCoords[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri1.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.texCoords[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.texCoords[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.texCoords[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.color = color;

			out_tri2.color = color;

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.vertices[0] = *inside_points[0];
			out_tri1.vertices[1] = *inside_points[1];
			out_tri1.texCoords[0] = *inside_tex[0];
			out_tri1.texCoords[1] = *inside_tex[1];

			float t;
			out_tri1.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.texCoords[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.texCoords[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.texCoords[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.vertices[0] = *inside_points[1];
			out_tri2.texCoords[0] = *inside_tex[1];
			out_tri2.vertices[1] = out_tri1.vertices[2];
			out_tri2.texCoords[1] = out_tri1.texCoords[2];
			out_tri2.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
			out_tri2.texCoords[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
			out_tri2.texCoords[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
			out_tri2.texCoords[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
			return 2; // Return two newly formed triangles which form a quad
		}

		return 0;
	}
};

struct mesh
{
	std::vector<Triangle> tris;

	void createCube(Vec3 center, Vec3 size) {
		Vec3 halfSize = size / 2;
		// SOUTH
		tris.push_back({
			{ center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f },
			{ center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f },
			{ center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f },
			{ 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } });

		tris.push_back({
			{center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f},{ 1.0f, 1.0f, 1.0f} });

		// EAST           																			   
		tris.push_back({
			{ center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f },
			{ center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f },
			{ center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f },
			{ 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } });

		tris.push_back({
			{center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });

		// NORTH     
		tris.push_back({
			{center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f} });

		tris.push_back({
			{center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });

		// WEST      
		tris.push_back({
			{center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f },		{1.0f, 0.0f, 1.0f} });

		tris.push_back({
			{center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f },		{1.0f, 1.0f, 1.0f } });

		// TOP   
		tris.push_back({
			{center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f },		{1.0f, 0.0f, 1.0f} });

		tris.push_back({
			{center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f },		{1.0f, 1.0f, 1.0f } });

		// BOTTOM      
		tris.push_back({
			{center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f },		{1.0f, 0.0f, 1.0f} });

		tris.push_back({
			{center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f},
			{center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f},
			{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f },		{1.0f, 1.0f, 1.0f } });
	}

	bool LoadFromObjectFile(std::string sFilename, bool bHasTexture = false)
	{
		std::ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		std::vector<Vec3> verts;
		std::vector<Vec2> texs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					Vec2 v;
					s >> junk >> junk >> v.u >> v.v;
					// A little hack for the spyro texture
					//v.u = 1.0f - v.u;
					//v.v = 1.0f - v.v;
					texs.push_back(v);
				}
				else
				{
					Vec3 v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}
			}

			if (!bHasTexture)
			{
				if (line[0] == 'f')
				{
					int f[3];
					s >> junk >> f[0] >> f[1] >> f[2];
					tris.push_back(Triangle(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]));
				}
			}
			else
			{
				if (line[0] == 'f')
				{
					s >> junk;

					std::string tokens[6];
					int nTokenCount = -1;


					while (!s.eof())
					{
						char c = s.get();
						if (c == ' ' || c == '/')
							nTokenCount++;
						else
							tokens[nTokenCount].append(1, c);
					}

					tokens[nTokenCount].pop_back();

					tris.push_back({ verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
						texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] });

				}

			}
		}
		return true;
	}
};

class Texture {
public:
	unsigned char* bytes;
	int width, height;
	int channel;

	Texture() {
		bytes = nullptr;
		width = 0;
		height = 0;
		channel = 0;
	}

	Texture(std::string path) {
		bytes = stbi_load(path.c_str(), &width, &height, &channel, 3);
	}

	COLORREF GetPixelNormalize(float xNormalized, float yNormalized) {
		int x = (int)(xNormalized * (width - 1));
		int y = (int)(yNormalized * (height - 1));
		int r = (int)bytes[y * width * channel + x * channel];
		int g = (int)bytes[y * width * channel + x * channel + 1];
		int b = (int)bytes[y * width * channel + x * channel + 2];
		return RGB(r, g, b);
	}

	COLORREF GetPixel(int x, int y) {
		int r = (int)bytes[y * width * channel + x * channel];
		int g = (int)bytes[y * width * channel + x * channel + 1];
		int b = (int)bytes[y * width * channel + x * channel + 2];
		return RGB(r, g, b);
	}

	~Texture() {
		if (bytes != nullptr)
			stbi_image_free(bytes);
	}
};

void DrawTriangle(Triangle tri, COLORREF c = WHITE);
void FillTriangle(Triangle tri, COLORREF fillColor, bool hasBorder = false, COLORREF borderColor = WHITE);
std::vector<Vec2> GetLinePixels(Vec2 a, Vec2 b);

void DrawLine(std::vector<Vec2> pixels, COLORREF c = WHITE);
void Init();
void setCursorPosition(int x, int y);
void DrawCall();
void Dot(Vec2 v, COLORREF c = WHITE);