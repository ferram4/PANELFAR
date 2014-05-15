// Quadric Error Metric

#pragma once

#include "PANELFARMath.hpp"

namespace panelfar_cpp
{
	namespace mesh_simplification
	{

		public class Quadric
		{
		private:
			double	a2, ab, ac, ad,
				b2, bc, bd,
				c2, cd,
				d2;

			double area;

		public:

			inline Quadric();
			inline Quadric(double const& a, double const& b, double const& c, double const& d, double const& r);

			inline Matrix3 Tensor();
			inline Vector3 Vector();
			inline double Offset();
			inline double GetArea();

			inline double Evaluate(Vector3 const& v);
			inline double Evaluate(double const& x, double const& y, double const& z);

			inline bool Optimize(Vector3 &v, double epsilon);

			inline void operator+=(Quadric const& Q1);
			inline void operator-=(Quadric const& Q1);
			inline void operator*=(double const& s);
		};

		inline Quadric operator+(Quadric const& Q0, Quadric const& Q1);
		inline Quadric operator-(Quadric const& Q0, Quadric const& Q1);
		inline Quadric operator*(Quadric const& Q0, double const& s);
	}
}