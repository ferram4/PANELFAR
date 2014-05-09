/*Implementation of a 3x3 Matrix*/

#pragma once;
#include "PANELFARVector3.hpp"

namespace panelfar_cpp
{
	public struct Matrix3
	{
	public:
		double a00, a01, a02,
			a10, a11, a12,
			a20, a21, a22;

		inline Matrix3();
		inline Matrix3(Vector3 v0, Vector3 v1, Vector3 v2);
		inline Matrix3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22);

		inline double Determinant();

		inline Matrix3 Transpose();

		inline static bool Inverse(Matrix3 &M, double const& margin);

		inline static Matrix3 Identity();
	};
	inline Matrix3 operator +(Matrix3 const& M0, Matrix3 const& M1);
	inline Matrix3 operator -(Matrix3 const& M0, Matrix3 const& M1);

	inline Matrix3 operator *(Matrix3 const& M, double const& s);
	inline Matrix3 operator *(double const& s, Matrix3 const& M);
	inline Matrix3 operator /(Matrix3 const& M, double const& s);

	inline Vector3 operator *(Matrix3 const& M, Vector3 const& v);
	inline Matrix3 operator *(Matrix3 const& M0, Matrix3 const& M1);

	inline bool operator ==(Matrix3 const& M0, Matrix3 const& M1);
	inline bool operator !=(Matrix3 const& M0, Matrix3 const& M1);
}