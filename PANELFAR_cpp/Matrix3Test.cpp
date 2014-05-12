#include "stdafx.h"
#include "PANELFARMatrix3-body.hpp"
#include <gtest\gtest.h>
#include <iostream>

namespace panelfar_cpp
{
	TEST(Matrix3Test, HandlesSimpleOperators)
	{
		Matrix3 M0, M1;
		M0 = Matrix3(1, 2, 3,
			4, 5, 6,
			7, 8, 9);

		EXPECT_EQ(Matrix3(1, 2, 3,
			4, 5, 6,
			7, 8, 9), M0);
		
		M1 = Matrix3(1, 1, 1,
			1, 1, 1,
			1, 1, 1);

		EXPECT_EQ(Matrix3(2, 3, 4,
			5, 6, 7,
			8, 9, 10), M0 + M1);

		EXPECT_EQ(Matrix3(0, 1, 2,
			3, 4, 5,
			6, 7, 8), M0 - M1);

		EXPECT_EQ(Matrix3(2, 2, 2,
			2, 2, 2,
			2, 2, 2), M1 * 2);

		EXPECT_EQ(Matrix3(0.5, 0.5, 0.5,
			0.5, 0.5, 0.5,
			0.5, 0.5, 0.5), M1 / 2);
	};

	TEST(Matrix3Test, HandlesMatrixMath)
	{
		Matrix3 M0, M1, M2;

		M0 = Matrix3(1, 2, 3,
			4, 5, 6,
			7, 8, 9);
		EXPECT_EQ(M0, M0 * Matrix3::Identity());

		EXPECT_EQ(Matrix3(1, 4, 7,
			2, 5, 8,
			3, 6, 9), M0.Transpose());

		M1 = Matrix3(Vector3(2, 0, 2),
					Vector3(2, 2, 0),
					Vector3(2, 1, 2));
		M2 = M1;
		EXPECT_EQ(4, M1.Determinant());

		EXPECT_TRUE(Matrix3::Inverse(M2, 1e-5));

		EXPECT_EQ(Matrix3(Vector3(2, 1, -2), Vector3(-2, 0, 2), Vector3(-1, -1, 2)), M2 * 2);

		EXPECT_EQ(Matrix3(12, 7, 8,
			30, 16, 20,
			48, 25, 32), M0 * M1);

		EXPECT_EQ(Matrix3(16,20,24,
			10,14,18,
			20,25,30), M1 * M0);
	}
}