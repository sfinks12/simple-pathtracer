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

using namespace cv;
using namespace std;


inline Point3d normalize(Point3d &point) {
	return point * (1.0 / (sqrt(point.dot(point))));
}

class Ray {
public:
	Ray(Point3d _source, Point3d _direction) :
			source(_source), direction(_direction) {
	}
	Point3d source;
	Point3d direction;
};

class Camera {
public:
	Camera(Point3d _position, Point3d _tl, Point3d _tr, Point3d _bl) :
			position(_position), tl(_tl), tr(_tr), bl(_bl) {
		xDirection = tr - tl;
		yDirection = bl - tl;
	}

	Ray getRay(double x, double y) {
		Point3d point = tl + (xDirection * x + yDirection * y) - position;
		point = normalize(point);
		return Ray(position, point);
	}

	Point3d position;
	Point3d tl, tr, bl;
	Point3d xDirection, yDirection;
};

class IMaterial {
public:
	virtual Point3d bounce(Ray &ray, Point3d &normal) = 0;
};

class Material: IMaterial {
public:
	Material(Vec3f _color, Vec3f _emission) :
			color(_color), emission(_emission) {
	}
	virtual Point3d bounce(Ray &ray, Point3d &normal) {
		register double u = (1.0 * rand() / RAND_MAX);
		register double v = (1.0 * rand() / RAND_MAX);
		register double r = sqrt(u);
		register double angle = 6.283185307179586 * v;

		Point3d sdir, tdir;

		if (abs(normal.x) < .5) {

			sdir = normal.cross(Point3d(1, 0, 0));
		} else {
			sdir = normal.cross(Point3d(0, 1, 0));
		}
		tdir = normal.cross(sdir);
		return (r * cos(angle) * sdir + r * sin(angle) * tdir
				+ sqrt(1. - u) * normal);
	}

	Vec3f color;
	Vec3f emission;
};

class Chrome: public Material {
public:
	Chrome(Vec3f _color) :
			Material(_color, Vec3f(0, 0, 0)) {
		Material::color = _color;
		Material::emission = Vec3f(0, 0, 0);
	}
	virtual Point3d bounce(Ray &ray, Point3d &normal) {
		double theta1 = abs(ray.direction.dot(normal));
		return ray.direction + normal * (theta1 * 2.0);
	}
};

class Glass: public Material {
public:
	Glass(Vec3f _color, double _ior, double _reflection) :
			Material(_color, Vec3f(0, 0, 0)), ior(_ior), reflection(_reflection) {
		Material::color = _color;
		Material::emission = Vec3f(0, 0, 0);

	}
	virtual Point3d bounce(Ray &ray, Point3d &normal) {
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

class IShape {
public:
	virtual double intersect(Ray &ray) = 0;
	virtual Point3d getNormal(Point3d &point) = 0;
};

class Sphere: public IShape {
public:
	Sphere(Point3d _center, double _radius) :
			center(_center), radius(_radius) {
		radius2 = _radius * _radius;
	}

	inline virtual double intersect(Ray &ray) {
		Point3d distVect = ray.source - this->center;
		double b = distVect.dot(ray.direction);
		double c = distVect.dot(distVect) - this->radius2;
		double d = b * b - c;
		return d > 0 ? -b - sqrt(d) : -1;
	}

	inline virtual Point3d getNormal(Point3d &point) {
		Point3d p = (point - this->center);
		return normalize(p);
	}

	Point3d center;
	double radius;
	double radius2;
};

inline Point3d div(Point3d a, Point3d b) {
	return Point3d(a.x / b.x, a.y / b.y, a.z / b.z);
}
inline Point3d min(Point3d a, Point3d b) {
	return Point3d(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline Point3d max(Point3d a, Point3d b) {
	return Point3d(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

class Cube: public IShape {
public:
	Cube(Point3d _cubeMin, Point3d _cubeMax) :
			cubeMin(_cubeMin), cubeMax(_cubeMax) {
	}

	inline virtual double intersect(Ray &ray) {
		Point3d tMin = div((cubeMin - ray.source), ray.direction);
		Point3d tMax = div((cubeMax - ray.source), ray.direction);
		Point3d t1 = min(tMin, tMax);
		Point3d t2 = max(tMin, tMax);
		double tNear = max(t1.x, max(t1.y, t1.z));
		double tFar = min(t2.x, min(t2.y, t2.z));
		if (tNear > 0 && tNear < tFar) {
			return tNear;
		}
		return -1;
	}

	inline virtual Point3d getNormal(Point3d &point) {
		if (point.x < cubeMin.x + 0.001)
			return Point3d(-1.0, 0.0, 0.0);
		else if (point.x > cubeMax.x - 0.001)
			return Point3d(1.0, 0.0, 0.0);
		else if (point.y < cubeMin.y + 0.001)
			return Point3d(0.0, -1.0, 0.0);
		else if (point.y > cubeMax.y - 0.001)
			return Point3d(0.0, 1.0, 0.0);
		else if (point.z < cubeMin.z + 0.001)
			return Point3d(0.0, 0.0, -1.0);
		else
			return Point3d(0.0, 0.0, 1.0);
	}

	Point3d cubeMin, cubeMax;
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
