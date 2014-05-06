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
            //Array of vertices; indexing must not change
            Vector3[] verts = new Vector3[mesh.vertexes.Length];
            mesh.vertexes.CopyTo(verts, 0);

            //Array of triangles; each triangle points to an index in verts
            MeshIndexTriangle[] indexTris = new MeshIndexTriangle[mesh.triangles.Length];
            mesh.triangles.CopyTo(indexTris, 0);

            //Array of a list of triangles that contain a given vertex; indexing is same as verts, each index in list points to an index in indexTris
            List<int>[] trisAttachedToVerts = GetTrisAttachedToVerts(verts, indexTris);

            //Array of quadrics associated with a particular vertex; indexing is same as verts
            Quadric[] vertQuadrics = CalculateVertQuadrics(verts, indexTris);

            //A list of possible vertex pairs that can be contracted into a single point
            List<MeshPairContraction> pairContractions = GeneratePairContractions(indexTris);

            //Calculate point that each pair contraction will contract to if it is to be done
            CalculateTargetPositionForAllPairContractions(ref pairContractions, verts, vertQuadrics);

            return mesh;
        }

        public static List<MeshPairContraction> GeneratePairContractions(MeshIndexTriangle[] indexTris)
        {
            List<MeshPairContraction> pairContractions = new List<MeshPairContraction>();

            foreach(MeshIndexTriangle tri in indexTris)
            {
                MeshPairContraction e0 = new MeshPairContraction(tri.v0, tri.v1),
                    e1 = new MeshPairContraction(tri.v1, tri.v2),
                    e2 = new MeshPairContraction(tri.v2, tri.v0);

                if (!pairContractions.Contains(e0))
                    pairContractions.Add(e0);
                if (!pairContractions.Contains(e1))
                    pairContractions.Add(e1);
                if (!pairContractions.Contains(e2))
                    pairContractions.Add(e2);
            }

            return pairContractions;
        }

        public static void CalculateTargetPositionForAllPairContractions(ref List<MeshPairContraction> pairContractions, Vector3[] verts, Quadric[] vertQuadrics)
        {
            for (int i = 0; i < pairContractions.Count; i++)
            {
                MeshPairContraction pair = pairContractions[i];
                CalculateTargetPositionForPairContraction(ref pair, verts, vertQuadrics);
                pairContractions[i] = pair;
            }
        }
            
        public static void CalculateTargetPositionForPairContraction(ref MeshPairContraction pair, Vector3[] verts, Quadric[] vertQuadrics)
        {
            Vector3 v0 = verts[pair.v0], v1 = verts[pair.v1];
            Quadric Q0 = vertQuadrics[pair.v0], Q1 = vertQuadrics[pair.v1];
            Quadric Q = Q0 + Q1;

            if (Q.Optimize(ref pair.contractedPosition, 1e-12))
                pair.error = -Q.Evaluate(pair.contractedPosition);
            else
            {
                double ei = Q.Evaluate(v0), ej = Q.Evaluate(v1);
                if (ei < ej)
                {
                    pair.error = -ei;
                    pair.contractedPosition = v0;
                }
                else
                {
                    pair.error = -ej;
                    pair.contractedPosition = v1;
                }
            }
        }

        //This returns an array that contains (in each element) a list of indexes that specify which MeshIndexTriangles (in indexTris) are connected to which Vector3s (in verts)
        public static List<int>[] GetTrisAttachedToVerts(Vector3[] verts, MeshIndexTriangle[] indexTris)
        {
            List<int>[] trisAttachedToVerts = new List<int>[verts.Length];

            for (int i = 0; i < trisAttachedToVerts.Length; i++)
            {
                trisAttachedToVerts[i] = new List<int>();
            }

            for (int i = 0; i < indexTris.Length; i++)
            {
                MeshIndexTriangle tri = indexTris[i];

                trisAttachedToVerts[tri.v0].Add(i);
                trisAttachedToVerts[tri.v1].Add(i);
                trisAttachedToVerts[tri.v2].Add(i);
            }


            return trisAttachedToVerts;
        }

        public static Quadric[] CalculateVertQuadrics(Vector3[] verts, MeshIndexTriangle[] indexTris)
        {
            Quadric[] vertQuadrics = new Quadric[verts.Length];

            for (int i = 0; i < vertQuadrics.Length; i++ )
                vertQuadrics[i] = new Quadric();

            foreach (MeshIndexTriangle tri in indexTris)
            {
                Vector3 v0, v1, v2;
                v0 = verts[tri.v0];
                v1 = verts[tri.v1];
                v2 = verts[tri.v2];
                Vector4 p = PANELFARTriangleUtils.triangle_plane(v0, v1, v2);
                double area = PANELFARTriangleUtils.triangle_area(v0, v1, v2);
                Quadric Q = new Quadric(p.x, p.y, p.z, p.w, area);

                // Area-weight quadric and add it into the three quadrics for the corners
                Q *= Q.area;
                vertQuadrics[tri.v0] += Q;
                vertQuadrics[tri.v1] += Q;
                vertQuadrics[tri.v2] += Q;
            }

            return vertQuadrics;
        }
    }
}
