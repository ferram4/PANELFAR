// Quadric Error Metric

#pragma once

#include "PANELFARQuadric.hpp"

namespace panelfar_cpp
{
	namespace mesh_simplification
	{

		inline Quadric::Quadric() : Quadric(0, 0, 0, 0, 0){}
		inline Quadric::Quadric(double const& a, double const& b, double const& c, double const& d, double const& r)
		{
			a2 = a * a;	ab = a * b;	ac = a * c;	ad = a * d;
			b2 = b * b;	bc = b * c; bd = b * d;
			c2 = c * c;	cd = c * d;
			d2 = d * d;

			area = r;
		}

		inline Matrix3 Quadric::Tensor()
		{
			return Matrix3(a2, ab, ac,
				ab, b2, bc,
				ac, bc, c2);
		}
		inline Vector3 Quadric::Vector()
		{
			return Vector3(ad, bd, cd);
		}
		inline double Quadric::Offset()
		{
			return d2;
		}

		inline double Quadric::GetArea()
		{
			return area;
		}

		inline double Quadric::Evaluate(Vector3 const& v)
		{
			return Evaluate(v.x, v.y, v.z);
		}
		inline double Quadric::Evaluate(double const& x, double const& y, double const& z)
		{
			return x * x * a2
				+ 2 * x * y * ab
				+ 2 * x * z * ac
				+ 2 * x * ad
				+ y * y * b2
				+ 2 * y * z * bc
				+ 2 * y * bd
				+ z * z * c2
				+ 2 * z * cd
				+ d2;
		}

		inline bool Quadric::Optimize(Vector3 &v, double epsilon)
		{
			Matrix3 invMat = Tensor();
			if (!Matrix3::Inverse(invMat, epsilon))
				return false;

			v = invMat * Vector() * -1;
			return true;
		}

		inline void Quadric::operator+=(Quadric const& Q1)
		{
			area += Q1.area;

			a2 += Q1.a2;	ab += Q1.ab;	ac += Q1.ac;	ad += Q1.ad;
			b2 += Q1.b2;	bc += Q1.bc;	bd += Q1.bd;
			c2 += Q1.c2;	cd += Q1.cd;
			d2 += Q1.d2;
		}
		inline void Quadric::operator -= (Quadric const& Q1)
		{
			area -= Q1.area;

			a2 -= Q1.a2;	ab -= Q1.ab;	ac -= Q1.ac;	ad -= Q1.ad;
			b2 -= Q1.b2;	bc -= Q1.bc;	bd -= Q1.bd;
			c2 -= Q1.c2;	cd -= Q1.cd;
			d2 -= Q1.d2;
		}
		inline void Quadric::operator*=(double const& s)
		{
			a2 *= s;	ab *= s;	ac *= s;	ad *= s;
			b2 *= s;	bc *= s;	bd *= s;
			c2 *= s;	cd *= s;
			d2 *= s;
		}


		inline Quadric operator+(Quadric const& Q0, Quadric const& Q1)
		{
			Quadric Q = Q0;
			Q += Q1;
			return Q;
		}
		inline Quadric operator-(Quadric const& Q0, Quadric const& Q1)
		{
			Quadric Q = Q0;
			Q -= Q1;
			return Q;
		}
		inline Quadric operator*(Quadric const& Q0, double const& s)
		{
			Quadric Q = Q0;
			Q *= s;
			return Q;
		}
	}
}