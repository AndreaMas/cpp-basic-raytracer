#pragma once
#include "quaternion.h"

namespace mgd {

class Transform {
public:
	Scalar scale;
	Vector3 translate;
	Quaternion rotate;

	Transform();
	Vector3 transformPoint(const Vector3& p);
	Vector3 transformVector(const Vector3& p);
	Vector3 transformVersor(const Vector3& p);
	void invert();
	Transform inverse()const;
	//test
	
};

}


