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

            int vertexCounter = 0;
            foreach (Transform t in p.FindModelComponents<Transform>())
            {
                MeshFilter mf = t.GetComponent<MeshFilter>();
                if (mf == null)
                    continue;
                Mesh m = mf.mesh;

                if (m == null)
                    continue;

                Matrix4x4 matrix = partUpMatrix * t.localToWorldMatrix;

                for (int i = 0; i < m.vertices.Length; i++)
                {
                    Vector3d v = matrix.MultiplyPoint(m.vertices[i]);
                    vertexList.Add(v);

                    int j = m.triangles[i];     //Vertex counter since otherwise there will be problems with parts that contain multiple mesh transforms
                    vertexForTriList.Add(vertexCounter);
                    vertexCounter++;
                }
            }

            mesh.vertexes = vertexList.ToArray();
            mesh.vertexesForTriangles = vertexForTriList.ToArray();
            mesh.part = p;

            return mesh;
        }
    }
}
