using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace panelfar
{
    public class Quadric
    {
        private double a2;
        private double ab, b2;
        private double ac, bc, c2;
        private double ad, bd, cd, d2;

        public double area;

        public Quadric(): this(0, 0, 0, 0, 0)
        {
        }

        public Quadric(double a, double b, double c, double d, double r)
        {
            a2 = a * a;
            ab = a * b;
            ac = a * c;
            ad = a * d;
            b2 = b * b;
            bc = b * c;
            bd = b * d;
            c2 = c * c;
            cd = c * d;

            area = r;
        }

        public MatrixSym3x3 Tensor()
        {
            return new MatrixSym3x3(a2, ab, ac, b2, bc, c2);
        }

        public Vector3d Vector()
        {
            return new Vector3d(ad, bd, cd);
        }

        public double Offset()
        {
            return d2;
        }

        public double Evaluate(Vector3d vec)
        {
            return Evaluate(vec.x, vec.y, vec.z);
        }

        public double Evaluate(double x, double y, double z)
        {
            return x * x * a2
                + 2 * x * y * ab
                + 2 * x * z * ac
                + 2 * x * ad
            + y * y * b2
            + 2 * y * z * bc
            + 2 * y * bd
            + z * z * c2
            + 2 * z * cd
            + d2;
        }

        public bool Optimize(ref Vector3 newVert, double epsilon)
        {
            MatrixSym3x3 invMat;
            double det = this.Tensor().DetAndInverse(out invMat);
            if (PANELFARMathUtils.Approximately(det, 0, epsilon))
                return false;

            newVert = -(invMat * this.Vector());
            return true;
        }

        public static Quadric operator +(Quadric Q1, Quadric Q2)
        {
            // Accumulate area
            Q1.area += Q2.area;

            // Accumulate coefficients
            Q1.a2 += Q2.a2; Q1.ab += Q2.ab; Q1.ac += Q2.ac; Q1.ad += Q2.ad;
            Q1.b2 += Q2.b2; Q1.bc += Q2.bc; Q1.bd += Q2.bd;
            Q1.c2 += Q2.c2; Q1.cd += Q2.cd;
            Q1.d2 += Q2.d2;

            return Q1;
        }

        public static Quadric operator -(Quadric Q1, Quadric Q2)
        {
            // Accumulate area
            Q1.area -= Q2.area;

            // Accumulate coefficients
            Q1.a2 -= Q2.a2; Q1.ab -= Q2.ab; Q1.ac -= Q2.ac; Q1.ad -= Q2.ad;
            Q1.b2 -= Q2.b2; Q1.bc -= Q2.bc; Q1.bd -= Q2.bd;
            Q1.c2 -= Q2.c2; Q1.cd -= Q2.cd;
            Q1.d2 -= Q2.d2;

            return Q1;
        }

        public static Quadric operator *(Quadric Q1, double s)
        {
            // Scale coefficients
            Q1.a2 *= s; Q1.ab *= s; Q1.ac *= s; Q1.ad *= s;
            Q1.b2 *= s; Q1.bc *= s; Q1.bd *= s;
            Q1.c2 *= s; Q1.cd *= s;
            Q1.d2 *= s;

            return Q1;
        }
    }
}
