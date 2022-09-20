#pragma once
#include "quaternion.h"

namespace mgd {

class Transform {
public:
	Scalar scale;
	Vector3 position;
	Quaternion rotation;

	Transform();
	Vector3 transformPoint(const Vector3& p) const;
	Vector3 transformVector(const Vector3& p) const;
	Vector3 transformVersor(const Vector3& p) const;
	Scalar transformScalar(Scalar p) const;
	void invert();
	Transform inverse()const;
	//void transform();
	//Transform transform()const;
	
};

Transform operator * (const Transform& a, const Transform& b);

} //end namespace mgd


