#pragma once

#include <array>

namespace ZL {

	struct Vector4f
	{
		std::array<float, 4> v = { 0.f, 0.f, 0.f, 0.f };
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

	struct Matrix3f
	{
		std::array<float, 9> m = {	0.f, 0.f, 0.f, 
									0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, };
	};

	struct Matrix4f
	{
		std::array<float, 16> m = { 0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f,
									0.f, 0.f, 0.f, 0.f };

		static Matrix4f Identity();
	};

	Matrix4f operator*(const Matrix4f& m1, const Matrix4f& m2);

	Matrix4f MakeOrthoMatrix(float width, float height, float zNear, float zFar);

	Matrix3f QuatToMatrix(const Vector4f& q);

	Vector4f QuatFromRotateAroundZ(float angle);

};