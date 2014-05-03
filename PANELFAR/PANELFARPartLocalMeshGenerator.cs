using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    public static class PANELFARPartLocalMeshGenerator
    {
        public static PANELFARPartLocalMesh GenerateFromPart(Part p)
        {
            PANELFARPartLocalMesh mesh = new PANELFARPartLocalMesh();

            Matrix4x4 partUpMatrix = p.transform.worldToLocalMatrix;

            List<Vector3d> vertexList = new List<Vector3d>();
            List<int> vertexForTriList = new List<int>();

            int vertexOffset = 0;
            foreach (Transform t in p.FindModelComponents<Transform>())
            {
                MeshFilter mf = t.GetComponent<MeshFilter>();
                if (mf == null)
                    continue;
                Mesh m = mf.mesh;

                if (m == null)
                    continue;

                if (p.InternalModelName == m.name)
                    continue;

                Matrix4x4 matrix = partUpMatrix * t.localToWorldMatrix;

                foreach (Vector3 vertex in m.vertices)
                {
                    Vector3d v = matrix.MultiplyPoint(vertex);
                    vertexList.Add(v);
                }
                foreach(int i in m.triangles)
                {
                    vertexForTriList.Add(i + vertexOffset);     //Vertex offset since otherwise there will be problems with parts that contain multiple mesh transforms
                }
                vertexOffset += m.vertices.Length;
            }

            mesh.vertexes = vertexList.ToArray();
            mesh.vertexesForTriangles = vertexForTriList.ToArray();
            mesh.part = p;

            return mesh;
        }
    }
}
