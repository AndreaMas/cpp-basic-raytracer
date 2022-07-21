#pragma once
#include <iostream>
#include "vector3.h"


namespace mgd {

class Quaternion {
public:
	Vector3 im;
	Scalar re;

	Quaternion(Vector3 _im, Scalar _re);
	void conjugate();
	Quaternion conjugated() const;
	static Quaternion fromVector3(const Vector3& v);
	static Quaternion fromAngleAxis(Scalar degrees, Vector3 axis);
	static Quaternion identity();
	Quaternion operator - () const;
	friend std::ostream& operator << (std::ostream& os, const Quaternion& q);
	
	Vector3 applyRotationTo(Vector3 v) const;
	
	
	
};


Quaternion operator * (const Quaternion& a, const Quaternion& b);
bool areEqual(const Quaternion& a, const Quaternion& b);
bool areEquivalent(const Quaternion& a, const Quaternion& b);
std::ostream& operator << (std::ostream& os, const Quaternion& q);

} // end of namespace mgd



