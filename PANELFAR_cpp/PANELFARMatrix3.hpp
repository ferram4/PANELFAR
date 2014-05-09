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

		inline Matrix3()
		{
			Matrix3(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0));
		}
		inline Matrix3(Vector3 v0, Vector3 v1, Vector3 v2)
		{
			Matrix3(v0.x, v0.y, v0.z,
				v1.x, v1.y, v1.z,
				v2.x, v2.y, v2.z);
		}
		inline Matrix3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22)
		{
			a00 = m00; a01 = m01; a02 = m02;
			a10 = m10; a11 = m11; a12 = m12;
			a20 = m20; a21 = m21; a22 = m22;
		}

		inline double Determinant()
		{
			return a00 * (a11 * a22 - a12 * a21)
				- a01 * (a10 * a22 - a12 * a20)
				+ a02 * (a10 * a21 - a11 * a20);
		}

		inline Matrix3 Transpose()
		{
			return Matrix3(a00, a10, a20,
				a01, a11, a21,
				a02, a12, a22);
		}

		inline static bool Inverse(Matrix3 &M, double margin)
		{
			double det = M.Determinant();
			if (abs(det - 0) < margin)
				return false;

			M = M.Transpose() / det;
			return true;
		}
	};
}