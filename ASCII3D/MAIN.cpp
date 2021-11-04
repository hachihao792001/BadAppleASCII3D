#include "Rasterize.h"
#include "Texture.h"
#include "Camera.h"

Cube meshCube;
mat4x4 matProj;	// Matrix that converts from view space to screen space

int texTick = 0;
int texsCount = 2191;
Texture* currentTex;
Texture** allTexs;

Camera camera;

bool OnUserCreate()
{
	//pDepthBuffer = new float[WIDTH * HEIGHT];

	meshCube = Cube({ 0.0f,0.0f,10.0f }, { 3.0f, 3.0f, 3.0f });
	meshCube.Translate({ 0.0f,0.0f, 10.0f });

	/*allTexs = new Texture * [texsCount];
	for (int i = 1; i <= texsCount; i++) {
		allTexs[i - 1] = new Texture("frames\\badApple (" + intToString(i) + ").jpg");
		if (i % 50 == 0)
			cout << "Loaded " << i << " frames" << endl;
	}*/

	camera = Camera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, 0.1f, 0.7f);

	// Projection Matrix
	matProj = mat4x4::Projection(60.0f, (float)HEIGHT / (float)WIDTH, 0.1f, 1000.0f);
	return true;
}

std::vector<Triangle> projectedTriangles;

bool OnUserUpdate(float fElapsedTime)
{

	meshCube.Rotate(Vec3(0.1f, 0.0f, 0.05f));

	camera.UpdateInput();

	projectedTriangles.clear();

	// Draw Triangles
	for (auto tri : meshCube.tris)
	{

		Vec3 normal = tri.getNormal();
		Vec3 vCameraRay = tri.vertices[0] - camera.pos;

		// If ray is aligned with normal, then triangle is visible
		if (Vec3::Dot(normal, vCameraRay) < 0.0f)
		{
			// Illumination
			Vec3 light_direction = { 0.0f, 1.0f, -1.0f };
			light_direction.Normalize();
			tri.CalculateIllumination(light_direction);

			tri.MultiplyMatrix(camera.matView);

			// Clip Viewed Triangle against near plane, this could form two additional triangles. 
			vector<Triangle> clippedTris = tri.ClipAgainstPlane({ { 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f } });

			// We may end up with multiple triangles form the clip, so project as required
			for (int i = 0; i < (int)clippedTris.size(); i++)
			{
				// Project triangles from 3D --> 2D
				clippedTris[i].MultiplyMatrix(matProj);
				Triangle triProjected;
				triProjected.Copy(clippedTris[i]);
				triProjected.Normalize();

				// X/Y are inverted so put them back
				triProjected.Scale(-1, -1);

				// Offset verts into visible normalised space
				Vec3 vOffsetView = { 1,1,0 };
				triProjected.Translate(vOffsetView);
				triProjected.Scale(0.5f * (float)WIDTH, 0.5f * (float)HEIGHT);

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

			vector<Triangle> trianglesToRaster;

			// Loop through all transformed, viewed, projected, and sorted triangles
			for (auto& projectedTri : projectedTriangles)
			{
				// Clip triangles against all four screen edges, this could yield
				// a bunch of triangles, so create a queue that we traverse to 
				//  ensure we only test new triangles generated against planes
				list<Triangle> clippedTriangle;

				// Add initial triangle
				clippedTriangle.push_back(projectedTri);
				int nNewTriangles = 1;

				for (int p = 0; p < 4; p++)
				{
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
						vector<Triangle> clipped;
						switch (p)
						{
						case 0:	clipped = test.ClipAgainstPlane(Plane::bottomScreenEdge()); break;
						case 1:	clipped = test.ClipAgainstPlane(Plane::topScreenEdge()); break;
						case 2:	clipped = test.ClipAgainstPlane(Plane::leftScreenEdge()); break;
						case 3:	clipped = test.ClipAgainstPlane(Plane::rightScreenEdge()); break;
						}

						// Clipping may yield a variable number of triangles, so
						// add these new ones to the back of the queue for subsequent
						// clipping against next planes
						for (int w = 0; w < (int)clipped.size(); w++)
							clippedTriangle.push_back(clipped[w]);
					}
					nNewTriangles = clippedTriangle.size();
				}

				for (auto& tri : clippedTriangle)
					trianglesToRaster.push_back(tri);
			}

			/*currentTex = allTexs[texTick++];
			if (texTick >= texsCount)
				texTick = 0;*/
				// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& tri : trianglesToRaster)
			{
				/*TexturedTriangle(
					(int)tri.vertices[0].x, (int)tri.vertices[0].y, (int)tri.texCoords[0].u, (int)tri.texCoords[0].v, (int)tri.texCoords[0].w,
					(int)tri.vertices[1].x, (int)tri.vertices[1].y, (int)tri.texCoords[1].u, (int)tri.texCoords[1].v, (int)tri.texCoords[1].w,
					(int)tri.vertices[2].x, (int)tri.vertices[2].y, (int)tri.texCoords[2].u, (int)tri.texCoords[2].v, (int)tri.texCoords[2].w, currentTex);*/

				FillTriangle(tri, tri.color);
				//DrawTriangle(tri);
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
