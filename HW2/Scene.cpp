#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "Scene.h"

using namespace tinyxml2;
using namespace std;

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true;
		}
		else
		{
			this->cullingEnabled = false;
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec4 *vertex = new Vec4();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);
		vertex->t = 1;

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		this->vertices.push_back(vertex);
		this->colorsOfVertices.push_back(color);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		Translation *translation = new Translation();

		translationElement->QueryIntAttribute("id", &translation->translationId);

		str = translationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translation->findTranslationMatrix();
		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		Scaling *scaling = new Scaling();

		scalingElement->QueryIntAttribute("id", &scaling->scalingId);
		str = scalingElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scaling->findScalingMatrix();
		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		Rotation *rotation = new Rotation();

		rotationElement->QueryIntAttribute("id", &rotation->rotationId);
		str = rotationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->u.x, &rotation->u.y, &rotation->u.z);
		rotation->findRotationMatrix();
		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = meshElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = WIREFRAME_MESH;
		}
		else
		{
			mesh->type = SOLID_MESH;
		}

		// read mesh transformations
		XMLElement *meshTransformationsElement = meshElement->FirstChildElement("Transformations");
		XMLElement *meshTransformationElement = meshTransformationsElement->FirstChildElement("Transformation");

		while (meshTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = meshTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			meshTransformationElement = meshTransformationElement->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *cloneStr;
		int v1, v2, v3;
		XMLElement *meshFacesElement = meshElement->FirstChildElement("Faces");
		str = meshFacesElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{

				mesh->triangles.push_back(Triangle(*vertices[v1 - 1], *vertices[v2 - 1], *vertices[v3 - 1], v1, v2, v3, mesh->type == SOLID_MESH));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}
}

void Scene::assignColorToPixel(int i, int j, Color c)
{
	this->image[i][j].r = c.r;
	this->image[i][j].g = c.g;
	this->image[i][j].b = c.b;
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;
			vector<double> rowOfDepths;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
				rowOfDepths.push_back(1.01);
			}

			this->image.push_back(rowOfColors);
			this->depth.push_back(rowOfDepths);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				assignColorToPixel(i, j, this->backgroundColor);
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFilename.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFilename << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
*/
void Scene::convertPPMToPNG(string ppmFileName)
{
	string command;

	// TODO: Change implementation if necessary.
	command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
	system(command.c_str());
}

/*
	Transformations, clipping, culling, rasterization are done here.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	vector<Triangle *> transformedTriangles;
	for (int i = 0; i < meshes.size(); i++)
	{
		/*
		 * Modeling transformations.
		 */
		transformMesh(meshes[i], transformedTriangles);
	}
	/*
	 * Camera, projection transformations.
	 */
	Matrix4 cameraTransformationMatrix = calculateCameraTransformationMatrix(camera);

	for (int i = 0; i < transformedTriangles.size(); i++)
	{
		transformedTriangles[i]->vertices[0] = cameraTransformationMatrix * transformedTriangles[i]->vertices[0];
		transformedTriangles[i]->vertices[1] = cameraTransformationMatrix * transformedTriangles[i]->vertices[1];
		transformedTriangles[i]->vertices[2] = cameraTransformationMatrix * transformedTriangles[i]->vertices[2];
	}

	/*
	 * Normalizing vertices.
	 */

	for (int i = 0; i < transformedTriangles.size(); i++)
	{
		transformedTriangles[i]->calculateNormal();
		transformedTriangles[i]->vertices[0].toHomogenous();
		transformedTriangles[i]->vertices[1].toHomogenous();
		transformedTriangles[i]->vertices[2].toHomogenous();
	}

	/*
	 * Culling (not backface culling :) )
	 */

	cullTriangles(transformedTriangles);

	if (this->cullingEnabled)
	{
		backfaceCullTriangles(transformedTriangles, camera);
	}

	/*
	 * Clipping
	 */
	Matrix4 viewPortMatrix = viewingMatrix(camera);

	for (int i = transformedTriangles.size() - 1; i >= 0; i--)
	{
		pair<Vec4, Color> v1 = make_pair(transformedTriangles[i]->vertices[0], *colorsOfVertices[transformedTriangles[i]->vertices[0].colorId - 1]);
		pair<Vec4, Color> v1_ = make_pair(transformedTriangles[i]->vertices[0], *colorsOfVertices[transformedTriangles[i]->vertices[0].colorId - 1]); // clipping will change v1 and apperantly we need to v1s for two different triangles.
		pair<Vec4, Color> v2 = make_pair(transformedTriangles[i]->vertices[1], *colorsOfVertices[transformedTriangles[i]->vertices[1].colorId - 1]);
		pair<Vec4, Color> v3 = make_pair(transformedTriangles[i]->vertices[2], *colorsOfVertices[transformedTriangles[i]->vertices[2].colorId - 1]);

		bool clip1 = clipEdge(v1, v2);
		bool clip2 = clipEdge(v2, v3);
		bool clip3 = clipEdge(v3, v1_);

		if (transformedTriangles[i]->isSolid)
		{
			if (v1.first != v1_.first)
			{
				drawTriangle(v1, v2, v3, viewPortMatrix);
			}
			drawTriangle(v1_, v2, v3, viewPortMatrix);
		}
		else
		{
			if (clip1)
			{
				rasterize(v1, v2, viewPortMatrix);
			}
			if (clip2)
			{
				rasterize(v2, v3, viewPortMatrix);
			}
			if (clip3)
			{
				rasterize(v3, v1_, viewPortMatrix);
			}
		}
	}

	// for (int i = 0; i < transformedTriangles.size(); i++)
	// {
	// 	cout << *transformedTriangles[i] << endl;
	// }
	cout << "-------------------------------------" << endl;
}

Vec4 Scene::calculateNormalOfTriangle(int v1, int v2, int v3)
{
	return (*this->vertices[v2 - 1] - *this->vertices[v1 - 1]) * (*this->vertices[v3 - 1] - *this->vertices[v1 - 1]);
}

void Scene::transformMesh(Mesh *mesh, vector<Triangle *> &triangles)
{
	for (int i = mesh->triangles.size() - 1; i >= 0; i--)
	{
		Triangle *newTriangle = new Triangle(mesh->triangles[i]);
		for (int j = 0; j < mesh->transformationIds.size(); j++)
		{
			Transformation *transformation;
			if (mesh->transformationTypes[j] == 'r')
			{
				transformation = this->rotations[mesh->transformationIds[j] - 1];
			}
			else if (mesh->transformationTypes[j] == 's')
			{
				transformation = this->scalings[mesh->transformationIds[j] - 1];
			}
			else if (mesh->transformationTypes[j] == 't')
			{
				transformation = this->translations[mesh->transformationIds[j] - 1];
			}
			newTriangle->vertices[0] = transformation->transform(newTriangle->vertices[0]);
			newTriangle->vertices[1] = transformation->transform(newTriangle->vertices[1]);
			newTriangle->vertices[2] = transformation->transform(newTriangle->vertices[2]);
		}
		newTriangle->calculateNormal();
		triangles.push_back(newTriangle);
	}
}

Matrix4 Scene::calculateCameraTransformationMatrix(Camera *camera)
{
	Vec3 u = camera->gaze * camera->v;
	Vec3 v = u * camera->gaze;
	Vec3 w = -camera->gaze;
	u = u.unit();
	v = v.unit();
	w = w.unit();
	Translation t = Translation(0, -camera->position.x, -camera->position.y, -camera->position.z);
	Transformation r;
	double homogenous[4][4] = {{u.x, u.y, u.z, 0},
							   {v.x, v.y, v.z, 0},
							   {w.x, w.y, w.z, 0},
							   {0, 0, 0, 1}};
	r.homogenous = Matrix4(homogenous);
	Matrix4 mcam = r.homogenous * t.homogenous;

	double morthArr[4][4] = {
		{2.0 / (camera->right - camera->left), 0, 0, -((double)(camera->right + camera->left) / (double)(camera->right - camera->left))},
		{0, 2.0 / (camera->top - camera->bottom), -((double)(camera->top + camera->bottom) / (double)(camera->top - camera->bottom))},
		{0, 0, -2.0 / (camera->far - camera->near), -((double)(camera->far + camera->near) / (double)(camera->far - camera->near))},
		{0, 0, 0, 1}};
	Matrix4 morth(morthArr);
	Matrix4 mper;

	if (camera->projectionType == PERSPECTIVE_PROJECTION)
	{
		double mp2oArr[4][4] = {
			{camera->near, 0, 0, 0},
			{0, camera->near, 0, 0},
			{0, 0, camera->near + camera->far, camera->near * camera->far},
			{0, 0, -1, 0}};
		Matrix4 mp2o(mp2oArr);
		mper = morth * mp2o;
	}
	else
	{
		mper = morth;
	}

	return mper * mcam;
}

Matrix4 Scene::viewingMatrix(Camera *camera)
{

	double vp[4][4] = {
		{(double)camera->horRes / 2.0, 0, 0, ((double)camera->horRes - 1.0) / 2.0},
		{0, (double)camera->verRes / 2.0, 0, ((double)camera->verRes - 1.0) / 2.0},
		{0, 0, 0.5, 0.5},
		{0, 0, 0, 0}};
	return Matrix4(vp);
}

void Scene::backfaceCullTriangles(vector<Triangle *> &triangles, Camera *camera)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		Vec3 normal = triangles[i]->normal;
		Vec3 cameraToTriangle = Vec3(triangles[i]->vertices[0].x - camera->position.x, triangles[i]->vertices[0].y - camera->position.y, triangles[i]->vertices[0].z - camera->position.z);
		if (dotProductVec3(normal, cameraToTriangle) > 0)
		{
			triangles.erase(triangles.begin() + i);
			i--;
		}
	}
}

void cullTriangles(vector<Triangle *> &triangles)
{
	for (int i = triangles.size() - 1; i >= 0; i--)
	{
		if (triangles[i]->vertices[0].x > 1 && triangles[i]->vertices[1].x > 1 && triangles[i]->vertices[2].x > 1 || triangles[i]->vertices[0].x < -1 && triangles[i]->vertices[1].x < -1 && triangles[i]->vertices[2].x < -1 || triangles[i]->vertices[0].y > 1 && triangles[i]->vertices[1].y > 1 && triangles[i]->vertices[2].y > 1 || triangles[i]->vertices[0].y < -1 && triangles[i]->vertices[1].y < -1 && triangles[i]->vertices[2].y < -1 ||
			triangles[i]->vertices[0].z > 1 && triangles[i]->vertices[1].z > 1 && triangles[i]->vertices[2].z > 1)
		{
			triangles.erase(triangles.begin() + i);
		}
	}
}

bool visible(double den, double num, double &te, double &tl)
{
	if (den > 0)
	{
		double t = num / den;
		if (t > tl)
		{
			return false;
		}
		else if (t > te)
		{
			te = t;
		}
	}
	else if (den < 0)
	{
		double t = num / den;
		if (t < te)
		{
			return false;
		}
		else if (t < tl)
		{
			tl = t;
		}
	}
	else if (num > 0)
	{
		return false;
	}
	return true;
}

bool Scene::clipEdge(pair<Vec4, Color> &v1, pair<Vec4, Color> &v2)
{
	double te = 0;
	double tl = 1;
	double dx = v2.first.x - v1.first.x;
	double dy = v2.first.y - v1.first.y;
	double dz = v2.first.z - v1.first.z;
	double num, den;
	if (dx == 0 && (v1.first.x < -1 || v1.first.x > 1))
	{
		return false;
	}
	if (dy == 0 && (v1.first.y < -1 || v1.first.y > 1))
	{
		return false;
	}
	if (dz == 0 && (v1.first.z < -1 || v1.first.z > 1))
	{
		return false;
	}

	if (!visible(dx, -1 - v1.first.x, te, tl))
	{
		return false;
	}

	if (!visible(-dx, v1.first.x - 1, te, tl))
	{
		return false;
	}

	if (!visible(dy, -1 - v1.first.y, te, tl))
	{
		return false;
	}

	if (!visible(-dy, v1.first.y - 1, te, tl))
	{
		return false;
	}

	if (!visible(dz, -1 - v1.first.z, te, tl))
	{
		return false;
	}

	if (!visible(-dz, v1.first.z - 1, te, tl))
	{
		return false;
	}
	if (tl < 1)
	{
		v2.first.x = v1.first.x + tl * dx;
		v2.first.y = v1.first.y + tl * dy;
		v2.first.z = v1.first.z + tl * dz;
		v2.second.r = v1.second.r + tl * (v2.second.r - v1.second.r);
		v2.second.g = v1.second.g + tl * (v2.second.g - v1.second.g);
		v2.second.b = v1.second.b + tl * (v2.second.b - v1.second.b);
	}
	if (te > 0)
	{
		v1.first.x = v1.first.x + te * dx;
		v1.first.y = v1.first.y + te * dy;
		v1.first.z = v1.first.z + te * dz;
		v1.second.r = v1.second.r + te * (v2.second.r - v1.second.r);
		v1.second.g = v1.second.g + te * (v2.second.g - v1.second.g);
		v1.second.b = v1.second.b + te * (v2.second.b - v1.second.b);
	}
	return true;
}

void Scene::rasterize(pair<Vec4, Color> &v1, pair<Vec4, Color> &v2, Matrix4 &viewPortMatrix)
{
	Vec4 v1_ = viewPortMatrix * v1.first;
	Vec4 v2_ = viewPortMatrix * v2.first;
	Color v1c = v1.second;
	Color v2c = v2.second;
	int x0 = v1_.x;
	int y0 = v1_.y;
	int x1 = v2_.x;
	int y1 = v2_.y;
	double z0 = v1_.z;
	double z1 = v2_.z;
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0,z1);
		std::swap(v1c, v2c);
	}
	int dx = x1 - x0;
	int dy = abs(y1 - y0);
	int error = dx / 2;
	int ystep = (y0 < y1) ? 1 : -1;
	int y = y0;
	Color c = v1c;
	Color dc;
	int steps = x1 - x0;
	dc.r = (v2c.r - v1c.r) / steps;
	dc.g = (v2c.g - v1c.g) / steps;
	dc.b = (v2c.b - v1c.b) / steps;
	for (int x = x0; x <= x1; x++)
	{
		if (x < 0 || x >= this->image.size() || y < 0 || y >= this->image[0].size())
		{
			continue;
		}
		double depth = z0 + (z1- z0) * (double)(x - x0) / (double)(x1 - x0);
		if (steep)
		{
			if (depth > this->depth[y][x])
			{
				continue;
			}
			assignColorToPixel(y, x, Color(makeBetweenZeroAnd255(c.r), makeBetweenZeroAnd255(c.g), makeBetweenZeroAnd255(c.b)));
			this->depth[y][x] = depth;
		}
		else
		{
			if (depth > this->depth[x][y])
			{
				continue;
			}
			assignColorToPixel(x, y, Color(makeBetweenZeroAnd255(c.r), makeBetweenZeroAnd255(c.g), makeBetweenZeroAnd255(c.b)));
			this->depth[x][y] = depth;
		}
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
		c.r += dc.r;
		c.g += dc.g;
		c.b += dc.b;
	}
}

void Scene::drawTriangle(pair<Vec4, Color> &v1, pair<Vec4, Color> &v2, pair<Vec4, Color> &v3, Matrix4 &viewPortMatrix)
{
	Vec4 v0_ = viewPortMatrix * v1.first;
	Vec4 v1_ = viewPortMatrix * v2.first;
	Vec4 v2_ = viewPortMatrix * v3.first;
	Color v1c = v1.second;
	Color v2c = v2.second;
	Color v3c = v3.second;
	int ymin = min(v0_.y, min(v1_.y, v2_.y)) + 0.5;
	int ymax = max(v0_.y, max(v1_.y, v2_.y)) + 0.5;
	int xmin = min(v0_.x, min(v1_.x, v2_.x)) + 0.5;
	int xmax = max(v0_.x, max(v1_.x, v2_.x)) + 0.5;
	double alphaDivider = v0_.x * (v1_.y - v2_.y) + v0_.y * (v2_.x - v1_.x) + v1_.x * v2_.y - v2_.x * v1_.y;
	double betaDivider = v1_.x * (v2_.y - v0_.y) + v1_.y * (v0_.x - v2_.x) + v2_.x * v0_.y - v0_.x * v2_.y;
	double gamaDivider = v2_.x * (v0_.y - v1_.y) + v2_.y * (v1_.x - v0_.x) + v0_.x * v1_.y - v1_.x * v0_.y;
	for (int y = ymin; y <= ymax; y++)
	{
		if (y < 0 || y >= this->image[0].size())
		{
			continue;
		}
		for (int x = xmin; x <= xmax; x++)
		{
			if (x < 0 || x >= this->image.size())
			{
				continue;
			}
			double f12 = x * (v1_.y - v2_.y) + y * (v2_.x - v1_.x) + v1_.x * v2_.y - v2_.x * v1_.y;
			double f20 = x * (v2_.y - v0_.y) + y * (v0_.x - v2_.x) + v2_.x * v0_.y - v0_.x * v2_.y;
			double f01 = x * (v0_.y - v1_.y) + y * (v1_.x - v0_.x) + v0_.x * v1_.y - v1_.x * v0_.y;
			double alpha = f12 / alphaDivider;
			double beta = f20 / betaDivider;
			double gama = f01 / gamaDivider;
			double depthOfPixel = alpha * v0_.z + beta * v1_.z + gama * v2_.z;
			if (depthOfPixel > 1)
			{
				cout << "WTF" << endl;
			}
			if (alpha >= 0 && beta >= 0 && gama >= 0)
			{
				if (depthOfPixel > this->depth[x][y])
				{
					continue;
				}
				Color c;
				c.r = makeBetweenZeroAnd255(alpha * v1c.r + beta * v2c.r + gama * v3c.r);
				c.g = makeBetweenZeroAnd255(alpha * v1c.g + beta * v2c.g + gama * v3c.g);
				c.b = makeBetweenZeroAnd255(alpha * v1c.b + beta * v2c.b + gama * v3c.b);
				this->depth[x][y] = depthOfPixel;
				assignColorToPixel(x, y, c);
			}
		}
	}
}