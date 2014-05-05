using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace panelfar
{
    public static class PANELFARMathUtils
    {
        public static bool Approximately(double p, double q, double error)
        {
            if (Math.Abs(p - q) < error)
                return true;
            return false;
        }
    }
}
