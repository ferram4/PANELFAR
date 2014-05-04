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

            List<Vector3> vertexList = new List<Vector3>();
            List<meshIndexTriangle> vertexForTriList = new List<meshIndexTriangle>();

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
                    Vector3 v = matrix.MultiplyPoint(vertex);
                    vertexList.Add(v);
                }
                for (int i = 0; i < m.triangles.Length; i += 3)
                {
                    meshIndexTriangle tri = new meshIndexTriangle();
                    tri.v0 = m.triangles[i] + vertexOffset;
                    tri.v1 = m.triangles[i + 1] + vertexOffset;
                    tri.v2 = m.triangles[i + 2] + vertexOffset;
                    vertexForTriList.Add(tri);     //Vertex offset since otherwise there will be problems with parts that contain multiple mesh transforms
                }
                vertexOffset += m.vertices.Length;
            }

            mesh.vertexes = vertexList.ToArray();
            mesh.triangles = vertexForTriList.ToArray();
            mesh.part = p;

            mesh = PANELFARMeshSimplification.PreProcessLocalMesh(mesh);

            return mesh;
        }
    }
}
