#include "faceObj.h"
#include <vector>

using namespace mgd;

FaceObj::FaceObj():
	body(Vector3(0, 0, 0), 1),
	nose(Vector3(0, 0, 0.8f), 0.5f)
{}

std::vector<mgd::Sphere> FaceObj::meshInWorldSpace() const
{
	std::vector<Sphere> res;
	res.push_back(applyTransToSphere(transform, body));
	res.push_back(applyTransToSphere(transform, nose));
	return res;
}
