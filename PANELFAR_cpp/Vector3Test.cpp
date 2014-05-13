#include "stdafx.h"
#include "PANELFARVector3-body.hpp"
#include <gtest\gtest.h>

namespace panelfar_cpp
{
	TEST(Vector3Test, HandlesMagnitudes)
	{
		Vector3 v0;
		v0 = Vector3(2, 0, 0);

		EXPECT_EQ(2, v0.Magnitude());
		EXPECT_EQ(4, v0.SqrMagnitude());
	};
	TEST(Vector3Test, HandlesOperators)
	{
		Vector3 v0, v1, v2;
		v0 = Vector3(1, 0, 0);
		v1 = Vector3(1, 0, 0);

		EXPECT_TRUE(v0 == v1);
		EXPECT_FALSE(v0 != v1);

		v0 = Vector3(2, 7, -5);
		v1 = Vector3(0, 1, 3);

		EXPECT_TRUE(v0 != v1);
		EXPECT_FALSE(v0 == v1);

		v2 = v0 + v1;
		EXPECT_EQ(Vector3(2, 8, -2), v2);
		v2 = v0 - v1;
		EXPECT_EQ(Vector3(2, 6, -8), v2);

		v0 += v1;
		EXPECT_EQ(Vector3(2, 8, -2), v0);

		v0 = Vector3(2, 7, -5);
		v0 -= v1;
		EXPECT_EQ(Vector3(2, 6, -8), v0);

		v0 = Vector3(2, 7, -5);

		v0 *= 5;
		v1 = Vector3(10, 35, -25);

		EXPECT_EQ(v1, v0);

		v0 /= 10;
		v1 = Vector3(1, 3.5, -2.5);

		EXPECT_EQ(v1, v0);

		v0 = Vector3(2, 7, -5);
		v1 = Vector3(0, 1, 3);


	};
	TEST(Vector3Test, HandlesDotAndCrossProducts)
	{
		Vector3 v0, v1;
		v0 = Vector3(1, 0, 0);
		v1 = Vector3(1, 0, 0);

		EXPECT_EQ(1, Vector3::Dot(v0, v1));
		EXPECT_EQ(0, Vector3::Cross(v0, v1).Magnitude());
		v1 = Vector3(0, 1, 0);
		EXPECT_EQ(Vector3(0, 0, 1), Vector3::Cross(v0, v1));
	};
}