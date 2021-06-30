#pragma once
#include "Math.h"

namespace ZL
{
	struct EllipsePhysicsObject
	{
		Vector2f center = {0.f, 0.f};

		float a = 0.f;
		float b = 0.f;
	};

	struct LinePhysicsObject
	{
		Vector2f start = { 0.f, 0.f };
		Vector2f end = { 0.f, 0.f };
	};

	bool TestIntersection(const LinePhysicsObject& line, const EllipsePhysicsObject& ellipse);

}