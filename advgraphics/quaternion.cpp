#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "quaternion.h"

using namespace mgd;

Quaternion::Quaternion(Vector3 _im, Scalar _re) :im(_im), re(_re){}

Quaternion::Quaternion() :im(), re(1) {}

void Quaternion::conjugate()
{
	im = -im;
}

Quaternion Quaternion::conjugated() const
{
	return Quaternion(-im, re);
}

Quaternion Quaternion::fromVector3(const Vector3& v)
{ // convert vector in quaternion appending zero
	return Quaternion(v, 0);
}

Quaternion mgd::operator * (const Quaternion& a, const Quaternion& b){
	return Quaternion(
		cross(a.im, b.im) + a.im * b.re + a.re * b.im,
		- dot(a.im, b.im) + a.re * b.re
		);
}

Vector3 Quaternion::applyRotationTo(Vector3 v) const {
	Quaternion q = fromVector3(v);
	q = (*this) * q * (*this).conjugated();
	return q.im;
}

Quaternion Quaternion::fromAngleAxis(Scalar degrees, Vector3 axis)
{
	Scalar rad = degrees * M_PI / 180;
	Scalar s = std::sin(rad/2);
	Scalar c = std::cos(rad/2);
	return Quaternion(axis.normalized() * s, c);
}

Quaternion Quaternion::identity()
{
	return Quaternion(Vector3(0, 0, 0), 1);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-im, -re);
}

bool mgd::areEqual(const Quaternion& a, const Quaternion& b)
{
	return areEqual(a.im, b.im) && areEqual(a.re, b.re);
}

bool mgd::areEquivalent(const Quaternion& a, const Quaternion& b)
{
	return areEqual(a, b) || areEqual(a, -b);
}

std::ostream& mgd::operator<<(std::ostream& os, const Quaternion& q)
{
	return os << q.im << " " << q.re << std::endl;
}
