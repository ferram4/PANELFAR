/*Implementation of a 3x3 Matrix*/

#pragma once;
#include "PANELFARMatrix3.hpp"

namespace panelfar_cpp
{
	inline Matrix3::Matrix3()
	{
		Matrix3(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
	inline Matrix3::Matrix3(Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Matrix3(v0.x, v0.y, v0.z,
			v1.x, v1.y, v1.z,
			v2.x, v2.y, v2.z);
	}
	inline Matrix3::Matrix3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22)
	{
		a00 = m00; a01 = m01; a02 = m02;
		a10 = m10; a11 = m11; a12 = m12;
		a20 = m20; a21 = m21; a22 = m22;
	}

	inline double Matrix3::Determinant()
	{
		return a00 * (a11 * a22 - a12 * a21)
			- a01 * (a10 * a22 - a12 * a20)
			+ a02 * (a10 * a21 - a11 * a20);
	}

	inline Matrix3 Matrix3::Transpose()
	{
		return Matrix3(a00, a10, a20,
			a01, a11, a21,
			a02, a12, a22);
	}

	inline bool Matrix3::Inverse(Matrix3 &M, double const& margin)
	{
		double det = M.Determinant();
		if (abs(det - 0) < margin)
			return false;

		M = M.Transpose() / det;
		return true;
	}

	inline Matrix3 Matrix3::Identity()
	{
		return Matrix3(1, 0, 0,
			0, 1, 0,
			0, 0, 1);
	}

	inline Matrix3 operator +(Matrix3 const& M0, Matrix3 const& M1)
	{
		return Matrix3(M0.a00 + M1.a00, M0.a01 + M1.a01, M0.a02 + M1.a02,
			M0.a10 + M1.a10, M0.a11 + M1.a11, M0.a12 + M1.a12,
			M0.a20 + M1.a20, M0.a21 + M1.a21, M0.a22 + M1.a22);
	}
	inline Matrix3 operator -(Matrix3 const& M0, Matrix3 const& M1)
	{
		return Matrix3(M0.a00 - M1.a00, M0.a01 - M1.a01, M0.a02 - M1.a02,
			M0.a10 - M1.a10, M0.a11 - M1.a11, M0.a12 - M1.a12,
			M0.a20 - M1.a20, M0.a21 - M1.a21, M0.a22 - M1.a22);
	}

	//Scaling operations
	inline Matrix3 operator *(Matrix3 const& M, double const& s)
	{
		return Matrix3(M.a00 * s, M.a01 * s, M.a02 * s,
			M.a10 * s, M.a11 * s, M.a12 * s,
			M.a20 * s, M.a21 * s, M.a22 * s);
	}
	inline Matrix3 operator *(double const& s, Matrix3 const& M)
	{
		return (M * s);
	}
	inline Matrix3 operator /(Matrix3 const& M, double const& s)
	{
		return (M * (1 / s));
	}

	//Matrix multiplication
	inline Vector3 operator *(Matrix3 const& M, Vector3 const& v)
	{
		return Vector3(M.a00 * v.x + M.a01 * v.y + M.a02 * v.z,
			M.a10 * v.x + M.a11 * v.y + M.a12 * v.z,
			M.a20 * v.x + M.a21 * v.y + M.a22 * v.z);
	}
	inline Matrix3 operator *(Matrix3 const& M0, Matrix3 const& M1)
	{
		return Matrix3(Vector3(M0.a00 * M1.a00 + M0.a01 * M1.a10 + M0.a02 * M1.a20, M0.a00 * M1.a01 + M0.a01 * M1.a11 + M0.a02 * M1.a21, M0.a00 * M1.a02 + M0.a01 * M1.a12 + M0.a02 * M1.a22),
			Vector3(M0.a10 * M1.a00 + M0.a11 * M1.a10 + M0.a12 * M1.a20, M0.a10 * M1.a01 + M0.a11 * M1.a11 + M0.a12 * M1.a21, M0.a10 * M1.a02 + M0.a11 * M1.a12 + M0.a12 * M1.a22),
			Vector3(M0.a20 * M1.a00 + M0.a21 * M1.a10 + M0.a22 * M1.a20, M0.a20 * M1.a01 + M0.a21 * M1.a11 + M0.a22 * M1.a21, M0.a20 * M1.a02 + M0.a21 * M1.a12 + M0.a22 * M1.a22));
	}

	inline bool operator ==(Matrix3 const& M0, Matrix3 const& M1)
	{
		return (M0.a00 == M1.a00 && M0.a01 == M1.a01 && M0.a02 == M1.a02
			&& M0.a10 == M1.a10 && M0.a11 == M1.a11 && M0.a12 == M1.a12
			&& M0.a20 == M1.a20 && M0.a21 == M1.a21 && M0.a22 == M1.a22);
	}
	inline bool operator !=(Matrix3 const& M0, Matrix3 const& M1)
	{
		return !(M0 == M1);
	}

}