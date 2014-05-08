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
            MinHeap<MeshPairContraction> pairContractions = GeneratePairContractions(indexTris, verts, vertQuadrics);

            int faces = (int)Math.Floor(indexTris.Length * 0.5);

            faces = DecimateVertices(faces, ref pairContractions, ref verts, ref indexTris, ref trisAttachedToVerts, ref vertQuadrics);

            //This will be used to update the old array (which has many empty elements) to a new vertex array and allow the indexTris to be updated as well
            Dictionary<int, int> beforeIndexAfterIndex = new Dictionary<int, int>();
            int currentIndex = 0;

            List<Vector3> newVerts = new List<Vector3>();
            for (int i = 0; i < verts.Length; i++)
            {
                Vector3 v = verts[i];
                if (trisAttachedToVerts[i] != null)
                {
                    beforeIndexAfterIndex.Add(i, currentIndex);
                    currentIndex++;
                    newVerts.Add(v);
                }
            }

            MeshIndexTriangle[] newIndexTris = new MeshIndexTriangle[faces];

            currentIndex = 0;
            foreach(MeshIndexTriangle tri in indexTris)
            {
                if(tri != null)
                {
                    MeshIndexTriangle newTri = new MeshIndexTriangle(beforeIndexAfterIndex[tri.v0], beforeIndexAfterIndex[tri.v1], beforeIndexAfterIndex[tri.v2]);
                    newIndexTris[currentIndex] = newTri;
                    currentIndex++;
                }
            }

            mesh.vertexes = newVerts.ToArray();
            mesh.triangles = newIndexTris;


            return mesh;
        }

        public static int DecimateVertices(int targetFaces, ref MinHeap<MeshPairContraction> pairContractions, ref Vector3[] verts, ref MeshIndexTriangle[] indexTris, ref List<int>[] trisAttachedToVerts, ref Quadric[] vertQuadrics)
        {
            int validFaces = indexTris.Length;
            int counter = 1;
            StringBuilder debug = new StringBuilder();
            debug.AppendLine("Target Faces: " + targetFaces);
            try
            {
                while (validFaces > targetFaces)
                {
                    debug.AppendLine("Iteration: " + counter + " Faces: " + validFaces);

                    //Get the pair contraction with the least error associated with it
                    MeshPairContraction pair = pairContractions.ExtractDominating();

                    debug.AppendLine("Contraction between vertices at indicies: " + pair.v0 + " and " + pair.v1);
                    debug.AppendLine("Tris attached to v0: " + trisAttachedToVerts[pair.v0].Count + " Tris attached to v1: " + trisAttachedToVerts[pair.v1].Count);

                    //Get faces that will be deleted / changed by contraction
                    ComputeContraction(ref pair, indexTris, trisAttachedToVerts);

                    //Act on faces, delete extra vertex, change all references to second vertex
                    validFaces -= ApplyContraction(ref pair, ref pairContractions, ref verts, ref indexTris, ref trisAttachedToVerts, ref vertQuadrics);

                    counter++;
                }
                for(int i = 0; i < indexTris.Length; i++)
                {
                    MeshIndexTriangle tri = indexTris[i];
                    if (tri == null)
                        continue;
                    if (trisAttachedToVerts[tri.v0] == null)
                    {
                        debug.AppendLine("Tri at index " + i + " points to nonexistent vertex at index " + tri.v0);
                    }
                    if (trisAttachedToVerts[tri.v1] == null)
                    {
                        debug.AppendLine("Tri at index " + i + " points to nonexistent vertex at index " + tri.v1);
                    }
                    if (trisAttachedToVerts[tri.v2] == null)
                    {
                        debug.AppendLine("Tri at index " + i + " points to nonexistent vertex at index " + tri.v2);
                    }
                }

                debug.AppendLine("Final: Faces: " + validFaces);
            }
            catch (Exception e)
            {
                debug.AppendLine("Error: " + e.Message);
                debug.AppendLine("Stack trace");
                debug.AppendLine(e.StackTrace);
            }


            Debug.Log(debug.ToString());
            return validFaces;
        }

        public static int ApplyContraction(ref MeshPairContraction pair, ref MinHeap<MeshPairContraction> pairContractions, ref Vector3[] verts, ref MeshIndexTriangle[] indexTris, ref List<int>[] trisAttachedToVerts, ref Quadric[] vertQuadrics)
        {
            int removedFaces = pair.deletedFaces.Count;

            //Move v0, clear v1
            verts[pair.v0] = pair.contractedPosition;
            verts[pair.v1] = Vector3.zero;

            foreach (int triIndex in trisAttachedToVerts[pair.v1])
                if (!pair.deletedFaces.Contains(triIndex) && !trisAttachedToVerts[pair.v0].Contains(triIndex))
                    trisAttachedToVerts[pair.v0].Add(triIndex);

            //Clear out all the tris attached to a non-existent vertex
            trisAttachedToVerts[pair.v1] = null;

            //Accumulate quadrics, clear unused one
            vertQuadrics[pair.v0] += vertQuadrics[pair.v1];
            vertQuadrics[pair.v1] = new Quadric();

            //Adjust deformed triangles
            foreach (int changedTri in pair.deformedFaces)
            {
                MeshIndexTriangle tri = indexTris[changedTri];
                if (tri.v0.Equals(pair.v1))
                    tri.v0 = pair.v0;
                else if (tri.v1.Equals(pair.v1))
                    tri.v1 = pair.v0;
                else
                    tri.v2 = pair.v0;

                indexTris[changedTri] = tri;
            }

            //Clear deleted triangles
            foreach(int deletedTri in pair.deletedFaces)
            {
                indexTris[deletedTri] = null;
            }

            List<MeshPairContraction> pairList = pairContractions.ToList();

            for (int i = 0; i < pairContractions.Count; i++)
            {
                MeshPairContraction otherPair = pairList[i];
                if (otherPair.v0.Equals(pair.v1))
                {
                    otherPair.v0 = pair.v0;
                }
                else if (otherPair.v1.Equals(pair.v1))
                {
                    otherPair.v1 = pair.v0;
                }
                pairList[i] = otherPair;
            }

            int count = pairList.Count;
            for (int i = 0; i < count; i++ )
            {
                MeshPairContraction iItem = pairList[i];
                for(int j = i + 1; j < count; j++)
                {
                    if (pairList[j].Equals(iItem))
                    {
                        pairList.RemoveAt(j);   //Remove duplicate element
                        count--;                //Reduce length to iterate over
                        j--;                    //Make sure not to skip over a duplicate
                    }
                }
                if(iItem.v1 == iItem.v0)
                {
                    pairList.RemoveAt(i);   //Remove degenerate edge
                    count--;                //Reduce length to iterate over
                    i--;                    //Make sure not to skip over a duplicate
                    continue;
                }
                CalculateTargetPositionForPairContraction(ref iItem, verts, vertQuadrics);
                pairList[i] = iItem;
            }

            pairContractions = new MinHeap<MeshPairContraction>(pairList);

            return removedFaces;
        }

        public static void ComputeContraction(ref MeshPairContraction pair, MeshIndexTriangle[] indexTris, List<int>[] trisAttachedToVerts)
        {
            //This contains a list of all tris that will be changed by this contraction; boolean indicates whether they will be removed or not
            Dictionary<int, bool> trisToChange = new Dictionary<int, bool>();

            pair.deformedFaces.Clear();
            pair.deletedFaces.Clear();

            //Iterate through every triangle attached to vertex 0 of this pair and add them to the dict
            foreach(int triIndex in trisAttachedToVerts[pair.v0])
            {
                if(indexTris[triIndex] != null)
                    trisToChange.Add(triIndex, false);
            }

            //Iterate through tris attached to vert 1...
            foreach (int triIndex in trisAttachedToVerts[pair.v1])
            {
                if (indexTris[triIndex] == null)
                    continue;

                //if the tri is already there, it will become degenerate during this contraction and should be removed
                if (trisToChange.ContainsKey(triIndex))
                    trisToChange[triIndex] = true;
                //else, add it and it will simply be deformed
                else
                    trisToChange.Add(triIndex, false);
            }

            //Now, divide them into the appropriate lists
            foreach(KeyValuePair<int, bool> triIndex in trisToChange)
            {
                if (triIndex.Value)
                    pair.deletedFaces.Add(triIndex.Key);
                else
                    pair.deformedFaces.Add(triIndex.Key);
            }
        }

        public static MinHeap<MeshPairContraction> GeneratePairContractions(MeshIndexTriangle[] indexTris, Vector3[] verts, Quadric[] vertQuadrics)
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

            //Calculate point that each pair contraction will contract to if it is to be done
            CalculateTargetPositionForAllPairContractions(ref pairContractions, verts, vertQuadrics);


            MinHeap<MeshPairContraction> heap = new MinHeap<MeshPairContraction>(pairContractions);

            return heap;
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
                pair.error = Q.Evaluate(pair.contractedPosition);
            else
            {
                double ei = Q.Evaluate(v0), ej = Q.Evaluate(v1);
                if (ei < ej)
                {
                    pair.error = ei;
                    pair.contractedPosition = v0;
                }
                else
                {
                    pair.error = ej;
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

        //Returns an array of quadrics for evaluating the error of each possible contraction
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
                double area = PANELFARTriangleUtils.triangle_area(v0, v1, v2);
                Vector4 p;
                if (area > 0)
                    p = PANELFARTriangleUtils.triangle_plane(v0, v1, v2);
                else
                {
                    p = PANELFARTriangleUtils.triangle_plane(v2, v1, v0);
                    area = -area;
                }

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
