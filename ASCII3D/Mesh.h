#pragma once
#include "Global.h"

struct Plane {
	Vec3 point;
	Vec3 normal;

	// Return signed shortest distance from point to plane, plane normal must be normalised
	float distToPoint(Vec3 p) {
		return Vec3::Dot(normal, p) - Vec3::Dot(normal, point);
	}

	Vec3 intersectWithLine(Vec3& lineStart, Vec3& lineEnd, float& t) {
		normal.Normalize();
		float plane_d = -Vec3::Dot(normal, point);
		float ad = Vec3::Dot(lineStart, normal);
		float bd = Vec3::Dot(lineEnd, normal);
		t = (-plane_d - ad) / (bd - ad);
		Vec3 lineStartToEnd = lineEnd - lineStart;
		Vec3 lineToIntersect = lineStartToEnd * t;
		return lineStart + lineToIntersect;
	}

	static Plane bottomScreenEdge() {
		return	{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } };
	}
	static Plane topScreenEdge() {
		return	{ { 0.0f, (float)HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f } };
	}
	static Plane leftScreenEdge() {
		return	{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
	}
	static Plane rightScreenEdge() {
		return	{ { (float)WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } };
	}
};


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

	Vec3 getNormal() {
		Vec3 line1 = vertices[1] - vertices[0];
		Vec3 line2 = vertices[2] - vertices[0];
		Vec3 normal = Vec3::CrossProduct(line1, line2);
		normal.Normalize();

		return normal;
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

	void CalculateIllumination(Vec3 lightDir) {
		float brightness = Vec3::Dot(lightDir, getNormal());
		color = RGB((int)(brightness * 255), (int)(brightness * 255), (int)(brightness * 255));
	}

	vector<Triangle> ClipAgainstPlane(Plane plane)
	{
		vector<Triangle> outTris;

		// Make sure plane normal is indeed normal
		plane.normal.Normalize();

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](Vec3& p)
		{
			return Vec3::Dot(plane.normal, p) - Vec3::Dot(plane.normal, plane.point);
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

			//return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through

			outTris.push_back(*this); // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			Triangle out_tri1;
			out_tri1.color = color;

			// The inside point is valid, so keep that...
			out_tri1.vertices[0] = *inside_points[0];
			out_tri1.texCoords[0] = *inside_tex[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float t;
			out_tri1.vertices[1] = plane.intersectWithLine(*inside_points[0], *outside_points[0], t);
			out_tri1.texCoords[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.texCoords[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.texCoords[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri1.vertices[2] = plane.intersectWithLine(*inside_points[0], *outside_points[1], t);
			out_tri1.texCoords[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.texCoords[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.texCoords[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

			outTris.push_back(out_tri1); // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			Triangle out_tri1, out_tri2;
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
			out_tri1.vertices[2] = plane.intersectWithLine(*inside_points[0], *outside_points[0], t);
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
			out_tri2.vertices[2] = plane.intersectWithLine(*inside_points[1], *outside_points[0], t);
			out_tri2.texCoords[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
			out_tri2.texCoords[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
			out_tri2.texCoords[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;

			outTris.push_back(out_tri1);// Return two newly formed triangles which form a quad
			outTris.push_back(out_tri2);
		}

		return outTris;
	}
};

struct Mesh
{
	std::vector<Triangle> tris;

	Vec3 getCenter() {
		Vec3 result(0, 0, 0);
		for (int i = 0; i < (int)tris.size(); i++) {
			for (int v = 0; v < 3; v++) {
				result.x += tris[i].vertices[v].x;
				result.y += tris[i].vertices[v].y;
				result.z += tris[i].vertices[v].z;
			}
		}
		result = result / (float)tris.size() / 3.0f;
		return result;
	}

	void Translate(Vec3 axes) {
		for (int i = 0; i < (int)tris.size(); i++)
			tris[i].MultiplyMatrix(mat4x4::Translation(axes.x, axes.y, axes.z));
	}

	void Rotate(Vec3 axes) {
		if (axes == Vec3(0, 0, 0)) return;

		Vec3 center = getCenter();

		for (int i = 0; i < (int)tris.size(); i++) {
			for (int v = 0; v < 3; v++) {
				Vec3 verticesLocal = tris[i].vertices[v] - center;
				tris[i].vertices[v] = center + (verticesLocal * mat4x4::RotationX(axes.x));
				tris[i].vertices[v] = center + ((tris[i].vertices[v] - center) * mat4x4::RotationY(axes.y));
				tris[i].vertices[v] = center + ((tris[i].vertices[v] - center) * mat4x4::RotationZ(axes.z));
			}
		}
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

class Cube : public Mesh {
public:
	Cube() {

	}

	Cube(Vec3 center, Vec3 size) {
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
};