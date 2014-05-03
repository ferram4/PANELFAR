using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    public class PANELFARPartLocalMesh
    {
        static Material _material;
        static Material material
        {
            get
            {
                if (_material == null) _material = new Material(Shader.Find("Particles/Additive"));
                return _material;
            }
        }

        public static void GLVertexMap(Vector3d worldPosition)
        {
            Vector3 screenPoint = PlanetariumCamera.Camera.WorldToScreenPoint(ScaledSpace.LocalToScaledSpace(worldPosition));
            GL.Vertex3(screenPoint.x / Camera.main.pixelWidth, screenPoint.y / Camera.main.pixelHeight, 0);
        }

        public Vector3d[] vertexes;
        public int[] vertexesForTriangles;
        public Part part;

        public void DebugDrawMesh()
        {
            Vector3d origin = part.transform.position;

            Vector3d[] transformedVerts = new Vector3d[vertexes.Length];

            Quaternion partRotation = part.transform.rotation;

            for (int i = 0; i < transformedVerts.Length; i++)
            {
                transformedVerts[i] = origin + partRotation * vertexes[i];
            }

            GL.PushMatrix();
            material.SetPass(0);
            GL.LoadOrtho();
            GL.Begin(GL.TRIANGLES);
            GL.wireframe = true;
            GL.Color(Color.yellow);

            for (int i = 0; i + 3 < transformedVerts.Length; i = i + 3) //triangles are defined by threes, so this makes sense
            {
                int id0, id1, id2;
                Vector3d v0, v1, v2;

                id0 = vertexesForTriangles[i];
                id1 = vertexesForTriangles[i + 1];
                id2 = vertexesForTriangles[i + 2];

                v0 = transformedVerts[id0];
                v1 = transformedVerts[id1];
                v2 = transformedVerts[id2];

                GLVertexMap(v0);
                GLVertexMap(v1);
                GLVertexMap(v2);

            }
            GL.End();
            GL.wireframe = false;
            GL.PopMatrix();
        }
    }
}
