using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace panelfar
{
    public class MatrixSym3x3
    {
        private double m00;
        private double m01, m11;
        private double m02, m12, m22;

        public MatrixSym3x3() : this(0, 0, 0, 0, 0, 0) { }

        public MatrixSym3x3(double a00, double a01, double a02, double a11, double a12, double a22)
        {
            m00 = a00;
            m01 = a01;
            m02 = a02;
            m11 = a11;
            m12 = a12;
            m22 = a22;
        }

        public double DetAndInverse(out MatrixSym3x3 inverseMatrix)
        {
            double det = Det();
            inverseMatrix = this;

            if (det != 0)
                inverseMatrix /= det;

            return det;
        }

        public double Det()
        {
            return m00 * (m11 * m22 - m12 * m12)
                - m01 * (m01 * m22 - m02 * m12)
                + m12 * (m01 * m12 - m11 * m02);
        }

        public static MatrixSym3x3 operator * (MatrixSym3x3 M, double s)
        {
            M.m00 *= s;
            M.m01 *= s;
            M.m02 *= s;
            M.m11 *= s;
            M.m12 *= s;
            M.m22 *= s;
            return M;
        }

        public static MatrixSym3x3 operator / (MatrixSym3x3 M, double s)
        {
            M.m00 /= s;
            M.m01 /= s;
            M.m02 /= s;
            M.m11 /= s;
            M.m12 /= s;
            M.m22 /= s;
            return M;
        }

        public static Vector3d operator *(MatrixSym3x3 M, Vector3d v)
        {
            Vector3d output = new Vector3d();

            output.x = M.m00 * v.x + M.m01 * v.y + M.m02 * v.z;
            output.y = M.m01 * v.x + M.m11 * v.y + M.m12 * v.z;
            output.z = M.m02 * v.x + M.m12 * v.y + M.m22 * v.z;

            return output;
        }
    }
}
