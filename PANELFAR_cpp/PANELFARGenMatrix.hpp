/*Implementation of a general Matrix*/

#pragma once;
#include "PANELFARVector3-body.hpp"
#include <vector>

namespace panelfar_cpp
{
	public class Matrix
	{
	private:
		std::vector<std::vector<double>> Aij;		//Array that holds all of the values; a single dimensional array that is treated as a two dimensional array by SetValue and GetValue

	public:
		unsigned int m, n;

		inline Matrix(unsigned int const& m, unsigned int const& n);

		inline Matrix Transpose();

		inline void Invert();
		inline Matrix Inverse();

		inline Matrix Solve(Matrix const& RHS);

		inline double& operator () (unsigned int i, unsigned int j);
		inline double operator () (unsigned int i, unsigned int j) const;

		inline void operator += (Matrix const& M);
		inline void operator -= (Matrix const& M);

		inline void operator *= (double const& s);
		
	private:
		inline void PartialPivoting(Matrix &L, Matrix & U);

		inline void UpperSubstitution(Matrix &U, Matrix &RHS);
		inline void LowerSubstitution(Matrix &U, Matrix &RHS);
	};

	inline Matrix operator + (Matrix const& M0, Matrix const& M1);
	inline Matrix operator - (Matrix const& M0, Matrix const& M1);
	inline Matrix operator * (Matrix const& M0, Matrix const& M1);
	inline Matrix operator * (Matrix const& M0, double const& s);
}