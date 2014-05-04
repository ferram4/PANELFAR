using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    public static class PANELFARMeshSimplification
    {
        //Take the raw part geometry and simplify it so that further simplification of the entire vessel is faster
        public static PANELFARPartLocalMesh PreProcessLocalMesh(PANELFARPartLocalMesh mesh)
        {
            meshVertex[] meshVertices = CreateVertexConnectivityArray(mesh);
            meshTriangle[] meshTriangles = TrianglesFromVertexConnectivity(meshVertices);

            Dictionary<meshVertex, PANELFARQuadric> quadrics = CalculateQuadrics(meshTriangles, meshVertices);

            return mesh;
        }

        public static Dictionary<meshVertex, PANELFARQuadric> CalculateQuadrics(meshTriangle[] meshTriangles, meshVertex[] meshVertices)
        {
            Dictionary<meshVertex, PANELFARQuadric> quadrics = new Dictionary<meshVertex, PANELFARQuadric>();

            foreach(meshVertex vert in meshVertices)
                quadrics.Add(vert, new PANELFARQuadric());

            foreach(meshTriangle tri in meshTriangles)
            {
                meshVertex v0, v1, v2;
                v0 = tri.attachedVerts[0];
                v1 = tri.attachedVerts[1];
                v2 = tri.attachedVerts[2];
                Vector4 p = PANELFARTriangleUtils.triangle_plane(v0.vert, v1.vert, v2.vert);
                double area = PANELFARTriangleUtils.triangle_area(v0.vert, v1.vert, v2.vert);
                PANELFARQuadric Q = new PANELFARQuadric(p.x, p.y, p.z, p.w, area);

                // Area-weight quadric and add it into the three quadrics for the corners
                Q *= Q.area;
                quadrics[v0] += Q;
                quadrics[v1] += Q;
                quadrics[v2] += Q;
            }

            return quadrics;
        }

        public static meshTriangle[] TrianglesFromVertexConnectivity(meshVertex[] meshVertices)
        {
            Dictionary<meshIndexTriangle, List<meshVertex>> triVertDict = new Dictionary<meshIndexTriangle, List<meshVertex>>();

            foreach(meshVertex vert in meshVertices)
            {
                meshIndexTriangle[] tris = vert.attachedTris;
                List<meshVertex> tmpList;

                foreach(meshIndexTriangle tri in tris)
                    if(triVertDict.TryGetValue(tri, out tmpList))
                    {
                        tmpList.Add(vert);
                        triVertDict[tri] = tmpList;
                    }
                    else
                    {
                        tmpList = new List<meshVertex>();
                        tmpList.Add(vert);
                        triVertDict.Add(tri, tmpList);
                    }
            }

            meshTriangle[] meshTriangles = new meshTriangle[triVertDict.Count];

            int i = 0;
            foreach (KeyValuePair<meshIndexTriangle, List<meshVertex>> vertTris in triVertDict)
            {
                meshTriangle meshTriangle = new meshTriangle(vertTris.Key, vertTris.Value.ToArray());

                meshTriangles[i] = meshTriangle;
                i++;
            }

            return meshTriangles;
        }

        public static meshVertex[] CreateVertexConnectivityArray(PANELFARPartLocalMesh mesh)
        {
            Dictionary<int, List<meshIndexTriangle>> vertexTriDict = new Dictionary<int, List<meshIndexTriangle>>();

            foreach(meshIndexTriangle tri in mesh.triangles)
            {
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v0);
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v1);
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v2);
            }

            meshVertex[] meshVertices = new meshVertex[vertexTriDict.Count];

            int i = 0;
            foreach(KeyValuePair<int, List<meshIndexTriangle>> vertTris in vertexTriDict)
            {
                Vector3 unconnectedVert = mesh.vertexes[vertTris.Key];
                meshVertex meshVert = new meshVertex(unconnectedVert, vertTris.Value.ToArray());

                meshVertices[i] = meshVert;
                i++;
            }

            return meshVertices;
        }

        public static void UpdateVertexTriDict(ref Dictionary<int, List<meshIndexTriangle>> vertexTriDict, meshIndexTriangle tri, int vertIndex)
        {
            List<meshIndexTriangle> tmpList;
            if (vertexTriDict.TryGetValue(vertIndex, out tmpList))
            {
                tmpList.Add(tri);
                vertexTriDict[vertIndex] = tmpList;
            }
            else
            {
                tmpList = new List<meshIndexTriangle>();
                tmpList.Add(tri);
                vertexTriDict.Add(vertIndex, tmpList);
            }
        }
    }
}
