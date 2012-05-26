/*
 * ObjReader.cpp
 *
 *  Created on: May 26, 2012
 *      Author: alexey
 */

#include "ObjReader.h"

ObjReader::ObjReader(char* path) {

	ifstream ifs(path);
	char lineType;
	Vector3i triangle;
	Vector3d point;

	while (!ifs.eof()) {
		ifs >> lineType;
		if (lineType == 'v') {
			ifs >> point.x >> point.z >> point.y;
			points.push_back(point);
	//		cout << "v " << point.x << " " << point.y << " "<< point.z<<endl;

		}
		if (lineType == 'f') {
			ifs >> triangle.x >> triangle.y >> triangle.z;
			iTriangles.push_back(triangle);
		//	cout << "f " << triangle.x << " " << triangle.y << " "<< triangle.z<<endl;
		}
	}

	for (size_t i = 0, lim = iTriangles.size(); i < lim; ++i) {
		triangles.push_back(
				Triangle(points[iTriangles[i].x -1]/3 +Vector3d(1,2,-1),
						points[iTriangles[i].y -1]/3 +Vector3d(1,2,-1),
						points[iTriangles[i].z -1]/3 +Vector3d(1,2,-1)));
	}
	ifs.close();
}

ObjReader::~ObjReader() {
	// TODO Auto-generated destructor stub
}

