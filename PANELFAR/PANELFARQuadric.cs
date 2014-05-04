using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace panelfar
{
    public class PANELFARQuadric
    {
        private double a2;
        private double ab, b2;
        private double ac, bc, c2;
        private double ad, bd, cd, d2;

        public double area
        {
            get { return area; }
            private set { area = value; }
        }

        public PANELFARQuadric(): this(0, 0, 0, 0, 0)
        {
        }

        public PANELFARQuadric(double a, double b, double c, double d, double r)
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

        public double evaluate(double x, double y, double z)
        {
            return x * x * a2 + 2 * x * y * ab + 2 * x * z * ac + 2 * x * ad
            + y * y * b2 + 2 * y * z * bc + 2 * y * bd
            + z * z * c2 + 2 * z * cd
            + d2;
        }


        public static PANELFARQuadric operator +(PANELFARQuadric Q1, PANELFARQuadric Q2)
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

        public static PANELFARQuadric operator -(PANELFARQuadric Q1, PANELFARQuadric Q2)
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

        public static PANELFARQuadric operator *(PANELFARQuadric Q1, double s)
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
