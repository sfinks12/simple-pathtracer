/*
 * Renderer.cpp
 *
 *  Created on: May 21, 2012
 *      Author: Alexey Stolyar
 */

#include "Renderer.h"

Renderer::Renderer() {
	// TODO Auto-generated constructor stub

}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

void Renderer::iterate(Mat& output, Camera &cam, Scene &scene) {
	double w = output.cols;
	double h = output.rows;

	Vector3d *ptr = (Vector3d*) output.ptr(0);
	for (double y = (1.0 * rand() / RAND_MAX) / h, ystep = 1.0 / h;
			y < 0.9999999; y += ystep) {
		for (double x = (1.0 * rand() / RAND_MAX) / w, xstep = 1.0 / w;
				x < 0.9999999; x += xstep) {
			Ray ray = cam.getRay(x, y);
			trace(ray, scene, 0, *(ptr++));

		}
	}
}
void Renderer::trace(Ray& ray, Scene &scene, int n, Vector3d& color) {

	double mind = 9999999;
	register Vector3d colorMask(1.0, 1.0, 1.0);
	register Vector3d accColor(0., 0., 0.);

	register Vector3d point, normal, direction;
	int i = 0;
	for (; i < 64; ++i) {

		Material *material = NULL;
		mind = 99999999;
		double d = 0;
		for (size_t j = 0, lim = scene.planes.size(); j < lim; ++j) {
			Body<Plane> &obj = scene.planes[j];
			d = obj.shape.intersect(ray);
			if (d > 0 && d <= mind) {
				mind = d;
				point = ray.source + ray.direction * mind;
				normal = obj.shape.getNormal(point);
				material = obj.material;
			}
		}

		for (size_t j = 0, lim = scene.spheres.size(); j < lim; ++j) {
			Body<Sphere> &obj = scene.spheres[j];
			d = obj.shape.intersect(ray);
			if (d > 0 && d <= mind) {
				mind = d;
				point = ray.source + ray.direction * mind;
				normal = obj.shape.getNormal(point);
				material = obj.material;
			}
		}

		for (size_t j = 0, lim = scene.cubes.size(); j < lim; ++j) {
			Body<Cube> &obj = scene.cubes[j];
			d = obj.shape.intersect(ray);
			if (d > 0 && d <= mind) {
				mind = d;
				point = ray.source + ray.direction * mind;
				normal = obj.shape.getNormal(point);
				material = obj.material;
			}
		}

		for (size_t j = 0, lim = scene.triangles.size(); j < lim; ++j) {
					Body<Triangle> &obj = scene.triangles[j];
					d = obj.shape.intersect(ray);
					if (d > 0 && d <= mind) {
						mind = d;
						point = ray.source + ray.direction * mind;
						normal = obj.shape.getNormal(point);
						material = obj.material;
					}
				}

		if (mind > 100000) {
			break;
		}

		direction = material->bounce(ray, normal);

		ray.source = point;
		ray.direction = direction;

		accColor = accColor + colorMask * (material->emission);
		colorMask = colorMask * (material->color);

		if (material->emission.sum() > 1)
			break;
	}
	color += accColor;
	return;

}

