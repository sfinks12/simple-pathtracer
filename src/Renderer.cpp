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

void Renderer::iterate(Mat& output, Camera &cam, vector<Body> &objects) {
	double w = output.cols;
	double h = output.rows;

	Vec3f *ptr = (Vec3f*) output.ptr(0);
	for (double y = (1.0 * rand() / RAND_MAX) / h, ystep = 1.0 / h;
			y < 0.9999999; y += ystep) {
		for (double x = (1.0 * rand() / RAND_MAX) / w, xstep = 1.0 / w;
				x < 0.9999999; x += xstep) {
			Ray ray = cam.getRay(x, y);
			Vec3f color;
			trace(ray, objects, 0, *ptr++);
		}
	}
}
void Renderer::trace(Ray& ray, vector<Body> &objects, int n, Vec3f& color) {

	double mind = 9999999;
	register Vec3f colorMask(1.0, 1.0, 1.0);
	register Vec3f accColor(0., 0., 0.);

	register Point3d point, normal, direction;
	int i = 0;
	for (; i < 64; ++i) {

		Body* hit = NULL;
		mind = 99999999;
		double d = 0;
		for (size_t i = 0, lim = objects.size(); i < lim; ++i) {
			Body &obj = objects[i];
			d = obj.shape->intersect(ray);
			if (d > 0 && d <= mind) {
				mind = d;
				hit = &obj;
			}
		}
		if (hit == NULL) {
			break;
		}

		point = ray.source + ray.direction * mind;
		normal = hit->shape->getNormal(point);
		direction = hit->material->bounce(ray, normal);

		ray.source = point;
		ray.direction = direction;

		accColor += colorMask.mul(hit->material->emission);
		colorMask = colorMask.mul(hit->material->color);
		if (norm(hit->material->emission) > 1)
			break;
	}
	color += accColor;
	return;

}

