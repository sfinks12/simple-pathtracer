/*
 * Shape.h
 *
 *  Created on: May 26, 2012
 *      Author: alexey
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <algorithm>

#include "core.h"

using namespace std;

class IShape {
public:
	virtual double intersect(Ray &ray) = 0;
	virtual Vector3d getNormal(Vector3d &point) = 0;
};

#include <iostream>
class Plane: public IShape {
public:
	Plane(Vector3d _normal, double _D) :
			normal(_normal), D(_D) {
		invnormal = normal*-1;
	}

	inline virtual double intersect(Ray &ray) {
		double Vd = invnormal.dot(ray.direction);
		if (Vd > 0) {
			return -(invnormal.dot(ray.source) + D) / Vd;
		}

		return -1;
	}

	inline virtual Vector3d getNormal(Vector3d &point) {
		return normal;
	}

	Vector3d normal;
	Vector3d invnormal;
	double D;
};

class Sphere: public IShape {
public:
	Sphere(Vector3d _center, double _radius) :
			center(_center), radius(_radius) {
		radius2 = _radius * _radius;
	}

	inline virtual double intersect(Ray &ray) {
		Vector3d distVect = ray.source - this->center;
		double b = distVect.dot(ray.direction);
		double c = distVect.dot(distVect) - this->radius2;
		double d = b * b - c;
		return d > 0 ? -b - sqrt(d) : -1;
	}

	inline virtual Vector3d getNormal(Vector3d &point) {
		return (point - this->center).norm();
	}

	Vector3d center;
	double radius;
	double radius2;
};



class Cube: public IShape {
public:
	Cube(Vector3d _cubeMin, Vector3d _cubeMax) :
			cubeMin(_cubeMin), cubeMax(_cubeMax) {
	}

	inline virtual double intersect(Ray &ray) {
		Vector3d tMin = (cubeMin - ray.source) / ray.direction;
		Vector3d tMax = (cubeMax - ray.source) / ray.direction;
		Vector3d t1 = min(tMin, tMax);
		Vector3d t2 = max(tMin, tMax);
		double tNear = max(t1.x, max(t1.y, t1.z));
		double tFar = min(t2.x, min(t2.y, t2.z));
		if (tNear > 0 && tNear < tFar) {
			return tNear;
		}
		return -1;
	}

	inline virtual Vector3d getNormal(Vector3d &point) {
		if (point.x < cubeMin.x + 0.001)
			return Vector3d(-1.0, 0.0, 0.0);
		else if (point.x > cubeMax.x - 0.001)
			return Vector3d(1.0, 0.0, 0.0);
		else if (point.y < cubeMin.y + 0.001)
			return Vector3d(0.0, -1.0, 0.0);
		else if (point.y > cubeMax.y - 0.001)
			return Vector3d(0.0, 1.0, 0.0);
		else if (point.z < cubeMin.z + 0.001)
			return Vector3d(0.0, 0.0, -1.0);
		else
			return Vector3d(0.0, 0.0, 1.0);
	}

	Vector3d cubeMin, cubeMax;
};

#endif /* SHAPE_H_ */
