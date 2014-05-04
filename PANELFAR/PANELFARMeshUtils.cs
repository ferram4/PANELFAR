using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    public struct meshIndexEdge
    {
        public meshIndexEdge(int vert0, int vert1)
        {
            v0 = vert0;
            v1 = vert1;
        }

        public int v0;
        public int v1;
    }
    
    public struct meshIndexTriangle
    {
        public meshIndexTriangle(int vert0, int vert1, int vert2)
        {
            v0 = vert0;
            v1 = vert1;
            v2 = vert2;
        }

        public int v0;
        public int v1;
        public int v2;
    }

    public struct meshIndexQuad
    {
        public meshIndexQuad(int vert0, int vert1, int vert2, int vert3)
        {
            v0 = vert0;
            v1 = vert1;
            v2 = vert2;
            v3 = vert3;
        }

        public int v0;
        public int v1;
        public int v2;
        public int v3;
    }

    public struct meshVertex
    {
        public meshVertex(Vector3 vertex, meshIndexTriangle[] tris)
        {
            vert = vertex;
            attachedTris = tris;
        }

        public Vector3 vert;
        public meshIndexTriangle[] attachedTris;
    }

    public struct meshTriangle
    {
        public meshTriangle(meshIndexTriangle triangle, meshVertex[] verts)
        {
            tri = triangle;
            attachedVerts = verts;
        }

        public meshIndexTriangle tri;
        public meshVertex[] attachedVerts;
    }
}
