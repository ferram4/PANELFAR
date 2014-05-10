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
		inline Matrix3(Vector3 const& v0,
						Vector3 const& v1,
						Vector3 const& v2);
		inline Matrix3(double const& m00, double const& m01, double const& m02,
						double const& m10, double const& m11, double const& m12,
						double const& m20, double const& m21, double const& m22);

		inline double Determinant();

		inline Matrix3 Transpose();
		inline Matrix3 CofactorMatrix();

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