/*Implementation of a general Matrix*/

#pragma once;
#include "PANELFARGenMatrix.hpp"

namespace panelfar_cpp
{

	inline Matrix::Matrix(unsigned int const& m, unsigned int const& n)
	{
		Aij = std::vector<std::vector<double>>();
		for (int i = 0; i < m; i++)
		{
			std::vector<double> vec = std::vector<double>();
			for (int j = 0; j < n; j++)
				vec.push_back(0);

			Aij.push_back(vec);
		}

		Matrix::m = m;
		Matrix::n = n;
	}

	inline double& Matrix::operator () (unsigned int i, unsigned int j)
	{
		return Aij[i][j];
	}

	inline double Matrix::operator () (unsigned int i, unsigned int j) const
	{
		return Aij[i][j];
	}
	
	inline Matrix Matrix::Transpose()
	{
		double tmp;

		for (unsigned int i = 0; i < m; i++)
		{
			for (unsigned int j = 0; j < n; j++)
			{
				tmp = operator()(i, j);
				operator()(i, j) = operator()(j, i);
				operator()(j, i) = tmp;
			}
		}

		//Switch width and height
		tmp = m;
		m = n;
		n = tmp;
	}

	inline void Matrix::Invert()
	{
		//TODO
	}
	inline Matrix Matrix::Inverse()
	{
		//TODO
		return *this;
	}

	inline void Matrix::PartialPivoting(Matrix &L, Matrix &U)
	{
		Matrix Pi_k = Matrix(U.n, U.n);
		for (int i = 0; i < U.n; i++)
			Pi_k(i, i) = 1;

		for (int k = 0; k < U.n; k++)
		{
			//Search for i in {k, ..., n} such that abs(A_ik) >= abs(A_jk) for all j in {k, ..., n}
			int i = k;
			double tmp = abs(U(k, k));
			for (int j = k; j < U.n; j++)
			{
				double tmp2 = abs(U(j, k));
				if (tmp2 >= tmp)
				{
					tmp = tmp2;
					i = j;
				}
			}

			//Define Pi_k, the permutation that swaps i and k
			Pi_k(i, i) = 0;	//Set the diagonals to zero
			Pi_k(k, k) = 0;
			Pi_k(k, i) = 1;	//Set the rows to swap
			Pi_k(i, k) = 1;

			U = Pi_k * U;

			Pi_k(i, i) = 1;	//Reset the diagonals to one
			Pi_k(k, k) = 1;
			Pi_k(k, i) = 0;	//Reset the rows to prevent more swaps
			Pi_k(i, k) = 0;

			//Determine Elements of Matrix L

			for (int i = k + 1; i < U.n; i++)
			{
				L(i, k) = U(i, k) / U(k, k);
			}

			//Multiply out
			U = L * U;
		}

		return;	//Return L and U from partial pivoting method
	}

	inline void Matrix::LowerSubstitution(Matrix &L, Matrix &RHS)
	{
		double tmp = 0;
		for (int i = 0; i < L.n; i++)
		{
			tmp = 0;
			for (int k = 0; k < i - 1; k++)
			{
				tmp += L(i, k) * RHS(i, 1);
			}
			RHS(i, 1) = (RHS(i, 1) - tmp) / L(i, i);
		}
	}


	inline void Matrix::UpperSubstitution(Matrix &U, Matrix &RHS)
	{
		double tmp = 0;
		for (int i = U.n - 1; i >= 0; i--)
		{
			tmp = 0;
			for (int k = i + 1; k <= n; k++)
			{
				tmp += U(i, k) * RHS(i, 1);
			}
			RHS(i, 1) = (RHS(i, 1) - tmp) / U(i, i);
		}
	}

	inline Matrix Matrix::Solve(Matrix const& RHS)
	{
		Matrix L = Matrix(m, m);
		Matrix U = Matrix(*this);
		Matrix soln = Matrix(RHS);

		PartialPivoting(L, U);
		LowerSubstitution(L, soln);
		UpperSubstitution(R, soln);

		return soln;
	}

	inline void Matrix::operator += (Matrix const& M)
	{
		if (m == M.m && n == M.n)
		{
			for (unsigned int i = 0; i < m; i++)
			{
				for (unsigned int j = 0; j < n; j++)
				{
					operator()(i, j) += M(i, j);
				}
			}
		}
		else
			throw "Invalid Matrix Sizes";
	}
	inline void Matrix::operator -= (Matrix const& M)
	{
		if (m == M.m && n == M.n)
		{
			for (unsigned int i = 0; i < m; i++)
			{
				for (unsigned int j = 0; j < n; j++)
				{
					operator()(i, j) -= M(i, j);
				}
			}
		}
		else
			throw "Invalid Matrix Sizes";
	}


	inline void Matrix::operator *= (double const& s)
	{
		for (unsigned int i = 0; i < m; i++)
		{
			for (unsigned int j = 0; j < n; j++)
			{
				operator()(i, j) *= s;
			}
		}

	}

	inline Matrix operator + (Matrix const& M0, Matrix const& M1)
	{
		Matrix M = M0;
		M += M1;
		return M;
	}
	inline Matrix operator - (Matrix const& M0, Matrix const& M1)
	{
		Matrix M = M0;
		M -= M1;
		return M;
	}
	inline Matrix operator * (Matrix const& M0, double const& s)
	{
		Matrix M = M0;
		M *= s;
		return M;
	}
	inline Matrix operator * (Matrix const& M0, Matrix const& M1)
	{
		if (M0.n == M1.m)
		{
			Matrix M = Matrix(M0.m, M1.n);
			for (unsigned int i = 0; i < M.m; i++)
			{
				for (unsigned int j = 0; j < M.n; j++)
				{
					M(i, j) = 0;
					for (unsigned int k = 0; k < M0.n; k++)
					{
						M(i, j) += M0(i, k) * M1(k, j);
					}
				}
			}
			return M;
		}
		else
			throw "Matrix size mismatch; cannot multiply matrices";
	}
}