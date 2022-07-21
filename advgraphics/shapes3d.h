#pragma once
#include "vector3.h"

namespace mgd {

struct Ray {
	Point3 p;
	Versor3 d;

	Ray();
	Ray(Point3 _p, Vector3 _d);
};


struct Sphere {
	Point3 c;
	Scalar r;

	Sphere(Point3 _c, Scalar _r);
};


struct Plane {
	Versor3 n;
	Point3 p; // could use k = dot(n,p)
	Plane();
	Plane(Point3 _p, Versor3 _n);
};


bool rayCast(Ray r, Sphere s, Point3& hitPoint, Versor3& hitNorm, float& distMax);
bool rayCast(Ray ray, Plane plane, Point3& hitPoint, Versor3& hitNorm, float &distMax);

};// namespace end
