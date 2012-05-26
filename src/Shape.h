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

class Triangle {
public:
	Triangle(Vector3d _v1, Vector3d _v2, Vector3d _v3) :
			v1(_v1), v2(_v2), v3(_v3) {
		normal = ((v3 - v2).cross(v1 - v2)).norm();
		invnormal = normal * -1;
		D = normal.dot(v1);
		e1 = v2 - v1;
		e2 = v3 - v1;

	}

	inline double intersect(Ray &ray) {

		Vector3d pvec = ray.direction.cross(e2);

		double det = e1.dot(pvec);

		if (det > -0.00000001 ){//&& det > -0.0000001) {
			return -1;
		}

		double inv_det = 1.0 / det;

		Vector3d tvec = ray.source - v1;

		double u = tvec.dot(pvec)*inv_det;

		if (u < 0 || u > 1)
			return -1;

		Vector3d qvec = tvec.cross(e1);

		double v = ray.direction.dot(qvec)*inv_det;

		if (v < 0 || u + v > 1)
			return -1;

		double t = e2.dot(qvec)*inv_det;

		return t ;
	}

	inline Vector3d getNormal(Vector3d &point) {
		return invnormal;
	}

	Vector3d normal;
	Vector3d invnormal;
	Vector3d v1, v2, v3, e1, e2;
	double D;
};

class Plane {
public:
	Plane(Vector3d _normal, double _D) :
			normal(_normal.norm()), D(_D) {
		Q = normal * D;
		invnormal = normal * -1;
	}

	inline double intersect(Ray &ray) {
		//return (normal.dot(Q - ray.source)) / (ray.source.dot(ray.direction));

		double Vd = invnormal.dot(ray.direction);
		if (Vd > 0) {
			return -(invnormal.dot(ray.source) + D) / Vd;
		}

		return -1;
	}

	inline Vector3d getNormal(Vector3d &point) {
		return normal;
	}

	Vector3d normal;
	Vector3d invnormal;
	Vector3d Q;
	double D;
};

class Sphere {
public:
	Sphere(Vector3d _center, double _radius) :
			center(_center), radius(_radius) {
		radius2 = _radius * _radius;
	}

	inline double intersect(Ray &ray) {
		Vector3d distVect = ray.source - this->center;
		double b = distVect.dot(ray.direction);
		double c = distVect.dot(distVect) - this->radius2;
		double d = b * b - c;
		return d > 0 ? -b - sqrt(d) : -1;
	}

	inline Vector3d getNormal(Vector3d &point) {
		return (point - this->center).norm();
	}

	Vector3d center;
	double radius;
	double radius2;
};

class Cube {
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
