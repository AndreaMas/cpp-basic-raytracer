#include "faceObj.h"
#include <vector>

using namespace mgd;

FaceObj::FaceObj():
	body(Vector3(0, 0, 0), 1),
	nose(Vector3(0, 0, 0.8), 0.5)
{

}

std::vector<mgd::Sphere> FaceObj::meshInWorldSpace() const override
{
	std::vector<Sphere> res;
	res.push_back(applyTransToSphere(transform, body));
	res.push_back(applyTransToSphere(transform, nose));
}
