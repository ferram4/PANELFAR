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
        public MeshVertex(Vector3 vertex) : this(vertex, new List<MeshTriangle>()){}

        public MeshVertex(Vector3 vertex, List<MeshTriangle> tris)
        {
            vert = vertex;
            attachedTris = tris;
        }

        public Vector3 vert;
        public List<MeshTriangle> attachedTris;
    }

    public struct MeshTriangle
    {
        public MeshTriangle(MeshIndexTriangle triangle) : this(triangle, new MeshVertex[3]) { }

        public MeshTriangle(MeshIndexTriangle triangle, MeshVertex[] verts)
        {
            tri = triangle;
            attachedVerts = verts;
        }

        public MeshIndexTriangle tri;
        public MeshVertex[] attachedVerts;
    }

    public struct MeshPairContraction : IComparable, IEquatable<MeshPairContraction>
    {
        public MeshPairContraction(int vert0, int vert1)
        {
            v0 = vert0;
            v1 = vert1;
            contractedPosition = new Vector3();
            error = 0;
            deletedFaces = new List<int>();
            deformedFaces = new List<int>();
        }

        public int v0, v1;
        public Vector3 contractedPosition;
        public double error;
        public List<int> deletedFaces;
        public List<int> deformedFaces;

        public int CompareTo(object obj)
        {
            return this.error.CompareTo(((MeshPairContraction)obj).error);
        }

        public static bool operator > (MeshPairContraction pair1, MeshPairContraction pair2)
        {
            return pair1.error > pair2.error;
        }

        public static bool operator <(MeshPairContraction pair1, MeshPairContraction pair2)
        {
            return pair1.error < pair2.error;
        }

        public static bool operator ==(MeshPairContraction pair1, MeshPairContraction pair2)
        {
            return pair1.Equals(pair2);
        }

        public static bool operator !=(MeshPairContraction pair1, MeshPairContraction pair2)
        {
            return !pair1.Equals(pair2);
        }

        public override bool Equals(object obj)
        {
            MeshPairContraction other = (MeshPairContraction)obj;
            return Equals(other);
        }

        public override int GetHashCode()
        {
            return this.v0.GetHashCode() + this.v1.GetHashCode();
        }

        public bool Equals(MeshPairContraction pair)
        {
            if (pair.v0 == this.v0 && pair.v1 == this.v1)
                return true;
            if (pair.v0 == this.v1 && pair.v1 == this.v0)
                return true;

            return false;
        }
    }
}
