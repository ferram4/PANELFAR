/*Implementation of 3 item Vector using doubles*/
#pragma once;
#include <math.h>

namespace panelfar_cpp
{
	public class Vector3
	{

	public:
		double x, y, z;
		inline Vector3();
		inline Vector3(double const& a, double const& b, double const& c);

		inline double Magnitude();

		inline double SqrMagnitude();

		inline void Normalize();
		inline Vector3 normalized();

		inline static double Dot(Vector3 const& v0, Vector3 const& v1);
		inline static Vector3 Cross(Vector3 const& v0, Vector3 const& v1);

		inline void operator+=(Vector3 const& v1);
		inline void operator-=(Vector3 const& v1);
		inline void operator*=(double const& s);
		inline void operator/=(double const& s);

		inline double& operator[](int i)       { if (i == 0) return x; else if (i == 1) return y; else return z; }
		inline double  operator[](int i) const { if (i == 0) return x; else if (i == 1) return y; else return z; }
	};

	inline Vector3 operator+(Vector3 const& v0, Vector3 const& v1);
	inline Vector3 operator-(Vector3 const& v0, Vector3 const& v1);
	inline Vector3 operator*(Vector3 const& v0, double const& s);
	inline Vector3 operator/(Vector3 const& v0, double const& s);
	inline bool operator==(Vector3 const& v0, Vector3 const& v1);
	inline bool operator!=(Vector3 const& v0, Vector3 const& v1);
}