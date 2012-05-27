/*
 * ObjReader.h
 *
 *  Created on: May 26, 2012
 *      Author: alexey
 */

#ifndef OBJREADER_H_
#define OBJREADER_H_

#include <vector>
#include <fstream>

#include "core.h"
#include "Shape.h"

using namespace std;

class ObjReader {


	vector<Vector3d> points;
	vector<Vector3i> iTriangles;
public:

	vector<Triangle> triangles;

	ObjReader(char *path);
	virtual ~ObjReader();

};

#endif /* OBJREADER_H_ */
