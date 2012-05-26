#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include "spt.h"
#include "Renderer.h"
#include "ObjReader.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img(480, 640, CV_64FC3, Scalar(0));

	Camera cam(Vector3d(0, -0.5, 0), Vector3d(-1.3, 1.0, 1.0),
			Vector3d(1.3, 1.0, 1.0), Vector3d(-1.3, 1.0, -1.0));

	Scene scene;

//	vector<Body> objs;
	srand(getTickCount());

	for (int i = 0; i < 5; ++i) {
		scene.spheres.push_back(
				Body<Sphere>(
						Sphere(
								Vector3d(1.5 - (3.0 * rand() / RAND_MAX),
										1 + (2.0 * rand() / RAND_MAX), 0), 0.5),
						new Chrome(
								Vector3d(0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX))));

		scene.spheres.push_back(
				Body<Sphere>(
						Sphere(
								Vector3d(1. - (2.0 * rand() / RAND_MAX),
										0.5 + (1.5 * rand() / RAND_MAX), -0.25),
								0.25),
						new Glass(
								Vector3d(0.8 + 0.2 * rand() / RAND_MAX,
										0.8 + 0.2 * rand() / RAND_MAX,
										0.8 + 0.2 * rand() / RAND_MAX), 1.5,
								0.1)));

		Vector3d pos(1.5 - (3.0 * rand() / RAND_MAX),
				1.5 + (2.0 * rand() / RAND_MAX),
				1. + (2.0 * rand() / RAND_MAX));

		scene.cubes.push_back(
				Body<Cube>(
						Cube(pos,
								pos
										+ Vector3d(
												0.5 + 1.0 * rand() / RAND_MAX,
												0.5 + 1.0 * rand() / RAND_MAX,
												0.5 + 1.0 * rand() / RAND_MAX)),
						new Material(
								Vector3d(0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX,
										0.5 + 0.5 * rand() / RAND_MAX),
								Vector3d(0, 0, 0))));
	}

	//X right
	//Y depth
	//Z up

		ObjReader oReader("teapot.obj");

		for(int i = 0; i < oReader.triangles.size(); ++i){
				scene.triangles.push_back(
						Body<Triangle>(oReader.triangles[i],
							new Material(Vector3d(0.5, 0.9, 0.5), Vector3d(0, 0, 0))));
//								new Chrome(
//										Vector3d(0.9,
//												0.9,
//												0.9))));


		}

//
//	scene.triangles.push_back(
//			Body<Triangle>(
//					Triangle(Vector3d(-1, 2, 1),
//							Vector3d(-2, 1, 0),
//							Vector3d(0, 3, 0)),
////					new Material(Vector3d(0.9, 0.9, 0.9), Vector3d(0, 0, 0))));
//					new Chrome(
//							Vector3d(0.9,
//									0.9,
//									0.9))));

//	scene.planes.push_back(
//			Body<Plane>(Plane(Vector3d(1.0, -1.0, 1.0), -3),
//										new Chrome(
//												Vector3d(0.7,
//														0.7,
//														0.9))));


	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(0.0, 0.0, 1.0), -1),
					new Material(Vector3d(0.9, 0.9, 0.9), Vector3d(0, 0, 0))));

	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(0.0, -1, 0.0), -4.5),
//					new Material(Vector3d(0.9, 0.9, 0.9), Vector3d(0, 0, 0))));
										new Chrome(
												Vector3d(0.9,
														0.9,
														0.9))));


	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(0.0, 1, 0.0), -1.5),
					new Material(Vector3d(0.5, 0.5, 0.5), Vector3d(0, 0, 0))));

	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(1, 0.0, 0.0), -3),
					new Material(Vector3d(0.9, 0.5, 0.5), Vector3d(0, 0, 0))));

	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(-1, 0.0, 0.0), -3),
					new Material(Vector3d(0.5, 0.5, 0.9), Vector3d(0, 0, 0))));

	scene.planes.push_back(
			Body<Plane>(Plane(Vector3d(0.0, 0.0, -1), -2.5),
					new Material(Vector3d(1.0, 1.0, 1.0),
							Vector3d(1.29, 1.47, 1.6))));

	Renderer renderer;
	for (int i = 0; i < 100; ++i) {

		double t = getTickCount();
		renderer.iterate(img, cam, scene);
		cout << "Render time: "
				<< ((getTickCount() - t) / getTickFrequency()) * 1000.0 << endl;

		//objs[0].shape.center.x = sin(i/20.0);
		//objs[0].shape.center.y = 2+ cos(i/20.0);
		Mat out;
		//resize(img / (i + 1), out, Size(), 2, 2);
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
