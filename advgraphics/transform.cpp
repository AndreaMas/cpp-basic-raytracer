#include "transform.h"

using namespace mgd;

Transform::Transform():rotate(Quaternion::identity()), 
						scale(1.0f), translate(0,0,0) {}

Vector3 Transform::transformPoint(const Vector3& p) const
{
	return rotate.applyRotationTo(p * scale) + translate;
}

Vector3 Transform::transformVector(const Vector3& p) const
{
	return rotate.applyRotationTo(p * scale);
}

Vector3 Transform::transformVersor(const Vector3& p) const
{
	return rotate.applyRotationTo(p);
}

Scalar Transform::transformScalar(Scalar p) const // simply scale
{
	return p * scale; 
}



Transform Transform::inverse() const
{
	Transform t;
	t.scale = (1 / scale);
	t.rotate = rotate.conjugated();
	t.translate = t.rotate.applyRotationTo(-translate * t.scale);
	return t;
}

void Transform::invert()
{
	scale = (1 / scale);
	rotate.conjugate();
	translate = rotate.applyRotationTo(-translate * scale);
}



// first b then a
Transform mgd::operator*(const Transform& a, const Transform& b)
{
	Transform t;
	t.rotate = a.rotate * b.rotate;
	t.scale = a.scale * b.scale;
	t.translate = a.transformVector(b.translate) + a.translate;
	return t;
}
