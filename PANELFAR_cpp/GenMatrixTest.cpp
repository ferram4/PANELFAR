#include "stdafx.h"
#include "PANELFARGenMatrix-body.hpp"
#include <gtest\gtest.h>
#include <iostream>

namespace panelfar_cpp
{
	TEST(GenMatrixTest, BasicPropertiesAndAssignment)
	{
		Matrix A = Matrix(2, 5);

		EXPECT_EQ(A.m, 2);
		EXPECT_EQ(A.n, 5);

		A(0, 3) = 2;
		A(1, 2) = 5;

		EXPECT_EQ(A(0, 3), 2);
		EXPECT_EQ(A(1, 2), 5);
	}

	TEST(GenMatrixTest, BasicOperations)
	{
		Matrix A = Matrix(2, 5);
		Matrix B = Matrix(2, 5);
		Matrix C = Matrix(2, 5);

		A(0, 3) = 2;
		A(1, 2) = 5;
		B(0, 3) = 1;
		B(1, 2) = -8;
		B(0, 0) = 1;

		C = A + B;

		EXPECT_EQ(C(0, 3), 3);
		EXPECT_EQ(C(1, 2), -3);
		EXPECT_EQ(C(0, 0), 1);

		C = A - B;

		EXPECT_EQ(C(0, 3), 1);
		EXPECT_EQ(C(1, 2), 13);
		EXPECT_EQ(C(0, 0), -1);

		C = C * 2;

		EXPECT_EQ(C(0, 3), 2);
		EXPECT_EQ(C(1, 2), 26);
		EXPECT_EQ(C(0, 0), -2);

		A += B; 

		EXPECT_EQ(A(0, 3), 3);
		EXPECT_EQ(A(1, 2), -3);
		EXPECT_EQ(A(0, 0), 1);

		A -= B;

		EXPECT_EQ(A(0, 3), 2);
		EXPECT_EQ(A(1, 2), 5);
		EXPECT_EQ(A(0, 0), 0);

		A *= 5;

		EXPECT_EQ(A(0, 3), 10);
		EXPECT_EQ(A(1, 2), 25);
		EXPECT_EQ(A(0, 0), 0);

		B = Matrix(2, 2);
		B(0, 0) = 2;
		B(1, 1) = 5;

		C = B * A;

		EXPECT_EQ(C(0, 3), 20);
		EXPECT_EQ(C(1, 2), 125);
		EXPECT_EQ(C(1, 4), 0);
	}
}
