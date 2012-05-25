/*
 * Material.h
 *
 *  Created on: May 26, 2012
 *      Author: Alexey Stolyar
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "core.h"

using namespace cv;
using namespace std;

class IMaterial {
public:
	virtual Vector3d bounce(Ray &ray, Vector3d &normal) = 0;
};

class Material: IMaterial {
public:
	Material(Vector3d _color, Vector3d _emission) :
			color(_color), emission(_emission) {
	}
	virtual Vector3d bounce(Ray &ray, Vector3d &normal) {
		register double u = (1.0 * rand() / RAND_MAX);
		register double v = (1.0 * rand() / RAND_MAX);
		register double r = sqrt(u);
		register double angle = 6.283185307179586 * v;

		Vector3d sdir, tdir;

		if (abs(normal.x) < .5) {

			sdir = normal.cross(Vector3d(1, 0, 0));
		} else {
			sdir = normal.cross(Vector3d(0, 1, 0));
		}
		tdir = normal.cross(sdir);
		return (sdir * r * cos(angle) + tdir * r * sin(angle)
				+ normal * sqrt(1. - u));
	}

	Vector3d color;
	Vector3d emission;
};

class Chrome: public Material {
public:
	Chrome(Vector3d _color) :
			Material(_color, Vector3d(0, 0, 0)) {
		Material::color = _color;
		Material::emission = Vector3d(0, 0, 0);
	}
	virtual Vector3d bounce(Ray &ray, Vector3d &normal) {
		double theta1 = abs(ray.direction.dot(normal));
		return ray.direction + normal * (theta1 * 2.0);
	}
};

class Glass: public Material {
public:
	Glass(Vector3d _color, double _ior, double _reflection) :
			Material(_color, Vector3d(0, 0, 0)), ior(_ior), reflection(_reflection) {
		Material::color = _color;
		Material::emission = Vector3d(0, 0, 0);

	}
	virtual Vector3d bounce(Ray &ray, Vector3d &normal) {
		double theta1 = abs(ray.direction.dot(normal));

		double internalIndex, externalIndex;
		if (theta1 >= 0.0) {
			internalIndex = this->ior;
			externalIndex = 1.0;
		} else {
			internalIndex = 1.0;
			externalIndex = this->ior;
		}
		double eta = externalIndex / internalIndex;
		double theta2 = sqrt(1.0 - (eta * eta) * (1.0 - (theta1 * theta1)));
		double rs = (externalIndex * theta1 - internalIndex * theta2)
				/ (externalIndex * theta1 + internalIndex * theta2);
		double rp = (internalIndex * theta1 - externalIndex * theta2)
				/ (internalIndex * theta1 + externalIndex * theta2);
		double reflectance = (rs * rs + rp * rp);
		// reflection
		if ((1.0 * rand() / RAND_MAX) < reflectance + this->reflection) {
			return ray.direction + normal * (theta1 * 2.0);
		}
		// refraction
		return ((ray.direction + (normal * theta1)) * (eta)
				+ (normal * (-theta2)));

	}
	double ior;
	double reflection;
};

#endif /* MATERIAL_H_ */
