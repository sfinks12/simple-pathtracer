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



	Scene();
	virtual ~Scene();
};

#endif /* SCENE_H_ */
