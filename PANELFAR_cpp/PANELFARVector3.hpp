/*Implementation of 3 item Vector using doubles*/
#pragma once;
#include <math.h>

namespace panelfar_cpp
{
	public struct PANELFARVector3
	{

	public:
		double x, y, z;
		inline PANELFARVector3()
		{
			PANELFARVector3(0, 0, 0);
		}
		inline PANELFARVector3(double const& a, double const& b, double const& c)
		{
			x = a;
			y = b;
			z = c;
		}

		inline double Magnitude()
		{
			return sqrt(SqrMagnitude());
			
		}

		inline double SqrMagnitude()
		{
			return x * x
				+ y * y
				+ z * z;
		}

		inline void Normalize()
		{
			double invMag = 1 / Magnitude();
			x *= invMag;
			y *= invMag;
			z *= invMag;
		}
		inline PANELFARVector3 normalized()
		{
			PANELFARVector3 v = PANELFARVector3(x, y, z);
			v.Normalize();
			return v;
		}

		inline double Dot(PANELFARVector3 const& v0, PANELFARVector3 const& v1)
		{
			double dot = v0.x * v1.x
				+ v0.y * v1.y
				+ v0.z * v1.z;
			return dot;
		}
		inline PANELFARVector3 Cross(PANELFARVector3 const& v0, PANELFARVector3 const& v1)
		{
			double a, b, c;
			a = v0.y * v1.z - v0.z * v1.y;
			b = v0.z * v1.x - v0.x * v1.z;
			c = v0.x * v1.y - v0.y * v1.x;

			PANELFARVector3 cross = PANELFARVector3(a,b,c);
			return cross;
		}

		inline PANELFARVector3 operator*(PANELFARVector3 const& v0, double const& s)
		{
			return PANELFARVector3(v0.x * s,
				v0.y * s,
				v0.z * s);
		}
	};

}