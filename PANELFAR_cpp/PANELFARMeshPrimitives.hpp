/* A set of mesh primitives */

#pragma once

#include "PANELFARMath.hpp"

namespace panelfar_cpp{

	typedef unsigned int MeshVertID;
	typedef unsigned int MxFaceID;

	public class MeshVertex
	{
	public:
		Vector3 vertex;

		inline MeshVertex(){}
		inline MeshVertex(Vector3 const& v);

		inline bool operator ==(MeshVertex const& v1);
		inline bool operator !=(MeshVertex const& v1);

		inline void operator +=(MeshVertex const& v1);
		inline void operator -=(MeshVertex const& v1);
		inline void operator *=(double const& s);
	};

	inline MeshVertex operator +(MeshVertex const& v0, MeshVertex const& v1);
	inline MeshVertex operator -(MeshVertex const& v0, MeshVertex const& v1);
	inline MeshVertex operator *(MeshVertex const& v0, double const& s);
	inline bool operator==(MeshVertex const& v0, MeshVertex const& v1);
	inline bool operator!=(MeshVertex const& v0, MeshVertex const& v1);


	public class MeshEdge
	{
	public:
		MeshVertID v0, v1;

		inline MeshEdge(){}
		inline MeshEdge(MeshVertID const& a, MeshVertID const& b);

		inline MeshVertID OppositeVert(MeshVertID const& v);
	};

	public class MeshFace
	{
	public:
		MeshVertID v[3];

		inline MeshFace() {}
		inline MeshFace(MeshVertID const& v0, MeshVertID const& v1, MeshVertID const& v2);

		inline MeshVertID& operator[](int i)       { return v[i]; }
		inline MeshVertID  operator[](int i) const { return v[i]; }

		inline int remap_vertex(MeshVertID const& from, MeshVertID const& to);

		inline unsigned int find_vertex(MeshVertID const& i);

		inline MeshVertID opposite_vertex(MeshVertID const& v0, MeshVertID const& v1);

		inline bool is_inorder(MeshVertID const& v0, MeshVertID const& v1);
	};
}