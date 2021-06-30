#include "Physics.h"

namespace ZL
{


	bool TestIntersection(const LinePhysicsObject& line, const EllipsePhysicsObject& ellipse)
	{

		// x*x / (a*a) + y*y / (b*b) = 1

		// x = x0 + (x1 - x0) * t = x0 + dx * t

		// y = y0 + (y1 - y0) * t = y0 + dy * t

		//^ solve above for t, and make sure 0 > t > 1

		float x0 = line.start.v[0] - ellipse.center.v[0];

		float y0 = line.start.v[1] - ellipse.center.v[1];

		float dx = line.end.v[0] - line.start.v[0];

		float dy = line.end.v[1] - line.start.v[1];


		static const float EPS = 0.0001f;  //Must very close to 0 to handle extreme situations


		float a = dx * dx * ellipse.b * ellipse.b + dy * dy * ellipse.a * ellipse.a;

		float b = 2.f * (x0 * dx * ellipse.b * ellipse.b + y0 * dy * ellipse.a * ellipse.a);

		float c = ellipse.b * ellipse.b * x0 * x0 + ellipse.a * ellipse.a * y0 * y0 - ellipse.a * ellipse.a * ellipse.b * ellipse.b;

		if (abs(a) < EPS) //Here consider a = 0 and equation becomes linear
		{
			float t = -c / b;

			if ((t > 0) && (t < 1.f))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			float D2 = b * b - 4 * a * c;

			if (D2 < 0)
			{
				return false;
			}
			else
			{
				float D = sqrtf(D2);

				float t1 = (-b - D) / (2 * a);

				float t2 = (-b + D) / (2 * a);

				if (((t1 > 0) && (t1 < 1.f)) || ((t2 > 0) && (t2 < 1.f)))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

}