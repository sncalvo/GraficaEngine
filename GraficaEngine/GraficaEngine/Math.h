#pragma once

#include <cmath>
#include <iostream>

#include "SDL_openGL.h"

class Vec3
{
private:
	GLfloat _vector[3];
public:
	Vec3() : _vector{ 0, 0, 0 } {}
	Vec3(GLfloat x, GLfloat y, GLfloat z) : _vector{ x, y, z } {}
	GLfloat x() const { return _vector[0]; }
	GLfloat y() const { return _vector[1]; }
	GLfloat z() const { return _vector[2]; }
	Vec3 operator-() const { return Vec3(-_vector[0], -_vector[1], -_vector[2]); }
	GLfloat operator[](int i) const { return _vector[i]; }
	GLfloat& operator[](int i) { return _vector[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		_vector[0] += v._vector[0];
		_vector[1] += v._vector[1];
		_vector[2] += v._vector[2];
		return *this;
	}

	Vec3& operator*=(const GLfloat t)
	{
		_vector[0] *= t;
		_vector[1] *= t;
		_vector[2] *= t;
		return *this;
	}

	Vec3& operator/=(const GLfloat t)
	{
		return *this *= 1 / t;
	}

	GLfloat norm() const
	{
		return std::sqrt(normSquared());
	}

	GLfloat normSquared() const
	{
		return _vector[0] * _vector[0] + _vector[1] * _vector[1] + _vector[2] * _vector[2];
	}
};

using Point3 = Vec3;

// Vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << "X: " << v.x() << ", Y: " << v.y() << ", Z: " << v.z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(GLfloat t, const Vec3& v)
{
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3& v, GLfloat t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, GLfloat t)
{
	return (1 / t) * v;
}

inline GLfloat dot(const Vec3& u, const Vec3& v)
{
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z();
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

inline Vec3 unitVector(Vec3 v)
{
	return v / v.norm();
}

class Vec4
{
public:
	GLfloat _vector[4];
	Vec4() : _vector{ 0, 0, 0, 0 } {}
	Vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : _vector{ x, y, z, w } {}
	GLfloat r() const { return _vector[0]; }
	GLfloat g() const { return _vector[1]; }
	GLfloat b() const { return _vector[2]; }
	GLfloat a() const { return _vector[3]; }

	GLfloat x() const { return _vector[0]; }
	GLfloat y() const { return _vector[1]; }
	GLfloat z() const { return _vector[2]; }
	GLfloat w() const { return _vector[3]; }

	Vec4 operator-() const { return Vec4(-_vector[0], -_vector[1], -_vector[2], -_vector[3]); }
	GLfloat operator[](int i) const { return _vector[i]; }
	GLfloat& operator[](int i) { return _vector[i]; }

	Vec4& operator+=(const Vec4& v)
	{
		_vector[0] += v._vector[0];
		_vector[1] += v._vector[1];
		_vector[2] += v._vector[2];
		_vector[3] += v._vector[3];
		return *this;
	}

	Vec4& operator*=(const GLfloat t)
	{
		_vector[0] *= t;
		_vector[1] *= t;
		_vector[2] *= t;
		_vector[3] *= t;
		return *this;
	}

	Vec4& operator/=(const GLfloat t)
	{
		return *this *= 1 / t;
	}

	GLfloat norm() const
	{
		return std::sqrt(normSquared());
	}

	GLfloat normSquared() const
	{
		return _vector[0] * _vector[0] + _vector[1] * _vector[1] + _vector[2] * _vector[2] + _vector[3] * _vector[3];
	}
};

using Point4 = Vec4;
using Color = Vec4;

// Vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec4& v)
{
	return out << "X: " << v.x() << ", Y: " << v.y() << ", Z: " << v.z() << "W: " << v.w();
}

inline Vec4 operator+(const Vec4& u, const Vec4& v)
{
	return Vec4(u.x() + v.x(), u.y() + v.y(), u.z() + v.z(), u.w() + v.w());
}

inline Vec4 operator-(const Vec4& u, const Vec4& v)
{
	return Vec4(u.x() - v.x(), u.y() - v.y(), u.z() - v.z(), u.w() - v.w());
}

inline Vec4 operator*(const Vec4& u, const Vec4& v)
{
	return Vec4(u.x() * v.x(), u.y() * v.y(), u.z() * v.z(), u.w() * v.w());
}

inline Vec4 operator*(GLfloat t, const Vec4& v)
{
	return Vec4(t * v.x(), t * v.y(), t * v.z(), t * v.w());
}

inline Vec4 operator*(const Vec4& v, GLfloat t)
{
	return t * v;
}

inline Vec4 operator/(const Vec4& v, GLfloat t)
{
	return (1 / t) * v;
}

inline GLfloat dot(const Vec4& u, const Vec4& v)
{
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z()
		+ u.w() * v.w();
}
