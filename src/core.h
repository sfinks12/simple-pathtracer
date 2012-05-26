/*
 * core.h
 *
 *  Created on: May 26, 2012
 *      Author: Alexey Stolyar
 */

#ifndef CORE_H_
#define CORE_H_

#include <math.h>
#include <algorithm>

using namespace std;


class Vector3d {
public:
	double x, y, z;

	Vector3d(double _x = 0, double _y = 0, double _z = 0) :
			x(_x), y(_y), z(_z) {
	}
	Vector3d operator+(const Vector3d &b) const {
		return Vector3d(x + b.x, y + b.y, z + b.z);
	}

	Vector3d& operator+=(const Vector3d &b) {
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		return *this;
	}

	Vector3d operator-(const Vector3d &b) const {
		return Vector3d(x - b.x, y - b.y, z - b.z);
	}
	Vector3d operator*(double b) const {
		return Vector3d(x * b, y * b, z * b);
	}

	Vector3d operator*(Vector3d &b) const {
		return Vector3d(x * b.x, y * b.y, z * b.z);
	}

	Vector3d operator/(Vector3d &b) const {
		return Vector3d(x / b.x, y / b.y, z / b.z);
	}

	Vector3d mult(const Vector3d &b) const {
		return Vector3d(x * b.x, y * b.y, z * b.z);
	}

	Vector3d norm() {
		return *this * (1 / sqrt(x * x + y * y + z * z));
	}

	double dot(const Vector3d &b) const {
		return x * b.x + y * b.y + z * b.z;
	} // cross:
//	Vector3d cross(Vector3d &b) {
//		return Vector3d(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
//	}

	Vector3d cross(Vector3d b) {
		return Vector3d(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}

	double sum() {
		return x + y + z;
	}

};

inline Vector3d min(Vector3d a, Vector3d b) {
	return Vector3d(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline Vector3d max(Vector3d a, Vector3d b) {
	return Vector3d(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}


//inline Vector3d normalize(Vector3d &point) {
//	return point * (1.0 / (sqrt(point.dot(point))));
//}

class Ray {
public:
	Ray(Vector3d _source, Vector3d _direction) :
			source(_source), direction(_direction) {
	}
	Vector3d source;
	Vector3d direction;
};

#endif /* CORE_H_ */
