#include "transform.h"

using namespace mgd;

Transform::Transform():rotation(Quaternion::identity()), 
						scale(1.0f), position(0,0,0) {}

Vector3 Transform::transformPoint(const Vector3& p) const
{
	return rotation.applyRotationTo(p * scale) + position;
}

Vector3 Transform::transformVector(const Vector3& p) const
{
	return rotation.applyRotationTo(p * scale);
}

Vector3 Transform::transformVersor(const Vector3& p) const
{
	return rotation.applyRotationTo(p);
}

Scalar Transform::transformScalar(Scalar p) const // simply scale
{
	return p * scale; 
}



Transform Transform::inverse() const
{
	Transform t;
	t.scale = (1 / scale);
	t.rotation = rotation.conjugated();
	t.position = t.rotation.applyRotationTo(-position * t.scale);
	return t;
}

void Transform::invert()
{
	scale = (1 / scale);
	rotation.conjugate();
	position = rotation.applyRotationTo(-position * scale);
}



// first b then a
Transform mgd::operator*(const Transform& a, const Transform& b)
{
	Transform t;
	t.rotation = a.rotation * b.rotation;
	t.scale = a.scale * b.scale;
	t.position = a.transformVector(b.position) + a.position;
	return t;
}
