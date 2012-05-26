/*
 * Renderer.h
 *
 *  Created on: May 21, 2012
 *      Author: Alexey Stolyar
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "spt.h"

using namespace cv;
using namespace std;



class Renderer {
public:
	Renderer();
	virtual ~Renderer();

	void iterate(Mat& output, Camera &cam, Scene &scene);
	void trace(Ray& ray,  Scene &scene, int n, Vector3d& color);

};


#endif /* RENDERER_H_ */
