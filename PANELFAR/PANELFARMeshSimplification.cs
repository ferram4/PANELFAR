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
            MeshVertex[] meshVertices = CreateVertexConnectivityArray(mesh);
            MeshTriangle[] meshTriangles = TrianglesFromVertexConnectivity(meshVertices);
            MeshEdgeContraction[] meshEdgeContractions = GenerateEdgesForContraction(meshTriangles);

            Dictionary<MeshVertex, Quadric> quadrics = CalculateQuadrics(meshTriangles, meshVertices);

            CalculateTargetPositionForEdgeContractions(ref meshEdgeContractions, quadrics);

            

            return mesh;
        }

        public static void CalculateTargetPositionForEdgeContractions(ref MeshEdgeContraction[] meshEdgeContractions, Dictionary<MeshVertex, Quadric> quadrics)
        {
            for (int i = 0; i < meshEdgeContractions.Length; i++)
                CalculateTargetPositionForEdgeContraction(ref meshEdgeContractions[i], quadrics);
        }

        public static void CalculateTargetPositionForEdgeContraction(ref MeshEdgeContraction edge, Dictionary<MeshVertex, Quadric> quadrics)
        {
            MeshVertex v0 = edge.v0, v1 = edge.v1;
            Quadric Q0 = quadrics[v0], Q1 = quadrics[v1];
            Quadric Q = Q0 + Q1;


            if (Q.Optimize(ref edge.contractedPosition, 1e-12))
                edge.error = -Q.Evaluate(edge.contractedPosition);
            else
            {
                double ei = Q.Evaluate(v0.vert), ej = Q.Evaluate(v1.vert);
                if (ei < ej)
                {
                    edge.error = -ei;
                    edge.contractedPosition = v0.vert;
                }
                else
                {
                    edge.error = -ej;
                    edge.contractedPosition = v1.vert;
                }
            }
        }

        public static MeshEdgeContraction[] GenerateEdgesForContraction(MeshTriangle[] meshTriangles)
        {
            List<MeshEdgeContraction> meshEdges = new List<MeshEdgeContraction>();

            foreach(MeshTriangle tri in meshTriangles)
            {
                MeshEdgeContraction e0, e1, e2, e0_, e1_, e2_;
                MeshVertex v0 = tri.attachedVerts[0], v1 = tri.attachedVerts[1], v2 = tri.attachedVerts[2];

                e0 = new MeshEdgeContraction(v0, v1);
                e0_ = new MeshEdgeContraction(v1, v0);
                if(!(meshEdges.Contains(e0) || meshEdges.Contains(e0_)))
                    meshEdges.Add(e0);
                
                e1 = new MeshEdgeContraction(v1, v2);
                e1_ = new MeshEdgeContraction(v2, v1);
                if(!(meshEdges.Contains(e1) || meshEdges.Contains(e1_)))
                    meshEdges.Add(e1);

                e2 = new MeshEdgeContraction(v2, v0);
                e2_ = new MeshEdgeContraction(v0, v2);
                if (!(meshEdges.Contains(e2) || meshEdges.Contains(e2_)))
                    meshEdges.Add(e2);
            }

            return meshEdges.ToArray();
        }

        public static Dictionary<MeshVertex, Quadric> CalculateQuadrics(MeshTriangle[] meshTriangles, MeshVertex[] meshVertices)
        {
            Dictionary<MeshVertex, Quadric> quadrics = new Dictionary<MeshVertex, Quadric>();

            foreach(MeshVertex vert in meshVertices)
                quadrics.Add(vert, new Quadric());

            foreach(MeshTriangle tri in meshTriangles)
            {
                MeshVertex v0, v1, v2;
                v0 = tri.attachedVerts[0];
                v1 = tri.attachedVerts[1];
                v2 = tri.attachedVerts[2];
                Vector4 p = PANELFARTriangleUtils.triangle_plane(v0.vert, v1.vert, v2.vert);
                double area = PANELFARTriangleUtils.triangle_area(v0.vert, v1.vert, v2.vert);
                Quadric Q = new Quadric(p.x, p.y, p.z, p.w, area);

                // Area-weight quadric and add it into the three quadrics for the corners
                Q *= Q.area;
                quadrics[v0] += Q;
                quadrics[v1] += Q;
                quadrics[v2] += Q;
            }

            return quadrics;
        }

        public static MeshTriangle[] TrianglesFromVertexConnectivity(MeshVertex[] meshVertices)
        {
            Dictionary<MeshIndexTriangle, List<MeshVertex>> triVertDict = new Dictionary<MeshIndexTriangle, List<MeshVertex>>();

            foreach(MeshVertex vert in meshVertices)
            {
                MeshIndexTriangle[] tris = vert.attachedTris;
                List<MeshVertex> tmpList;

                foreach(MeshIndexTriangle tri in tris)
                    if(triVertDict.TryGetValue(tri, out tmpList))
                    {
                        tmpList.Add(vert);
                        triVertDict[tri] = tmpList;
                    }
                    else
                    {
                        tmpList = new List<MeshVertex>();
                        tmpList.Add(vert);
                        triVertDict.Add(tri, tmpList);
                    }
            }

            MeshTriangle[] meshTriangles = new MeshTriangle[triVertDict.Count];

            int i = 0;
            foreach (KeyValuePair<MeshIndexTriangle, List<MeshVertex>> vertTris in triVertDict)
            {
                MeshTriangle meshTriangle = new MeshTriangle(vertTris.Key, vertTris.Value.ToArray());

                meshTriangles[i] = meshTriangle;
                i++;
            }

            return meshTriangles;
        }

        public static MeshVertex[] CreateVertexConnectivityArray(PANELFARPartLocalMesh mesh)
        {
            Dictionary<int, List<MeshIndexTriangle>> vertexTriDict = new Dictionary<int, List<MeshIndexTriangle>>();

            foreach(MeshIndexTriangle tri in mesh.triangles)
            {
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v0);
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v1);
                UpdateVertexTriDict(ref vertexTriDict, tri, tri.v2);
            }

            MeshVertex[] meshVertices = new MeshVertex[vertexTriDict.Count];

            int i = 0;
            foreach(KeyValuePair<int, List<MeshIndexTriangle>> vertTris in vertexTriDict)
            {
                Vector3 unconnectedVert = mesh.vertexes[vertTris.Key];
                MeshVertex meshVert = new MeshVertex(unconnectedVert, vertTris.Value.ToArray());

                meshVertices[i] = meshVert;
                i++;
            }

            return meshVertices;
        }

        public static void UpdateVertexTriDict(ref Dictionary<int, List<MeshIndexTriangle>> vertexTriDict, MeshIndexTriangle tri, int vertIndex)
        {
            List<MeshIndexTriangle> tmpList;
            if (vertexTriDict.TryGetValue(vertIndex, out tmpList))
            {
                tmpList.Add(tri);
                vertexTriDict[vertIndex] = tmpList;
            }
            else
            {
                tmpList = new List<MeshIndexTriangle>();
                tmpList.Add(tri);
                vertexTriDict.Add(vertIndex, tmpList);
            }
        }
    }
}
