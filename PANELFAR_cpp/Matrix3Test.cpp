#include "stdafx.h"
#include "PANELFARMatrix3-body.hpp"
#include <gtest\gtest.h>

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

		EXPECT_EQ(Matrix3(1 / 2, 1 / 2, 1 / 2,
			1 / 2, 1 / 2, 1 / 2,
			1 / 2, 1 / 2, 1 / 2), M1 / 2);

		EXPECT_EQ(M0, M0 * Matrix3::Identity());

	};
}