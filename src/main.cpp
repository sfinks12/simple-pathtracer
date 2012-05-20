#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include "spt.h"
#include "Renderer.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	Mat img(768, 1024, CV_32FC3, Scalar(0));

	Camera cam(Point3d(0, -0.5, 0), Point3d(-1.3, 1.0, 1.0),
			Point3d(1.3, 1.0, 1.0), Point3d(-1.3, 1.0, -1.0));

	vector<Body> objs;
	srand(getTickCount());

	for (int i = 0; i < 4; ++i) {
		objs.push_back(
				Body(
						new Sphere(
								Point3d(1.5 - (3.0 * rand() / RAND_MAX),
										1 + (2.0 * rand() / RAND_MAX), 0), 0.5),
						new Chrome(
								Vec3f(0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX))));

		objs.push_back(
				Body(
						new Sphere(
								Point3d(1. - (2.0 * rand() / RAND_MAX),
										0.5 + (1.5 * rand() / RAND_MAX), -0.25),
								0.25),
						new Glass(
								Vec3f(0.8 + 0.2 * rand() / RAND_MAX,
										0.8 + 0.2 * rand() / RAND_MAX,
										0.8 + 0.2 * rand() / RAND_MAX), 1.5,
								0.1)));

		Point3d pos(1.5 - (3.0 * rand() / RAND_MAX),
				1.5 + (2.0 * rand() / RAND_MAX),
				1. + (2.0 * rand() / RAND_MAX));

objs	.push_back(
			Body(new Cube(pos, pos + Point3d(0.5+1.0 * rand() / RAND_MAX, 0.5+1.0 * rand() / RAND_MAX, 0.5+1.0 * rand() / RAND_MAX)),
											new Material(
													Vec3f(0.5 + 0.5 * rand() / RAND_MAX,
															0.5 + 0.5 * rand() / RAND_MAX,
															0.5 + 0.5 * rand() / RAND_MAX),
													Vec3f(0, 0, 0))));

						}

	objs.push_back(
			Body(new Sphere(Point3d(0.0, 3.5, -10e6), 10e6 - 0.5),
					new Material(Vec3f(0.9, 0.9, 0.9), Vec3f(0, 0, 0))));

	objs.push_back(
			Body(new Sphere(Point3d(0.0, 10e6, 0.0), 10e6 - 4.5),
					new Material(Vec3f(0.9, 0.9, 0.9), Vec3f(0, 0, 0))));

	objs.push_back(
			Body(new Sphere(Point3d(-10e6, 3.5, 0.0), 10e6 - 1.9),
					new Material(Vec3f(0.9, 0.5, 0.5), Vec3f(0, 0, 0))));

	objs.push_back(
			Body(new Sphere(Point3d(10e6, 3.5, 0.0), 10e6 - 1.9),
					new Material(Vec3f(0.5, 0.5, 0.9), Vec3f(0, 0, 0))));

	objs.push_back(
			Body(new Sphere(Point3d(0.0, 0.0, 10e6), 10e6 - 2.5),
					new Material(Vec3f(1.0, 1.0, 1.0),
							Vec3f(1.29, 1.47, 1.6))));

	objs.push_back(
			Body(new Sphere(Point3d(0.0, -10e6, 0.0), 10e6 - 2.5),
					new Material(Vec3f(0.5, 0.9, 0.5), Vec3f(0, 0, 0))));

	Renderer renderer;
	for (int i = 0; i < 10000; ++i) {

		double t = getTickCount();
		renderer.iterate(img, cam, objs);
		cout << "Render time: "
				<< ((getTickCount() - t) / getTickFrequency()) * 1000.0 << endl;

		//objs[0].shape.center.x = sin(i/20.0);
		//objs[0].shape.center.y = 2+ cos(i/20.0);
		Mat out;
		//resize(img/(i+1),out, Size(),2,2 );
		out = img / (i + 1);
		imshow("out", out);
		imwrite("out.jpg", (img / (i + 1)) * 255);
		char c = waitKey(1);
		if (c == 27) {
			break;
		}
	}
	return 0;
}
