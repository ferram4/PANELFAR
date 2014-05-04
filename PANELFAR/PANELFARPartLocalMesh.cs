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

        public Vector3[] vertexes;
        public meshIndexTriangle[] triangles;
        public Part part;

        public void DebugDrawMesh()
        {
            Vector3 origin = part.transform.position;

            Vector3[] transformedVerts = new Vector3[vertexes.Length];

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

            foreach(meshIndexTriangle tri in triangles) //triangles are defined by threes, so this makes sense
            {
                Vector3 v0, v1, v2;


                v0 = transformedVerts[tri.v0];
                v1 = transformedVerts[tri.v1];
                v2 = transformedVerts[tri.v2];

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
