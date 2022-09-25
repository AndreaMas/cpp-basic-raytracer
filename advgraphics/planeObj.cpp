#include "planeObj.h"
#include <vector>

using namespace mgd;

PlaneObj::PlaneObj() :
	plane(Point3(0,-1,0), Versor3(0,1,0))
{}

PlaneObj::PlaneObj(Point3 p, Versor3 n):
	plane(p, n)
{}

Plane PlaneObj::planeMeshInWorldSpace() const
{
	return applyTransToPlane(transform, plane);
}
