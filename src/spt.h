/*
 * spt.h
 *
 *  Created on: May 20, 2012
 *      Author: Alexey Stolyar
 */


#ifndef SPT_H_
#define SPT_H_

#include <iostream>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "core.h"
#include "Material.h"
#include "Shape.h"

using namespace cv;
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


class Body {
public:
	Body(IShape *_shape, Material *_material) :
			shape(_shape), material(_material) {
	}
	IShape *shape; // TODO: interface it.
	Material *material;

};

#endif /* SPT_H_ */
