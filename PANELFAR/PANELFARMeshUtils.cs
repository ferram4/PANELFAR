using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    public struct MeshIndexEdge
    {
        public MeshIndexEdge(int vert0, int vert1)
        {
            v0 = vert0;
            v1 = vert1;
        }

        public int v0;
        public int v1;
    }
    
    public struct MeshIndexTriangle
    {
        public MeshIndexTriangle(int vert0, int vert1, int vert2)
        {
            v0 = vert0;
            v1 = vert1;
            v2 = vert2;
        }

        public int v0;
        public int v1;
        public int v2;
    }

    public struct MeshIndexQuad
    {
        public MeshIndexQuad(int vert0, int vert1, int vert2, int vert3)
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

    public struct MeshVertex
    {
        public MeshVertex(Vector3 vertex, MeshIndexTriangle[] tris)
        {
            vert = vertex;
            attachedTris = tris;
        }

        public Vector3 vert;
        public MeshIndexTriangle[] attachedTris;
    }

    public struct MeshTriangle
    {
        public MeshTriangle(MeshIndexTriangle triangle, MeshVertex[] verts)
        {
            tri = triangle;
            attachedVerts = verts;
        }

        public MeshIndexTriangle tri;
        public MeshVertex[] attachedVerts;
    }

    public struct MeshEdgeContraction
    {
        public MeshEdgeContraction(MeshVertex vert0, MeshVertex vert1)
        {
            v0 = vert0;
            v1 = vert1;
            contractedPosition = new Vector3();
            error = 0;
        }
        public MeshVertex v0, v1;
        public Vector3 contractedPosition;
        public double error;
    }
}
