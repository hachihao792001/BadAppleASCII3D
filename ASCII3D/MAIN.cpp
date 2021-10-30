#include "Rasterize.h"

mesh meshCube;
mat4x4 matProj;	// Matrix that converts from view space to screen space
Vec3 vCamera;	// Location of camera in world space
Vec3 vLookDir;	// Direction vector along the direction camera points
float fYaw;		// FPS Camera rotation in XZ plane
float fTheta;	// Spins World transform
float* pDepthBuffer = nullptr;

int texTick = 0;
int texsCount = 2191;
Texture* currentTex;
Texture** allTexs;

void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
	int x2, int y2, float u2, float v2, float w2,
	int x3, int y3, float u3, float v3, float w3,
	Texture* tex)
{
	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
		std::swap(w1, w2);
	}

	if (y3 < y1)
	{
		std::swap(y1, y3);
		std::swap(x1, x3);
		std::swap(u1, u3);
		std::swap(v1, v3);
		std::swap(w1, w3);
	}

	if (y3 < y2)
	{
		std::swap(y2, y3);
		std::swap(x2, x3);
		std::swap(u2, u3);
		std::swap(v2, v3);
		std::swap(w2, w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step = 0, dw2_step = 0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = (int)(x1 + (float)(i - y1) * dax_step);
			int bx = (int)(x1 + (float)(i - y1) * dbx_step);

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float texCoords = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - texCoords) * tex_su + texCoords * tex_eu;
				tex_v = (1.0f - texCoords) * tex_sv + texCoords * tex_ev;
				tex_w = (1.0f - texCoords) * tex_sw + texCoords * tex_ew;
				if (tex_w > pDepthBuffer[i * WIDTH + j])
				{
					Dot(Vec2(j, i), tex->GetPixelNormalize(tex_u / tex_w, tex_v / tex_w));
					pDepthBuffer[i * WIDTH + j] = tex_w;
				}
				texCoords += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int bx = (int)(x1 + (float)(i - y1) * dbx_step);
			int ax = (int)(x2 + (float)(i - y2) * dax_step);

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float texCoords = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - texCoords) * tex_su + texCoords * tex_eu;
				tex_v = (1.0f - texCoords) * tex_sv + texCoords * tex_ev;
				tex_w = (1.0f - texCoords) * tex_sw + texCoords * tex_ew;

				if (tex_w > pDepthBuffer[i * WIDTH + j])
				{
					Dot(Vec2(j, i), tex->GetPixelNormalize(tex_u / tex_w, tex_v / tex_w));
					pDepthBuffer[i * WIDTH + j] = tex_w;
				}
				texCoords += tstep;
			}
		}
	}
}

bool OnUserCreate()
{
	pDepthBuffer = new float[WIDTH * HEIGHT];

	// Load object file
	//meshCube.LoadFromObjectFile("mountains.obj");

	meshCube.createCube({ 0.0f,0.0f,0.0f }, { 3.0f, 3.0f, 3.0f });

	allTexs = new Texture * [texsCount];
	for (int i = 1; i <= texsCount; i++) {
		allTexs[i - 1] = new Texture("frames\\badApple (" + intToString(i) + ").jpg");
		if (i % 50 == 0)
			cout << "Loaded " << i << " frames" << endl;
	}

	// Projection Matrix
	matProj = mat4x4::Projection(90.0f, (float)HEIGHT / (float)WIDTH, 0.1f, 1000.0f);
	return true;
}

std::vector<Triangle> projectedTriangles;

bool OnUserUpdate(float fElapsedTime)
{

	if (GetKeyState(VK_UP) < 0)
		vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards

	if (GetKeyState(VK_DOWN) < 0)
		vCamera.y -= 8.0f * fElapsedTime;	// Travel Downwards

	// Dont use these two in FPS mode, it is confusing :P
	if (GetKeyState(VK_LEFT) < 0)
		vCamera.x -= 8.0f * fElapsedTime;	// Travel Along X-Axis

	if (GetKeyState(VK_RIGHT) < 0)
		vCamera.x += 8.0f * fElapsedTime;	// Travel Along X-Axis

	Vec3 vForward = vLookDir * (8.0f * fElapsedTime);

	// Standard FPS Control scheme, but turn instead of strafe
	if (GetKeyState(L'W') < 0)
		vCamera = vCamera + vForward;

	if (GetKeyState(L'S') < 0)
		vCamera = vCamera - vForward;

	if (GetKeyState(L'A') < 0)
		fYaw -= 2.0f * fElapsedTime;

	if (GetKeyState(L'D') < 0)
		fYaw += 2.0f * fElapsedTime;

	// Set up rotation matrices
	mat4x4 matRotZ = mat4x4::RotationZ(fTheta * 0.5f),
		matRotX = mat4x4::RotationX(fTheta);

	fTheta += 0.5f * fElapsedTime;

	mat4x4 matTrans = mat4x4::Translation(0.0f, 0.0f, 25.0f);

	mat4x4 matWorld = mat4x4::Identity();	// Form World Matrix
	matWorld = matRotZ * matRotX; // Transform by rotation
	matWorld = matWorld * matTrans; // Transform by translation

	// Create "Point At" Matrix for camera
	Vec3 vUp = { 0,1,0 };
	Vec3 vTarget = { 0,0,1 };
	mat4x4 matCameraRot = mat4x4::RotationY(fYaw);
	vLookDir = vTarget * matCameraRot;
	vTarget = vCamera + vLookDir;
	mat4x4 matCamera = mat4x4::PointAt(vCamera, vTarget, vUp);

	// Make view matrix from camera
	mat4x4 matView = matCamera.QuickInverse();

	// Store triagles for rastering later
	projectedTriangles.clear();

	// Draw Triangles
	for (auto tri : meshCube.tris)
	{
		Triangle triProjected, triTransformed, triViewed;

		// World Matrix Transform
		tri.MultiplyMatrix(matWorld);
		triTransformed.Copy(tri);

		// Calculate triangle Normal
		Vec3 normal, line1, line2;

		// Get lines either side of triangle
		line1 = triTransformed.vertices[1] - triTransformed.vertices[0];
		line2 = triTransformed.vertices[2] - triTransformed.vertices[0];

		// Take cross product of lines to get normal to triangle surface
		normal = Vec3::CrossProduct(line1, line2);

		// You normally need to normalise a normal!
		normal.Normalize();

		// Get Ray from triangle to camera
		Vec3 vCameraRay = triTransformed.vertices[0] - vCamera;

		// If ray is aligned with normal, then triangle is visible
		if (Vec3::Dot(normal, vCameraRay) < 0.0f)
		{
			// Illumination
			Vec3 light_direction = { 0.0f, 1.0f, -1.0f };
			light_direction.Normalize();

			// How "aligned" are light direction and triangle surface normal?
			float dp = max(0.1f, Vec3::Dot(light_direction, normal));

			// Choose console colours as required (much easier with RGB)
			triTransformed.color = RGB((int)(dp * 255), (int)(dp * 255), (int)(dp * 255));

			// Convert World Space --> View Space
			triTransformed.MultiplyMatrix(matView);
			triViewed.Copy(triTransformed);

			// Clip Viewed Triangle against near plane, this could form two additional
			// additional triangles. 
			int nClippedTriangles = 0;
			Triangle clipped[2];
			nClippedTriangles = triViewed.ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, clipped[0], clipped[1]);

			// We may end up with multiple triangles form the clip, so project as
			// required
			for (int n = 0; n < nClippedTriangles; n++)
			{
				// Project triangles from 3D --> 2D
				clipped[n].MultiplyMatrix(matProj);
				triProjected.Copy(clipped[n]);
				triProjected.Normalize();

				// X/Y are inverted so put them back
				triProjected.Scale(-1, -1);

				// Offset verts into visible normalised space
				Vec3 vOffsetView = { 1,1,0 };
				triProjected.Translate(vOffsetView);
				triProjected.Scale(0.5f * (float)WIDTH, 0.5f * (float)HEIGHT);

				// Store triangle for sorting
				projectedTriangles.push_back(triProjected);
			}
		}
	}

	// Sort triangles from back to front
	/*sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](Triangle& t1, Triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});*/

	return true;
}

void UpdateThread() {
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (true) {
		if ((int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastTime).count() >= (1.0 / FPS) * 1000) {
			OnUserUpdate(0.04f);
			lastTime = std::chrono::high_resolution_clock::now();
		}
	}
}

void DrawThread() {
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (true) {
		if ((int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastTime).count() >= (1.0 / FPS) * 1000) {

			// Clear Depth Buffer
			for (int i = 0; i < WIDTH * HEIGHT; i++)
				pDepthBuffer[i] = 0.0f;

			std::vector<Triangle> trianglesToRaster;

			// Loop through all transformed, viewed, projected, and sorted triangles
			for (auto& projectedTri : projectedTriangles)
			{
				// Clip triangles against all four screen edges, this could yield
				// a bunch of triangles, so create a queue that we traverse to 
				//  ensure we only test new triangles generated against planes
				Triangle clipped[2];
				std::list<Triangle> clippedTriangle;

				// Add initial triangle
				clippedTriangle.push_back(projectedTri);
				int nNewTriangles = 1;

				for (int vertices = 0; vertices < 4; vertices++)
				{
					int nTrisToAdd = 0;
					while (nNewTriangles > 0)
					{
						// Take triangle from front of queue
						Triangle test = clippedTriangle.front();
						clippedTriangle.pop_front();
						nNewTriangles--;

						// Clip it against a plane. We only need to test each 
						// subsequent plane, against subsequent new triangles
						// as all triangles after a plane clip are guaranteed
						// to lie on the inside of the plane. I like how this
						// comment is almost completely and utterly justified
						switch (vertices)
						{
						case 0:	nTrisToAdd = test.ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, clipped[0], clipped[1]); break;
						case 1:	nTrisToAdd = test.ClipAgainstPlane({ 0.0f, (float)HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, clipped[0], clipped[1]); break;
						case 2:	nTrisToAdd = test.ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, clipped[0], clipped[1]); break;
						case 3:	nTrisToAdd = test.ClipAgainstPlane({ (float)WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, clipped[0], clipped[1]); break;
						}

						// Clipping may yield a variable number of triangles, so
						// add these new ones to the back of the queue for subsequent
						// clipping against next planes
						for (int w = 0; w < nTrisToAdd; w++)
							clippedTriangle.push_back(clipped[w]);
					}
					nNewTriangles = clippedTriangle.size();
				}

				for (auto& clipped : clippedTriangle)
					trianglesToRaster.push_back(clipped);
			}

			currentTex = allTexs[texTick++];
			if (texTick >= texsCount)
				texTick = 0;
			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& tri : trianglesToRaster)
			{
				TexturedTriangle(
					(int)tri.vertices[0].x, (int)tri.vertices[0].y, (int)tri.texCoords[0].u, (int)tri.texCoords[0].v, (int)tri.texCoords[0].w,
					(int)tri.vertices[1].x, (int)tri.vertices[1].y, (int)tri.texCoords[1].u, (int)tri.texCoords[1].v, (int)tri.texCoords[1].w,
					(int)tri.vertices[2].x, (int)tri.vertices[2].y, (int)tri.texCoords[2].u, (int)tri.texCoords[2].v, (int)tri.texCoords[2].w, currentTex);

				//FillTriangle(tri, tri.color);
				DrawTriangle(tri);
			}

			DrawCall();

			lastTime = std::chrono::high_resolution_clock::now();
		}
	}

}

int main()
{
	Init();
	OnUserCreate();
	std::thread update(UpdateThread);
	std::thread draw(DrawThread);
	update.join();
	draw.join();

	return 0;
}
