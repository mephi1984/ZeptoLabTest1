#include "Math.h"

#include <exception>
#include <cmath>

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

	Vector3f operator+(const Vector3f& x, const Vector3f& y)
	{
		Vector3f result;

		result.v[0] = x.v[0] + y.v[0];
		result.v[1] = x.v[1] + y.v[1];
		result.v[2] = x.v[2] + y.v[2];
		return result;
	}

	Vector3f operator-(const Vector3f& x, const Vector3f& y)
	{
		Vector3f result;

		result.v[0] = x.v[0] - y.v[0];
		result.v[1] = x.v[1] - y.v[1];
		result.v[2] = x.v[2] - y.v[2];
		return result;
	}

	Vector4f operator+(const Vector4f& x, const Vector4f& y)
	{
		Vector4f result;

		result.v[0] = x.v[0] + y.v[0];
		result.v[1] = x.v[1] + y.v[1];
		result.v[2] = x.v[2] + y.v[2];
		result.v[3] = x.v[3] + y.v[3];
		return result;
	}

	Vector4f operator-(const Vector4f& x, const Vector4f& y)
	{
		Vector4f result;

		result.v[0] = x.v[0] - y.v[0];
		result.v[1] = x.v[1] - y.v[1];
		result.v[2] = x.v[2] - y.v[2];
		result.v[3] = x.v[3] - y.v[3];
		return result;
	}

	Matrix3f Matrix3f::Identity()
	{
		Matrix3f r;

		r.m[0] = 1.f;
		r.m[1] = 0.f;
		r.m[2] = 0.f;

		r.m[3] = 0.f;
		r.m[4] = 1.f;
		r.m[5] = 0.f;

		r.m[6] = 0.f;
		r.m[7] = 0.f;
		r.m[8] = 1.f;
		
		return r;

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

	Matrix4f MakePerspectiveMatrix(float fovY, float aspectRatio, float zNear, float zFar)
	{
		float tanHalfFovy = tan(fovY / 2.f);
		Matrix4f r;

		if (zNear >= zFar || aspectRatio == 0)
		{
			throw std::runtime_error("Invalid perspective parameters");
		}

		r.m[0] = 1.f / (aspectRatio * tanHalfFovy);
		r.m[1] = 0;
		r.m[2] = 0;
		r.m[3] = 0;

		r.m[4] = 0;
		r.m[5] = 1.f / (tanHalfFovy);
		r.m[6] = 0;
		r.m[7] = 0;

		r.m[8] = 0;
		r.m[9] = 0;
		r.m[10] = -(zFar + zNear) / (zFar - zNear);
		r.m[11] = -1;

		r.m[12] = 0;
		r.m[13] = 0;
		r.m[14] = -(2.f * zFar * zNear) / (zFar - zNear);
		r.m[15] = 0;

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

	Vector4f MatrixToQuat(const Matrix3f& m)
	{
		Vector4f r;
		float trace = m.m[0] + m.m[4] + m.m[8]; 

		if (trace > 0)
		{
			float s = 0.5f / sqrtf(trace + 1.0f);
			r.v[3] = 0.25f / s;
			r.v[0] = (m.m[5] - m.m[7]) * s;
			r.v[1] = (m.m[6] - m.m[2]) * s;
			r.v[2] = (m.m[1] - m.m[3]) * s;
		}
		else if (m.m[0] > m.m[4] && m.m[0] > m.m[8])
		{
			float s = 2.0f * sqrtf(1.0f + m.m[0] - m.m[4] - m.m[8]);
			r.v[3] = (m.m[5] - m.m[7]) / s;
			r.v[0] = 0.25f * s;
			r.v[1] = (m.m[1] + m.m[3]) / s;
			r.v[2] = (m.m[6] + m.m[2]) / s;
		}
		else if (m.m[4] > m.m[8])
		{
			float s = 2.0f * sqrtf(1.0f + m.m[4] - m.m[0] - m.m[8]);
			r.v[3] = (m.m[6] - m.m[2]) / s;
			r.v[0] = (m.m[1] + m.m[3]) / s;
			r.v[1] = 0.25f * s;
			r.v[2] = (m.m[5] + m.m[7]) / s;
		}
		else
		{
			float s = 2.0f * sqrtf(1.0f + m.m[8] - m.m[0] - m.m[4]);
			r.v[3] = (m.m[1] - m.m[3]) / s;
			r.v[0] = (m.m[6] + m.m[2]) / s;
			r.v[1] = (m.m[5] + m.m[7]) / s;
			r.v[2] = 0.25f * s;
		}

		return r.normalized();
	}

	Vector4f QuatFromRotateAroundX(float angle)
	{
		Vector4f result;

		result.v[0] = sinf(angle * 0.5f);
		result.v[1] = 0.f;
		result.v[2] = 0.f;
		result.v[3] = cosf(angle * 0.5f);

		return result;
	}

	Vector4f QuatFromRotateAroundY(float angle)
	{
		Vector4f result;

		result.v[0] = 0.f;
		result.v[1] = sinf(angle * 0.5f);
		result.v[2] = 0.f;
		result.v[3] = cosf(angle * 0.5f);

		return result;
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

	Matrix3f TransposeMatrix(const Matrix3f& m)
	{
		Matrix3f r;
		r.m[0] = m.m[0];
		r.m[1] = m.m[3];
		r.m[2] = m.m[6];
		r.m[3] = m.m[1];
		r.m[4] = m.m[4];
		r.m[5] = m.m[7];
		r.m[6] = m.m[2];
		r.m[7] = m.m[5];
		r.m[8] = m.m[8];
		return r;
	}

	Matrix3f InverseMatrix(const Matrix3f& m)
	{
		float d;
		Matrix3f r;

		d = m.m[0] * (m.m[4] * m.m[8] - m.m[5] * m.m[7]);
		d -= m.m[1] * (m.m[3] * m.m[8] - m.m[6] * m.m[5]);
		d += m.m[2] * (m.m[3] * m.m[7] - m.m[6] * m.m[4]);

		if (fabs(d) < 0.01f)
		{
			throw std::runtime_error("Error: matrix cannot be inversed!");

		}
		else
		{

			r.m[0] = (m.m[4] * m.m[8] - m.m[5] * m.m[7]) / d;
			r.m[1] = -(m.m[1] * m.m[8] - m.m[2] * m.m[7]) / d;
			r.m[2] = (m.m[1] * m.m[5] - m.m[2] * m.m[4]) / d;

			r.m[3] = -(m.m[3] * m.m[8] - m.m[5] * m.m[6]) / d;
			r.m[4] = (m.m[0] * m.m[8] - m.m[2] * m.m[6]) / d;
			r.m[5] = -(m.m[0] * m.m[5] - m.m[2] * m.m[3]) / d;

			r.m[6] = (m.m[3] * m.m[7] - m.m[6] * m.m[4]) / d;
			r.m[7] = -(m.m[0] * m.m[7] - m.m[6] * m.m[1]) / d;
			r.m[8] = (m.m[0] * m.m[4] - m.m[1] * m.m[3]) / d;

		};
		return r;
	}


	Matrix4f InverseMatrix(const Matrix4f& mat)
	{
		Matrix4f inv;
		float det;

		inv.m[0] = mat.m[5] * mat.m[10] * mat.m[15] -
			mat.m[5] * mat.m[11] * mat.m[14] -
			mat.m[9] * mat.m[6] * mat.m[15] +
			mat.m[9] * mat.m[7] * mat.m[14] +
			mat.m[13] * mat.m[6] * mat.m[11] -
			mat.m[13] * mat.m[7] * mat.m[10];

		inv.m[4] = -mat.m[4] * mat.m[10] * mat.m[15] +
			mat.m[4] * mat.m[11] * mat.m[14] +
			mat.m[8] * mat.m[6] * mat.m[15] -
			mat.m[8] * mat.m[7] * mat.m[14] -
			mat.m[12] * mat.m[6] * mat.m[11] +
			mat.m[12] * mat.m[7] * mat.m[10];

		inv.m[8] = mat.m[4] * mat.m[9] * mat.m[15] -
			mat.m[4] * mat.m[11] * mat.m[13] -
			mat.m[8] * mat.m[5] * mat.m[15] +
			mat.m[8] * mat.m[7] * mat.m[13] +
			mat.m[12] * mat.m[5] * mat.m[11] -
			mat.m[12] * mat.m[7] * mat.m[9];

		inv.m[12] = -mat.m[4] * mat.m[9] * mat.m[14] +
			mat.m[4] * mat.m[10] * mat.m[13] +
			mat.m[8] * mat.m[5] * mat.m[14] -
			mat.m[8] * mat.m[6] * mat.m[13] -
			mat.m[12] * mat.m[5] * mat.m[10] +
			mat.m[12] * mat.m[6] * mat.m[9];

		inv.m[1] = -mat.m[1] * mat.m[10] * mat.m[15] +
			mat.m[1] * mat.m[11] * mat.m[14] +
			mat.m[9] * mat.m[2] * mat.m[15] -
			mat.m[9] * mat.m[3] * mat.m[14] -
			mat.m[13] * mat.m[2] * mat.m[11] +
			mat.m[13] * mat.m[3] * mat.m[10];

		inv.m[5] = mat.m[0] * mat.m[10] * mat.m[15] -
			mat.m[0] * mat.m[11] * mat.m[14] -
			mat.m[8] * mat.m[2] * mat.m[15] +
			mat.m[8] * mat.m[3] * mat.m[14] +
			mat.m[12] * mat.m[2] * mat.m[11] -
			mat.m[12] * mat.m[3] * mat.m[10];

		inv.m[9] = -mat.m[0] * mat.m[9] * mat.m[15] +
			mat.m[0] * mat.m[11] * mat.m[13] +
			mat.m[8] * mat.m[1] * mat.m[15] -
			mat.m[8] * mat.m[3] * mat.m[13] -
			mat.m[12] * mat.m[1] * mat.m[11] +
			mat.m[12] * mat.m[3] * mat.m[9];

		inv.m[13] = mat.m[0] * mat.m[9] * mat.m[14] -
			mat.m[0] * mat.m[10] * mat.m[13] -
			mat.m[8] * mat.m[1] * mat.m[14] +
			mat.m[8] * mat.m[2] * mat.m[13] +
			mat.m[12] * mat.m[1] * mat.m[10] -
			mat.m[12] * mat.m[2] * mat.m[9];

		inv.m[2] = mat.m[1] * mat.m[6] * mat.m[15] -
			mat.m[1] * mat.m[7] * mat.m[14] -
			mat.m[5] * mat.m[2] * mat.m[15] +
			mat.m[5] * mat.m[3] * mat.m[14] +
			mat.m[13] * mat.m[2] * mat.m[7] -
			mat.m[13] * mat.m[3] * mat.m[6];

		inv.m[6] = -mat.m[0] * mat.m[6] * mat.m[15] +
			mat.m[0] * mat.m[7] * mat.m[14] +
			mat.m[4] * mat.m[2] * mat.m[15] -
			mat.m[4] * mat.m[3] * mat.m[14] -
			mat.m[12] * mat.m[2] * mat.m[7] +
			mat.m[12] * mat.m[3] * mat.m[6];

		inv.m[10] = mat.m[0] * mat.m[5] * mat.m[15] -
			mat.m[0] * mat.m[7] * mat.m[13] -
			mat.m[4] * mat.m[1] * mat.m[15] +
			mat.m[4] * mat.m[3] * mat.m[13] +
			mat.m[12] * mat.m[1] * mat.m[7] -
			mat.m[12] * mat.m[3] * mat.m[5];

		inv.m[14] = -mat.m[0] * mat.m[5] * mat.m[14] +
			mat.m[0] * mat.m[6] * mat.m[13] +
			mat.m[4] * mat.m[1] * mat.m[14] -
			mat.m[4] * mat.m[2] * mat.m[13] -
			mat.m[12] * mat.m[1] * mat.m[6] +
			mat.m[12] * mat.m[2] * mat.m[5];

		inv.m[3] = -mat.m[1] * mat.m[6] * mat.m[11] +
			mat.m[1] * mat.m[7] * mat.m[10] +
			mat.m[5] * mat.m[2] * mat.m[11] -
			mat.m[5] * mat.m[3] * mat.m[10] -
			mat.m[9] * mat.m[2] * mat.m[7] +
			mat.m[9] * mat.m[3] * mat.m[6];

		inv.m[7] = mat.m[0] * mat.m[6] * mat.m[11] -
			mat.m[0] * mat.m[7] * mat.m[10] -
			mat.m[4] * mat.m[2] * mat.m[11] +
			mat.m[4] * mat.m[3] * mat.m[10] +
			mat.m[8] * mat.m[2] * mat.m[7] -
			mat.m[8] * mat.m[3] * mat.m[6];

		inv.m[11] = -mat.m[0] * mat.m[5] * mat.m[11] +
			mat.m[0] * mat.m[7] * mat.m[9] +
			mat.m[4] * mat.m[1] * mat.m[11] -
			mat.m[4] * mat.m[3] * mat.m[9] -
			mat.m[8] * mat.m[1] * mat.m[7] +
			mat.m[8] * mat.m[3] * mat.m[5];

		inv.m[15] = mat.m[0] * mat.m[5] * mat.m[10] -
			mat.m[0] * mat.m[6] * mat.m[9] -
			mat.m[4] * mat.m[1] * mat.m[10] +
			mat.m[4] * mat.m[2] * mat.m[9] +
			mat.m[8] * mat.m[1] * mat.m[6] -
			mat.m[8] * mat.m[2] * mat.m[5];

		// ��������� �����������
		det = mat.m[0] * inv.m[0] + mat.m[1] * inv.m[4] + mat.m[2] * inv.m[8] + mat.m[3] * inv.m[12];

		if (std::fabs(det) < 0.01f)
		{
			throw std::runtime_error("Error: matrix cannot be inversed!");
		}

		// ����� ��� �������� �� �����������
		det = 1.0f / det;
		for (int i = 0; i < 16; i++)
		{
			inv.m[i] *= det;
		}

		return inv;
	}

	Matrix3f CreateZRotationMatrix(float angle)
	{
		Matrix3f result = Matrix3f::Identity();

		result.m[0] = cosf(angle);
		result.m[1] = -sinf(angle);
		result.m[3] = sinf(angle);
		result.m[4] = cosf(angle);

		return result;
	}

	Matrix4f MultMatrixMatrix(const Matrix4f& m1, const Matrix4f& m2)
	{
		Matrix4f rx;

		rx.m[0] = m1.m[0] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[8] * m2.m[2] + m1.m[12] * m2.m[3];
		rx.m[1] = m1.m[1] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[9] * m2.m[2] + m1.m[13] * m2.m[3];
		rx.m[2] = m1.m[2] * m2.m[0] + m1.m[6] * m2.m[1] + m1.m[10] * m2.m[2] + m1.m[14] * m2.m[3];
		rx.m[3] = m1.m[3] * m2.m[0] + m1.m[7] * m2.m[1] + m1.m[11] * m2.m[2] + m1.m[15] * m2.m[3];

		rx.m[4] = m1.m[0] * m2.m[4] + m1.m[4] * m2.m[5] + m1.m[8] * m2.m[6] + m1.m[12] * m2.m[7];
		rx.m[5] = m1.m[1] * m2.m[4] + m1.m[5] * m2.m[5] + m1.m[9] * m2.m[6] + m1.m[13] * m2.m[7];
		rx.m[6] = m1.m[2] * m2.m[4] + m1.m[6] * m2.m[5] + m1.m[10] * m2.m[6] + m1.m[14] * m2.m[7];
		rx.m[7] = m1.m[3] * m2.m[4] + m1.m[7] * m2.m[5] + m1.m[11] * m2.m[6] + m1.m[15] * m2.m[7];


		rx.m[8] = m1.m[0] * m2.m[8] + m1.m[4] * m2.m[9] + m1.m[8] * m2.m[10] + m1.m[12] * m2.m[11];
		rx.m[9] = m1.m[1] * m2.m[8] + m1.m[5] * m2.m[9] + m1.m[9] * m2.m[10] + m1.m[13] * m2.m[11];
		rx.m[10] = m1.m[2] * m2.m[8] + m1.m[6] * m2.m[9] + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11];
		rx.m[11] = m1.m[3] * m2.m[8] + m1.m[7] * m2.m[9] + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11];

		rx.m[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8] * m2.m[14] + m1.m[12] * m2.m[15];
		rx.m[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9] * m2.m[14] + m1.m[13] * m2.m[15];
		rx.m[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
		rx.m[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];

		return rx;
	}

	Matrix3f MultMatrixMatrix(const Matrix3f& m1, const Matrix3f& m2)
	{
		Matrix3f r;

		r.m[0] = m1.m[0] * m2.m[0] + m1.m[3] * m2.m[1] + m1.m[6] * m2.m[2];
		r.m[1] = m1.m[1] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[7] * m2.m[2];
		r.m[2] = m1.m[2] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[8] * m2.m[2];

		r.m[3] = m1.m[0] * m2.m[3] + m1.m[3] * m2.m[4] + m1.m[6] * m2.m[5];
		r.m[4] = m1.m[1] * m2.m[3] + m1.m[4] * m2.m[4] + m1.m[7] * m2.m[5];
		r.m[5] = m1.m[2] * m2.m[3] + m1.m[5] * m2.m[4] + m1.m[8] * m2.m[5];

		r.m[6] = m1.m[0] * m2.m[6] + m1.m[3] * m2.m[7] + m1.m[6] * m2.m[8] ;
		r.m[7] = m1.m[1] * m2.m[6] + m1.m[4] * m2.m[7] + m1.m[7] * m2.m[8];
		r.m[8] = m1.m[2] * m2.m[6] + m1.m[5] * m2.m[7] + m1.m[8] * m2.m[8];

		return r;
	}

	Matrix3f MakeTranslationMatrix(const Vector3f& p)
	{
		Matrix3f r = Matrix3f::Identity();

		r.m[12] = p.v[0];
		r.m[13] = p.v[1];
		r.m[14] = p.v[2];

		return r;
	}

	Matrix3f MakeScaleMatrix(float scale)
	{
		Matrix3f r = Matrix3f::Identity();

		r.m[0] = scale;
		r.m[5] = scale;
		r.m[10] = scale;

		return r;
	}

	Matrix3f MakeRotationMatrix(const Vector3f& p)
	{
		Matrix3f r = Matrix3f::Identity();

		r.m[12] = p.v[0];
		r.m[13] = p.v[1];
		r.m[14] = p.v[2];

		return r;
	}

	Vector3f operator*(Vector3f v, float scale)
	{
		Vector3f r = v;

		r.v[0] = v.v[0] * scale;
		r.v[1] = v.v[1] * scale;
		r.v[2] = v.v[2] * scale;

		return r;
	}

	Vector4f operator*(Vector4f v, float scale)
	{
		Vector4f r = v;

		r.v[0] = v.v[0] * scale;
		r.v[1] = v.v[1] * scale;
		r.v[2] = v.v[2] * scale;
		r.v[3] = v.v[3] * scale;

		return r;
	}

	Vector3f MultVectorMatrix(Vector3f v, Matrix3f mt)
	{
		Vector3f r;

		r.v[0] = v.v[0] * mt.m[0] + v.v[1] * mt.m[1] + v.v[2] * mt.m[2];
		r.v[1] = v.v[0] * mt.m[3] + v.v[1] * mt.m[4] + v.v[2] * mt.m[5];
		r.v[2] = v.v[0] * mt.m[6] + v.v[1] * mt.m[7] + v.v[2] * mt.m[8];

		return r;
	}

	Vector4f MultVectorMatrix(Vector4f v, Matrix4f mt)
	{
		Vector4f r;

		r.v[0] = v.v[0] * mt.m[0] + v.v[1] * mt.m[1] + v.v[2] * mt.m[2] + v.v[3] * mt.m[3];
		r.v[1] = v.v[0] * mt.m[4] + v.v[1] * mt.m[5] + v.v[2] * mt.m[6] + v.v[3] * mt.m[7];
		r.v[2] = v.v[0] * mt.m[8] + v.v[1] * mt.m[9] + v.v[2] * mt.m[10] + v.v[3] * mt.m[11];
		r.v[3] = v.v[0] * mt.m[12] + v.v[1] * mt.m[13] + v.v[2] * mt.m[14] + v.v[3] * mt.m[15];

		return r;
	}

	Vector4f MultMatrixVector(Matrix4f mt, Vector4f v)
	{
		Vector4f r;

		r.v[0] = v.v[0] * mt.m[0] + v.v[1] * mt.m[4] + v.v[2] * mt.m[8] + v.v[3] * mt.m[12];
		r.v[1] = v.v[0] * mt.m[1] + v.v[1] * mt.m[5] + v.v[2] * mt.m[9] + v.v[3] * mt.m[13];
		r.v[2] = v.v[0] * mt.m[2] + v.v[1] * mt.m[6] + v.v[2] * mt.m[10] + v.v[3] * mt.m[14];
		r.v[3] = v.v[0] * mt.m[3] + v.v[1] * mt.m[7] + v.v[2] * mt.m[11] + v.v[3] * mt.m[15];

		return r;
	}

	Vector4f slerp(const Vector4f& q1, const Vector4f& q2, float t)
	{
		const float epsilon = 1e-6f;

		// Нормализация входных кватернионов
		Vector4f q1_norm = q1.normalized();
		Vector4f q2_norm = q2.normalized();

		float cosTheta = q1_norm.dot(q2_norm);

		// Если q1 и q2 близки к противоположным направлениям, корректируем q2
		Vector4f q2_adjusted = q2_norm;
		if (cosTheta < 0.0f) {
			q2_adjusted.v[0] = -q2_adjusted.v[0];
			q2_adjusted.v[1] = -q2_adjusted.v[1];
			q2_adjusted.v[2] = -q2_adjusted.v[2];
			q2_adjusted.v[3] = -q2_adjusted.v[3];
			cosTheta = -cosTheta;
		}

		// Если кватернионы близки, используем линейную интерполяцию
		if (cosTheta > 1.0f - epsilon) {
			Vector4f result;

			result.v[0] = q1_norm.v[0] + t * (q2_adjusted.v[0] - q1_norm.v[0]);
			result.v[1] = q1_norm.v[1] + t * (q2_adjusted.v[1] - q1_norm.v[1]);
			result.v[2] = q1_norm.v[2] + t * (q2_adjusted.v[2] - q1_norm.v[2]);
			result.v[3] = q1_norm.v[3] + t * (q2_adjusted.v[3] - q1_norm.v[3]);

			return result.normalized();
		}

		// Иначе используем сферическую интерполяцию
		float theta = std::acos(cosTheta);
		float sinTheta = std::sin(theta);

		float coeff1 = std::sin((1.0f - t) * theta) / sinTheta;
		float coeff2 = std::sin(t * theta) / sinTheta;

		Vector4f result;

		result.v[0] = coeff1 * q1_norm.v[0] + coeff2 * q2_adjusted.v[0];
		result.v[1] = coeff1 * q1_norm.v[1] + coeff2 * q2_adjusted.v[1];
		result.v[2] = coeff1 * q1_norm.v[2] + coeff2 * q2_adjusted.v[2];
		result.v[3] = coeff1 * q1_norm.v[3] + coeff2 * q2_adjusted.v[3];

		return result.normalized();
	}

	Matrix4f MakeMatrix4x4(const Matrix3f& m, const Vector3f pos)
	{
		Matrix4f r;

		r.m[0] = m.m[0];
		r.m[1] = m.m[1];
		r.m[2] = m.m[2];
		r.m[3] = 0;

		r.m[4] = m.m[3];
		r.m[5] = m.m[4];
		r.m[6] = m.m[5];
		r.m[7] = 0;

		r.m[8] = m.m[6];
		r.m[9] = m.m[7];
		r.m[10] = m.m[8];
		r.m[11] = 0;

		r.m[12] = pos.v[0];
		r.m[13] = pos.v[1];
		r.m[14] = pos.v[2];
		r.m[15] = 1.0;
		return r;
	}


};
