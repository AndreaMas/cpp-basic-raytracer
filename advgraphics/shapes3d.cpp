#include "shapes3d.h"
#include "vector3.h"
#include <cmath>

using namespace mgd;

Ray::Ray() :p(Point3()), d(Vector3()) {}

Ray::Ray(Point3 _p, Vector3 _d) : p(_p), d(_d.normalized()) {}

Plane::Plane(){}

Plane::Plane(Point3 _p, Versor3 _n) : p(_p), n(_n){};

Sphere::Sphere(Point3 _c, Scalar _r):c(_c),r(_r){}

bool mgd::rayCast(Ray ray, Sphere sphere, Point3& hitPos, Versor3 &hitNorm, float& distMax) {
	// hitpos is: (ray.p + k * ray.dir)
	// with this eq true: (a*k^2 + b*k + c = 0)
	Scalar a = 1.0;
	Scalar b = 2 * dot(ray.d, ray.p - sphere.c);
	Scalar c = (ray.p - sphere.c).squaredNorm() - sphere.r * sphere.r;

	Scalar delta = b * b - 4 * a * c;
	if (delta < 0) return false; // ray missed sphere

	Scalar k = (-b - sqrt(delta)) / (2 * a);
	if (k < 0) return false; // ray missed sphere
	if (k > distMax) return false; // sphere is behind other object
	distMax = k;

	hitPos = ray.p + k * ray.d;
	hitNorm = (hitPos - sphere.c).normalized();
	return true;
}

bool mgd::rayCast(Ray ray, Plane plane, Point3& hitPos, Versor3& hitNorm, float &distMax) {
	Scalar dn = dot(ray.d, plane.n);
	if (isZero(dn)) return false;
	Scalar k = dot(plane.p - ray.p, plane.n) / dn;
	if (k < 0) return false;
	if (k > distMax) return false;
	distMax = k;
	hitPos = ray.p + k * ray.d;
	hitNorm = plane.n;
	return true;
}

Sphere mgd::applyTransToSphere(const Transform& a, const Sphere& s)
{
	return Sphere(
		a.transformPoint(s.c),
	    a.transformScalar(s.r)
	);
}

Plane mgd::applyTransToPlane(const Transform& a, const Plane& s)
{
	return Plane(
		a.transformPoint(s.p),
		a.transformVersor(s.n)
	);
}

