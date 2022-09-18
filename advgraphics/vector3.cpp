#include "vector3.h"
#include <assert.h>
//#include <cmath>
#include "math.h"

using namespace mgd;

// Linear operations
Vector3 Vector3::operator * (Scalar k) const { 
	return Vector3(k * x, k * y, k * z); 
}
Vector3 mgd::operator * (Scalar k, const Vector3& a) {
	return a * k;
}
Vector3 Vector3::operator / (Scalar k) const {
	return Vector3(x / k, y / k, z / k);
}
void Vector3::operator *= (Scalar k) { 
	x *= k; y *= k; z *= k; 
}
void Vector3::operator /= (Scalar k) { 
	x /= k; y /= k; z /= k; 
}
Vector3 Vector3::operator + (const Vector3& v) const { 
	return Vector3(v.x + x, v.y + y, v.z + z); 
}
void Vector3::operator += (const Vector3& v) { 
	x += v.x; y += v.y; z += v.z; 
}
Vector3 Vector3::operator - (const Vector3& v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}
void Vector3::operator -= (const Vector3& v) {
	x -= v.x; y -= v.y; z -= v.z;
}
Vector3 Vector3::operator - () const {
	return Vector3(-x,-y,-z);
}
Vector3 Vector3::operator + () const {
	return Vector3(x, y, z);
}
std::ostream& mgd::operator << (std::ostream& os, const Vector3& v) {
	return os << v.x << " " << v.y << " " << v.z;;
}
Scalar mgd::dot(const Vector3 &a,const Vector3 &b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vector3 mgd::cross(const Vector3 &a, const Vector3 &b) {
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}
// Accesses to individual coordinates as a vector
Scalar Vector3::operator[] (int i) const {
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	default: assert(0); return 0;
	}
}
Scalar& Vector3::operator[] (int i) {
	static Scalar dummy;
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	default: assert(0); return dummy;
	}
}
// Equality check
bool Vector3::operator == (const Vector3& b) const {
	return (x == x) && (y == y) && (z == z);
}
bool mgd::areEqual(const Scalar a, const Scalar b) {
	return std::abs(a-b) < TOLERANCE;
}
bool mgd::areEqual(const Vector3 &a, const Vector3 &b) {
	return areEqual(a.x,b.x) &&
		   areEqual(a.y, b.y) &&
		   areEqual(a.z, b.z);
}
bool mgd::isZero(const Scalar a) {
	return std::abs(a) < TOLERANCE;
}
bool mgd::isZero(const Vector3& a) {
	return isZero(a.x) &&
		   isZero(a.y) &&
		   isZero(a.z);
}
// Squared norm (or length)
Scalar Vector3::squaredNorm() const {
	return x * x + y * y + z * z;
}
Scalar Vector3::norm() const {
	return sqrt(squaredNorm());
}
Vector3 Vector3::normalized() const {
	return (* this) / norm();
}
void Vector3::normalize() {
	(* this) /= norm();
}
Scalar mgd::randomScalar(Scalar min, Scalar max)
{
	return min + (max - min) * (std::rand() % 1001) / (Scalar)1001;
}
Vector3 mgd::Vector3::randomVector(Scalar min, Scalar max)
{
	return Vector3(
		randomScalar(min, max),
		randomScalar(min, max),
		randomScalar(min, max)
	);
}

