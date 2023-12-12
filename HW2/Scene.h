#ifndef _SCENE_H_
#define _SCENE_H_
#include "Vec3.h"
#include "Vec4.h"
#include "Color.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Camera.h"
#include "Mesh.h"

#include "Vec3.h"

#include <iostream>
#include <vector>

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;

	std::vector<std::vector<Color>> image;
	std::vector<std::vector<double>> depth;
	std::vector<Camera *> cameras;
	std::vector<Vec4 *> vertices;
	std::vector<Color *> colorsOfVertices;
	std::vector<Scaling *> scalings;
	std::vector<Rotation *> rotations;
	std::vector<Translation *> translations;
	std::vector<Mesh *> meshes;


	Scene(const char *xmlPath);

	void assignColorToPixel(int i, int j, Color c);
	void initializeImage(Camera *camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera *camera);
	void convertPPMToPNG(std::string ppmFileName);
	void forwardRenderingPipeline(Camera *camera);
	/******************************************************************/
	Vec4 calculateNormalOfTriangle(int v1, int v2, int v3);
	void transformMesh(Mesh *mesh, std::vector<Triangle *> &triangles);
	Matrix4 calculateCameraTransformationMatrix(Camera *camera);
	Matrix4 viewingMatrix(Camera *camera);
	void backfaceCullTriangles(std::vector<Triangle *> &triangles, Camera *camera);
	void clipTriangle(std::vector<Triangle *> &triangles, int index);
	bool clipEdge(std::pair<Vec4, Color> &v1, std::pair<Vec4, Color> &v2);
	void rasterize(std::pair<Vec4, Color> &v1, std::pair<Vec4, Color> &v2, Matrix4 &viewPortMatrix);
	void drawTriangle(std::pair<Vec4, Color> &v1, std::pair<Vec4, Color> &v2, std::pair<Vec4, Color> &v3, Matrix4 &viewPortMatrix);
};

void cullTriangles(std::vector<Triangle *> &triangles);
bool visible(double den, double num, double &te, double &tl);

#endif
