#include "stdafx.h"
#include "PANELFARMeshPrimitives-body.hpp"
#include <gtest\gtest.h>
#include <iostream>

namespace panelfar_cpp
{
	TEST(MeshVertex, HandlesOperations)
	{
		MeshVertex v0, v1, v2;
		v0 = MeshVertex(Vector3(1, 1, 1));
		v1 = MeshVertex(Vector3(1, 1, 1));
		v2 = MeshVertex(Vector3(2, 2, 2));

		EXPECT_EQ(v1, v0);

		EXPECT_EQ(v2, v0 + v1);

		v0 += v1;

		EXPECT_EQ(v2, v0);

		v0 = MeshVertex(Vector3(1, 1, 1));
		v1 = MeshVertex(Vector3(1, 1, 1));
		v2 = MeshVertex(Vector3(0, 0, 0));

		EXPECT_EQ(v2, v0 - v1);

		v0 -= v1;

		EXPECT_EQ(v2, v0);

		v0 = MeshVertex(Vector3(1, 1, 1));
		v1 = MeshVertex(Vector3(2, 2, 2));

		EXPECT_EQ(v1, v0 * 2);

		v0 *= 2;

		EXPECT_EQ(v1, v0);
	}
	TEST(MeshEdge, HandlesFunction)
	{
		MeshVertID v0, v1;
		v0 = 0;
		v1 = 2;

		MeshEdge e0 = MeshEdge(v0, v1);

		EXPECT_EQ(v0, e0.v0);
		EXPECT_EQ(v1, e0.v1);

		EXPECT_EQ(v0, e0.OppositeVert(v1));
		EXPECT_EQ(v1, e0.OppositeVert(v0));
	}
	TEST(MeshFace, HandlesFunction)
	{
		MeshVertID v0, v1, v2, v3;
		v0 = 0;
		v1 = 2;
		v2 = 5;
		v3 = 10;

		MeshFace f0 = MeshFace(v0, v1, v2);

		EXPECT_EQ(v0, f0[0]);
		EXPECT_EQ(v1, f0[1]);
		EXPECT_EQ(v2, f0[2]);

		EXPECT_EQ(v0, f0.opposite_vertex(v1, v2));
		EXPECT_EQ(v1, f0.opposite_vertex(v0, v2));
		EXPECT_EQ(v2, f0.opposite_vertex(v0, v1));

		EXPECT_TRUE(f0.is_inorder(v0, v1));
		EXPECT_TRUE(f0.is_inorder(v1, v2));
		EXPECT_TRUE(f0.is_inorder(v2, v0));
		EXPECT_FALSE(f0.is_inorder(v1, v0));

		f0.remap_vertex(v1, v3);

		EXPECT_EQ(v3, f0[1]);

	}
}