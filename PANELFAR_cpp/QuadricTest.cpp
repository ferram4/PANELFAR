#include "stdafx.h"
#include "PANELFARQuadric-body.hpp"
#include <gtest\gtest.h>
#include <iostream>

namespace panelfar_cpp
{
	TEST(QuadricTest, HandlesGetFunctions)
	{
		Quadric Q0;

		Q0 = Quadric(2, 3, 4, 5, 2);

		EXPECT_EQ(Matrix3(4, 6, 8,
						6, 9, 12,
						8, 12, 16), Q0.Tensor());

		EXPECT_EQ(Vector3(10, 15, 20), Q0.Vector());

		EXPECT_EQ(25, Q0.Offset());

		EXPECT_EQ(2, Q0.GetArea());
	};
	TEST(QuadricTest, HandlesBinaryOperations)
	{
		Quadric Q0, Q1;

		Q0 = Quadric(2, 3, 4, 5, 2);
		Q1 = Quadric(1, 1, 1, 1, 1);

		//Addition
		EXPECT_EQ(Matrix3(5, 7, 9,
			7, 10, 13,
			9, 13, 17), (Q0 + Q1).Tensor());

		EXPECT_EQ(Vector3(11, 16, 21), (Q0 + Q1).Vector());

		EXPECT_EQ(26, (Q0 + Q1).Offset());

		EXPECT_EQ(3, (Q0 + Q1).GetArea());

		//Subtraction
		EXPECT_EQ(Matrix3(3, 5, 7,
			5, 8, 11,
			7, 11, 15), (Q0 - Q1).Tensor());

		EXPECT_EQ(Vector3(9, 14, 19), (Q0 - Q1).Vector());

		EXPECT_EQ(24, (Q0 - Q1).Offset());

		EXPECT_EQ(1, (Q0 - Q1).GetArea());

		//Scaling
		EXPECT_EQ(Matrix3(8, 12, 16,
			12, 18, 24,
			16, 24, 32), (Q0 * 2).Tensor());

		EXPECT_EQ(Vector3(20, 30, 40), (Q0 * 2).Vector());

		EXPECT_EQ(50, (Q0 * 2).Offset());

		EXPECT_EQ(2, (Q0 * 2).GetArea());
	};

	TEST(QuadricTest, HandlesUnaryOperations)
	{
		Quadric Q0, Q1;

		Q0 = Quadric(2, 3, 4, 5, 2);
		Q1 = Quadric(1, 1, 1, 1, 1);

		Q0 += Q1;

		//Addition
		EXPECT_EQ(Matrix3(5, 7, 9,
			7, 10, 13,
			9, 13, 17), Q0.Tensor());

		EXPECT_EQ(Vector3(11, 16, 21), Q0.Vector());

		EXPECT_EQ(26, Q0.Offset());

		EXPECT_EQ(3, Q0.GetArea());

		Q0 = Quadric(2, 3, 4, 5, 2);

		Q0 -= Q1;

		//Subtraction
		EXPECT_EQ(Matrix3(3, 5, 7,
			5, 8, 11,
			7, 11, 15), Q0.Tensor());

		EXPECT_EQ(Vector3(9, 14, 19), Q0.Vector());

		EXPECT_EQ(24, Q0.Offset());

		EXPECT_EQ(1, Q0.GetArea());

		Q0 = Quadric(2, 3, 4, 5, 2);

		Q0 *= 2;
		//Scaling
		EXPECT_EQ(Matrix3(8, 12, 16,
			12, 18, 24,
			16, 24, 32), Q0.Tensor());

		EXPECT_EQ(Vector3(20, 30, 40), Q0.Vector());

		EXPECT_EQ(50, Q0.Offset());

		EXPECT_EQ(2, Q0.GetArea());
	};
	TEST(QuadricTest, HandlesFunctions)
	{
		Quadric Q0;

		Q0 = Quadric(2, 3, 4, 5, 2);

		EXPECT_EQ(196, Q0.Evaluate(1, 1, 1));

		Vector3 v = Vector3(1, 1, 1);

		EXPECT_FALSE(Q0.Optimize(v, 1e-12));
	}
}