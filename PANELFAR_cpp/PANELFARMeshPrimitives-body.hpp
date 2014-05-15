/* A set of mesh primitives */

#pragma once

#include "PANELFARMeshPrimitives.hpp"

namespace panelfar_cpp{

	namespace mesh_simplification
	{

		//MeshVertex 
		inline MeshVertex::MeshVertex(Vector3 const& v)
		{
			vertex = v;
		}

		inline bool MeshVertex::operator == (MeshVertex const& v1)
		{
			return vertex == v1.vertex;
		}

		inline bool MeshVertex::operator != (MeshVertex const& v1)
		{
			return vertex != v1.vertex;
		}

		inline void MeshVertex::operator += (MeshVertex const& v1)
		{
			this->vertex += v1.vertex;
		}

		inline void MeshVertex::operator -=(MeshVertex const& v1)
		{
			this->vertex -= v1.vertex;
		}


		inline void MeshVertex::operator *=(double const& s)
		{
			this->vertex *= s;
		}


		inline MeshVertex operator +(MeshVertex const& v0, MeshVertex const& v1)
		{
			MeshVertex vert = v0;
			vert += v1;
			return vert;
		}

		inline MeshVertex operator -(MeshVertex const& v0, MeshVertex const& v1)
		{
			MeshVertex vert = v0;
			vert -= v1;
			return vert;
		}

		inline MeshVertex operator *(MeshVertex const& v0, double const& s)
		{
			MeshVertex vert = v0;
			vert *= s;
			return vert;
		}

		inline bool operator == (MeshVertex const& v0, MeshVertex const& v1)
		{
			return v0.vertex == v1.vertex;
		}

		inline bool operator != (MeshVertex const& v0, MeshVertex const& v1)
		{
			return v0.vertex != v1.vertex;
		}


		//MeshEdge
		inline MeshEdge::MeshEdge(MeshVertID const& a, MeshVertID const& b)
		{
			v0 = a;
			v1 = b;
		}

		inline MeshVertID MeshEdge::OppositeVert(MeshVertID const& v)
		{
			if (v == v0) return v1;
			else { return v0; }
		}

		//MeshFace
		inline MeshFace::MeshFace(MeshVertID const& v0, MeshVertID const& v1, MeshVertID const& v2)
		{
			v[0] = v0;  v[1] = v1;  v[2] = v2;
		}

		inline int MeshFace::remap_vertex(MeshVertID const& from, MeshVertID const& to)
		{
			int nmapped = 0;
			for (int i = 0; i < 3; i++)
			if (v[i] == from)
			{
				v[i] = to;
				nmapped++;
			}
			return nmapped;
		}

		inline unsigned int MeshFace::find_vertex(MeshVertID const& i)
		{
			if (v[0] == i) return 0;
			else if (v[1] == i) return 1;
			else { return 2; }
		}

		inline MeshVertID MeshFace::opposite_vertex(MeshVertID const& v0, MeshVertID const& v1)
		{
			if (v[0] != v0 && v[0] != v1) return v[0];
			else if (v[1] != v0 && v[1] != v1) return v[1];
			else { return v[2]; }
		}

		inline bool MeshFace::is_inorder(MeshVertID const& v0, MeshVertID const& v1)
		{
			if (v[0] == v0) return v[1] == v1;
			else if (v[1] == v0) return v[2] == v1;
			else { return v[0] == v1; }
		}
	}
}