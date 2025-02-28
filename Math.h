#pragma once

#include <array>
#include <exception>
#include <stdexcept>
#include <cmath>

namespace ZL {

	struct Vector4f
	{
		std::array<float, 4> v = { 0.f, 0.f, 0.f, 0.f };

		Vector4f normalized() const {
			double norm = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
			Vector4f r;

			r.v[0] = v[0] / norm;
			r.v[1] = v[1] / norm;
			r.v[2] = v[2] / norm;
			r.v[3] = v[3] / norm;

			return r;
		}

		double dot(const Vector4f& other) const {
			return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2] + v[3] * other.v[3];
		}
	};

	struct Vector3f
	{
		std::array<float, 3> v = { 0.f, 0.f, 0.f };
	};

	struct Vector2f
	{
		std::array<float, 2> v = {0.f, 0.f};
	};

	Vector2f operator+(const Vector2f& x, const Vector2f& y);

	Vector2f operator-(const Vector2f& x, const Vector2f& y);
	
	Vector3f operator+(const Vector3f& x, const Vector3f& y);

	Vector3f operator-(const Vector3f& x, const Vector3f& y);
	Vector4f operator+(const Vector4f& x, const Vector4f& y);

	Vector4f operator-(const Vector4f& x, const Vector4f& y);


	struct Matrix3f
	{
		std::array<float, 9> m = {	0.f, 0.f, 0.f, 
									0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, };

		static Matrix3f Identity();
	};

	struct Matrix4f
	{
		std::array<float, 16> m = { 0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f };

		static Matrix4f Identity();

		float& operator()(int row, int col) {
			//return m[row * 4 + col]; //OpenGL specific
			return m[col * 4 + row];
		}

		const float& operator()(int row, int col) const {
			//return m[row * 4 + col];
			return m[col * 4 + row];
		}
	};

	Matrix4f operator*(const Matrix4f& m1, const Matrix4f& m2);

	Matrix4f MakeOrthoMatrix(float width, float height, float zNear, float zFar);

	Matrix4f MakePerspectiveMatrix(float fovY, float aspectRatio, float zNear, float zFar);

	Matrix3f QuatToMatrix(const Vector4f& q);

	Vector4f MatrixToQuat(const Matrix3f& m);

	Vector4f QuatFromRotateAroundX(float angle);
	Vector4f QuatFromRotateAroundY(float angle);
	Vector4f QuatFromRotateAroundZ(float angle);

	Vector3f operator*(Vector3f v, float scale);
	Vector4f operator*(Vector4f v, float scale);

	Vector3f MultVectorMatrix(Vector3f v, Matrix3f mt);
	Vector4f MultVectorMatrix(Vector4f v, Matrix4f mt);
	Vector4f MultMatrixVector(Matrix4f mt, Vector4f v);

	Vector4f slerp(const Vector4f& q1, const Vector4f& q2, double t);
	Matrix3f InverseMatrix(const Matrix3f& m);
	Matrix4f InverseMatrix(const Matrix4f& m);
	Matrix3f MultMatrixMatrix(const Matrix3f& m1, const Matrix3f& m2);
	Matrix4f MultMatrixMatrix(const Matrix4f& m1, const Matrix4f& m2);
	Matrix4f MakeMatrix4x4(const Matrix3f& m, const Vector3f pos);

};