#pragma once
#include <iostream>


namespace mgd {

typedef float Scalar;
const Scalar TOLERANCE = 1e-5f;

struct Vector3 {
	Scalar x, y, z;

	Vector3() :x(0), y(0), z(0) {}
	Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}

	// Linear operations
	Vector3 operator * (Scalar k) const;
	Vector3 operator / (Scalar k) const;
	void operator *= (Scalar k);
	void operator /= (Scalar k);
	Vector3 operator + (const Vector3& v) const;
	void operator += (const Vector3& v);
	Vector3 operator - (const Vector3& v) const;
	void operator -= (const Vector3& v);
	Vector3 operator - () const;
	Vector3 operator + () const;
	//norm (aka magnitude, length, intensity)
	Scalar squaredNorm() const;
	Scalar norm() const;
	Vector3 normalized() const;
	void normalize();
	// Access coordinates
	Scalar operator [] (int i) const;
	Scalar& operator[] (int i);
	// Equals
	bool operator == (const Vector3& b) const;
	// Print
	friend std::ostream& operator << (std::ostream& os, const Vector3& v);
	// static func
	static Vector3 randomVector(Scalar min, Scalar max);

};

typedef Vector3 Point3;
typedef Vector3 Versor3;

std::ostream& operator << (std::ostream& os, const Vector3& v);
Scalar dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);
bool areEqual(const Scalar a, const Scalar b);
bool areEqual(const Vector3 &a, const Vector3 &b);
bool isZero(const Scalar a);
bool isZero(const Vector3& a);
Vector3 operator * (Scalar k, const Vector3& a);
Scalar randomScalar(Scalar min, Scalar max);



};

