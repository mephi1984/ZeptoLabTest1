#include "Math.h"

#include <exception>

namespace ZL {

	Vector2f operator+(const Vector2f& x, const Vector2f& y)
	{
		Vector2f result;

		result.v[0] = x.v[0] + y.v[0];
		result.v[1] = x.v[1] + y.v[1];

		return result;
	}

	Vector2f operator-(const Vector2f& x, const Vector2f& y)
	{
		Vector2f result;

		result.v[0] = x.v[0] - y.v[0];
		result.v[1] = x.v[1] - y.v[1];

		return result;
	}

	Matrix4f Matrix4f::Identity()
	{
		Matrix4f r;

		r.m[0] = 1.f;
		r.m[1] = 0.f;
		r.m[2] = 0.f;
		r.m[3] = 0.f;

		r.m[4] = 0.f;
		r.m[5] = 1.f;
		r.m[6] = 0.f;
		r.m[7] = 0.f;

		r.m[8] = 0.f;
		r.m[9] = 0.f;
		r.m[10] = 1.f;
		r.m[11] = 0.f;

		r.m[12] = 0.f;
		r.m[13] = 0.f;
		r.m[14] = 0.f;
		r.m[15] = 1.f;

		return r;

	}

	Matrix4f operator*(const Matrix4f& m1, const Matrix4f& m2)
	{
		Matrix4f r;

		r.m[0] = m1.m[0] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[8] * m2.m[2] + m1.m[12] * m2.m[3];
		r.m[1] = m1.m[1] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[9] * m2.m[2] + m1.m[13] * m2.m[3];
		r.m[2] = m1.m[2] * m2.m[0] + m1.m[6] * m2.m[1] + m1.m[10] * m2.m[2] + m1.m[14] * m2.m[3];
		r.m[3] = m1.m[3] * m2.m[0] + m1.m[7] * m2.m[1] + m1.m[11] * m2.m[2] + m1.m[15] * m2.m[3];

		r.m[4] = m1.m[0] * m2.m[4] + m1.m[4] * m2.m[5] + m1.m[8] * m2.m[6] + m1.m[12] * m2.m[7];
		r.m[5] = m1.m[1] * m2.m[4] + m1.m[5] * m2.m[5] + m1.m[9] * m2.m[6] + m1.m[13] * m2.m[7];
		r.m[6] = m1.m[2] * m2.m[4] + m1.m[6] * m2.m[5] + m1.m[10] * m2.m[6] + m1.m[14] * m2.m[7];
		r.m[7] = m1.m[3] * m2.m[4] + m1.m[7] * m2.m[5] + m1.m[11] * m2.m[6] + m1.m[15] * m2.m[7];


		r.m[8] = m1.m[0] * m2.m[8] + m1.m[4] * m2.m[9] + m1.m[8] * m2.m[10] + m1.m[12] * m2.m[11];
		r.m[9] = m1.m[1] * m2.m[8] + m1.m[5] * m2.m[9] + m1.m[9] * m2.m[10] + m1.m[13] * m2.m[11];
		r.m[10] = m1.m[2] * m2.m[8] + m1.m[6] * m2.m[9] + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11];
		r.m[11] = m1.m[3] * m2.m[8] + m1.m[7] * m2.m[9] + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11];

		r.m[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8] * m2.m[14] + m1.m[12] * m2.m[15];
		r.m[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9] * m2.m[14] + m1.m[13] * m2.m[15];
		r.m[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
		r.m[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];

		return r;
	}

	Matrix4f MakeOrthoMatrix(float width, float height, float zNear, float zFar)
	{
		float depthRange = zFar - zNear;

		if (depthRange <= 0)
		{
			throw std::runtime_error("zFar must be greater than zNear");
		}

		Matrix4f r;

		r.m[0] = 2.f / width;
		r.m[1] = 0;
		r.m[2] = 0;
		r.m[3] = 0;

		r.m[4] = 0;
		r.m[5] = 2.f / height;
		r.m[6] = 0;
		r.m[7] = 0;

		r.m[8] = 0;
		r.m[9] = 0;
		r.m[10] = -1 / depthRange;
		r.m[11] = 0;

		r.m[12] = -1;
		r.m[13] = -1;
		r.m[14] = zNear / depthRange;
		r.m[15] = 1;

		return r;
	}

	Matrix3f QuatToMatrix(const Vector4f& q)
	{
		Matrix3f m;

		float wx, wy, wz, xx, yy, yz, xy, xz, zz, s, x2, y2, z2;

		s = 2.0f / (q.v[0] * q.v[0] + q.v[1] * q.v[1] + q.v[2] * q.v[2] + q.v[3] * q.v[3]);


		x2 = q.v[0] * s;
		y2 = q.v[1] * s;
		z2 = q.v[2] * s;

		wx = q.v[3] * x2; wy = q.v[3] * y2; wz = q.v[3] * z2;
		xx = q.v[0] * x2; xy = q.v[1] * x2; xz = q.v[2] * x2;
		yy = q.v[1] * y2; yz = q.v[2] * y2;
		zz = q.v[2] * z2;

		m.m[0] = 1.0f - (yy + zz);
		m.m[1] = xy + wz;
		m.m[2] = xz - wy;

		m.m[3] = xy - wz;
		m.m[4] = 1.0f - (xx + zz);
		m.m[5] = yz + wx;

		m.m[6] = xz + wy;
		m.m[7] = yz - wx;
		m.m[8] = 1.0f - (xx + yy);

		return m;

	}

	Vector4f QuatFromRotateAroundZ(float angle)
	{
		Vector4f result;

		result.v[0] = 0.f;
		result.v[1] = 0.f;
		result.v[2] = sinf(angle * 0.5f);
		result.v[3] = cosf(angle * 0.5f);

		return result;
	}


};
