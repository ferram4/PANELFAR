using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace panelfar
{
    public static class PANELFARTriangleUtils
    {
        public static Vector3 triangle_raw_normal(Vector3 v1, Vector3 v2, Vector3 v3)
        {
            return Vector3.Cross(v2 - v1, v3 - v1);
        }

        public static double triangle_area(Vector3 v1, Vector3 v2, Vector3 v3)
        {
            return 0.5 * (triangle_raw_normal(v1, v2, v3)).magnitude;
        }

        public static Vector3 triangle_normal(Vector3 v1, Vector3 v2, Vector3 v3)
        {
            return triangle_raw_normal(v1, v2, v3).normalized;
        }

        public static Vector4 triangle_plane(Vector3 v1, Vector3 v2, Vector3 v3)
        {
            Vector3 n = triangle_normal(v1, v2, v3);
            return new Vector4(n.x, n.y, n.z, -Vector3.Dot(n, v1));
        }

    }
}
