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

#include <iostream>
using namespace std;

void Renderer::iterate(Mat& output, Camera &cam, Scene &scene) {
	double w = output.cols;
	double h = output.rows;

	static int step = 4;
	static int iter = 1;
	if (!(iter++ % (4 * step))) {
		step = max(step / 2, 1);
	}

	int iy = 0;
	int ix = 0;
	w /= step;
	h /= step;

	Mat small(h, w, CV_64FC3, Scalar(0));

	double ystep = 1.0 / h;
	double yinit = (1.0 * rand() / RAND_MAX) / h;
	int hi = h;

	Mat distmap(h, w, CV_64FC1, Scalar(0));
#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < hi; i++) {
//	for (double y = (1.0 * rand() / RAND_MAX) / h, ystep = 1.0 / h;
//			y < 0.9999999; y += ystep) {
//		cout << "row" << endl;
		double y = yinit + ystep * i;
		double* distPtr = (double*) distmap.ptr<double>(i, 0);
		Vector3d *ptr = (Vector3d*) small.ptr<Vector3d>(i, 0);
		for (double x = (1.0 * rand() / RAND_MAX) / w, xstep = 1.0 / w;
				x < 0.9999999; x += xstep) {
			Vector3d color(0, 0, 0);

			Ray ray = cam.getRay(x, y);
			trace(ray, scene, 0, color, *(distPtr++));
			*(ptr++) += color;
		}
	}
	Mat img;

	normalize(distmap, img, 0, 255, NORM_MINMAX, CV_8UC1);

	imwrite("dist.jpg", img);
//	imshow("small",small);
//	waitKey(0);

	resize(small.clone(), small, Size(), step, step);
	output += small;
}
void Renderer::trace(Ray& ray, Scene &scene, int n, Vector3d& color,
		double &distance) {

	double mind = 9999999;
	register Vector3d colorMask(1.0, 1.0, 1.0);
	register Vector3d accColor(0., 0., 0.);

	register Vector3d point, fwpoint, normal, direction;
	int i = 0;
	double dist = 0;
	for (; i < 8;) {

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

		double mind2 = mind;
		int index = -1;
		for (size_t j = 0, lim = scene.grid.size(); j < lim; ++j) {
			Body<Cube> &obj = scene.grid[j];
			d = obj.shape.intersect(ray);
			if (d > 0 && d <= mind2) {
				mind2 = d;
				fwpoint = ray.source + ray.direction.norm() * (mind2);
				index = j;
			}
		}

//				for (size_t j = 0, lim = scene.grid.size(); j < lim; ++j) {
//					Body<Cube> &obj = scene.grid[j];
//					d = obj.shape.intersect(ray);
//					if (d > 0 && d <= mind) {
//						mind = d;
//						point = ray.source + ray.direction * mind;
//						normal = obj.shape.getNormal(point);
//						material = obj.material;
//					}
//				}

		if (index != -1) {
			int foundFlag = false;
			//mind = 99999999;
			for (size_t j = 0, lim = scene.gridTriangles[index].size(); j < lim;
					++j) {
				Body<Triangle> &obj = scene.gridTriangles[index][j];
				d = obj.shape.intersect(ray);
				if (d > 0 && d <= mind) {
					mind = d;
					point = ray.source + ray.direction * (d);
					normal = obj.shape.getNormal(point);
					material = obj.material;
					foundFlag = true;
				}
			}

			if (!foundFlag && mind >100000) {
				ray.source = fwpoint;
				dist += mind2;
				continue;
			}
			//mind/=10;

		}

		if (i == 0) {
			distance = mind;//dist + mind;
		}

//		for (size_t j = 0, lim = scene.triangles.size(); j < lim; ++j) {
//			Body<Triangle> &obj = scene.triangles[j];
//			d = obj.shape.intersect(ray);
//			if (d > 0 && d <= mind) {
//				mind = d;
//				point = ray.source + ray.direction * mind;
//				normal = obj.shape.getNormal(point);
//				material = obj.material;
//			}
//		}

		if (mind > 100000) {
			break;
		}

		direction = material->bounce(ray, normal);

		ray.source = point;
		ray.direction = direction.norm();

		accColor = accColor + colorMask * (material->emission);
		colorMask = colorMask * (material->color);
		dist += mind;
		if (material->emission.sum() > 1)
			break;
		++i;
	}
	color += accColor;
	return;

}

