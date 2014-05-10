/*Implementation of 3 item Vector using doubles*/
#pragma once;
#include <math.h>

namespace panelfar_cpp
{
	public struct Vector3
	{

	public:
		double x, y, z;
		inline Vector3() : Vector3(0, 0, 0){}

		inline Vector3(double const& a, double const& b, double const& c)
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
		inline Vector3 normalized()
		{
			Vector3 v = Vector3(x, y, z);
			v.Normalize();
			return v;
		}

		inline static double Dot(Vector3 const& v0, Vector3 const& v1)
		{
			double dot = v0.x * v1.x
				+ v0.y * v1.y
				+ v0.z * v1.z;
			return dot;
		}
		inline static Vector3 Cross(Vector3 const& v0, Vector3 const& v1)
		{
			double a, b, c;
			a = v0.y * v1.z - v0.z * v1.y;
			b = v0.z * v1.x - v0.x * v1.z;
			c = v0.x * v1.y - v0.y * v1.x;

			Vector3 cross = Vector3(a, b, c);
			return cross;
		}

		inline void operator*=(double const& s)
		{
			this->x *= s;
			this->y *= s;
			this->z *= s;

		};
		inline void operator/=(double const& s)
		{
			this->operator*=(1 / s);
		};

	};

	inline Vector3 operator+(Vector3 const& v0, Vector3 const& v1)
	{
		return Vector3(v0.x + v1.x,
			v0.y + v1.y,
			v0.z + v1.z);
	};
	inline Vector3 operator-(Vector3 const& v0, Vector3 const& v1)
	{
		return Vector3(v0.x - v1.x,
			v0.y - v1.y,
			v0.z - v1.z);
	};
	inline Vector3 operator*(Vector3 const& v0, double const& s)
	{
		return Vector3(v0.x * s,
			v0.y * s,
			v0.z * s);
	};
	inline Vector3 operator/(Vector3 const& v0, double const& s)
	{
		return v0 * (1 / s);
	};
	inline bool operator==(Vector3 const& v0, Vector3 const& v1)
	{
		if (v0.x == v1.x &&
			v0.y == v1.y &&
			v0.z == v1.z){
			return true;
		}
		return false;
	};
	inline bool operator!=(Vector3 const& v0, Vector3 const& v1)
	{
		return !(v0 == v1);
	};
}