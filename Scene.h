/*
 * Scene.h
 *
 *  Created on: May 26, 2012
 *      Author: alexey
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "core.h"
#include "Shape.h"
#include "Material.h"

using namespace std;

class Camera {
public:
	Camera(Vector3d _position, Vector3d _tl, Vector3d _tr, Vector3d _bl) :
			position(_position), tl(_tl), tr(_tr), bl(_bl) {
		xDirection = tr - tl;
		yDirection = bl - tl;
	}

	Ray getRay(double x, double y) {
		Vector3d point = tl + (xDirection * x + yDirection * y) - position;
		point = point.norm();
		return Ray(position, point);
	}

	Vector3d position;
	Vector3d tl, tr, bl;
	Vector3d xDirection, yDirection;
};

template<class ShapeType>
class Body {
public:
	Body(ShapeType _shape, Material *_material) :
			shape(_shape), material(_material) {
	}
	ShapeType shape; // TODO: interface it.
	Material *material;

};

class Scene {
public:
	vector<Body<Sphere> > spheres;
	vector<Body<Plane> > planes;
	vector<Body<Cube> > cubes;

	vector<Body<Triangle> > triangles;
	vector<Body<Cube> > grid;
	vector<vector<Body<Triangle> > > gridTriangles;

	void indexTriangles() {
		Vector3d minVal(10e10, 10e10, 10e10);
		Vector3d maxVal(-10e10, -10e10, -10e10);
		for (size_t i = 0, lim = triangles.size(); i < lim; ++i) {
			minVal = min(minVal, triangles[i].shape.v1);
			minVal = min(minVal, triangles[i].shape.v2);
			minVal = min(minVal, triangles[i].shape.v3);

			maxVal = max(maxVal, triangles[i].shape.v1);
			maxVal = max(maxVal, triangles[i].shape.v2);
			maxVal = max(maxVal, triangles[i].shape.v3);
		}

		Vector3d boundingSize = maxVal - minVal;
		Vector3d cubeSize = boundingSize / 1;
		int counter = 0;

		for (int x = 0; x < 1; ++x) {
			for (int y = 0; y < 1; ++y) {
				for (int z = 0; z < 1; ++z) {
					Cube cube(
							(minVal
									+ cubeSize
											* Vector3d((double) x, (double) y,
													(double) z)),
							(minVal
									+ cubeSize
											* Vector3d(x + 1.0, y + 1.0,
													z + 1.0)));
					int flag = false;
					for (size_t i = 0, lim = triangles.size(); i < lim; ++i) {
						if (cube.isPointInside(triangles[i].shape.v1)
								|| cube.isPointInside(triangles[i].shape.v2)
								|| cube.isPointInside(triangles[i].shape.v3)) {
							if (!flag) {

								grid.push_back(
										Body<Cube>(cube,
												new Glass(
														Vector3d(0.8, 0.8, 0.8),
														1.0, 0)));
//												new Material(
//														Vector3d(0.8, 0.8, 0.8),
//														Vector3d(0, 0, 0))));
								counter++;
								gridTriangles.push_back(
										vector<Body<Triangle> >());
								flag = true;
							}
							gridTriangles[counter - 1].push_back(triangles[i]);

						}

					}
				}
			}
		}
	}

	Scene();
	virtual ~Scene();
};

#endif /* SCENE_H_ */
