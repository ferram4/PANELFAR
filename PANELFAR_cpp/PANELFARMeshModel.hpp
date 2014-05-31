/* Model used for handling contraction, based on QSlim software package, GPL v2*/

#pragma once

#include "PANELFARPairContraction.hpp"


namespace panelfar_cpp
{
	namespace mesh_simplification
	{
		#define MX_VALID_FLAG 0x01
		#define MX_PROXY_FLAG 0x02
		#define MX_TOUCHED_FLAG 0x04

		public class MeshModel
		{
		private:
			struct vertex_data {
				unsigned char mark, tag;             // Internal tag bits
				unsigned char user_mark, user_tag;   // External tag bits
			};
			struct face_data {
				unsigned char mark, tag;             // Internal tag bits
				unsigned char user_mark, user_tag;   // External tag bits
			};

			unsigned int flags;

			// Required blocks
			std::vector<MeshVertex> vertices;
			std::vector<MeshFace> faces;

			std::vector<vertex_data> v_data;
			std::vector<face_data> f_data;
			std::vector<std::vector<unsigned int>> face_links;


		protected:
			//
			// Accessors for internal tag and mark bits
			inline unsigned int v_check_tag(MeshVertID i, unsigned int tag) const { return v_data[i].tag&tag; }
			inline void v_set_tag(MeshVertID i, unsigned int tag) { v_data[i].tag |= tag; }
			inline void v_unset_tag(MeshVertID i, unsigned int tag) { v_data[i].tag &= ~tag; }
			inline unsigned char vmark(MeshVertID i) const { return v_data[i].mark; }
			inline void vmark(MeshVertID i, unsigned char m) { v_data[i].mark = m; }

			inline unsigned int f_check_tag(MeshFaceID i, unsigned int tag) const { return f_data[i].tag&tag; }
			inline void f_set_tag(MeshFaceID i, unsigned int tag) { f_data[i].tag |= tag; }
			inline void f_unset_tag(MeshFaceID i, unsigned int tag) { f_data[i].tag &= ~tag; }
			inline unsigned char fmark(MeshFaceID i) const { return f_data[i].mark; }
			inline void fmark(MeshFaceID i, unsigned char m) { f_data[i].mark = m; }

			inline MeshVertID alloc_vertex(double, double, double);
			inline void free_vertex(MeshVertID);
			inline void free_face(MeshFaceID);
			inline MeshFaceID alloc_face(MeshVertID, MeshVertID, MeshVertID);
			inline void init_face(MeshFaceID);
			inline virtual void init_vertex(MeshVertID);

		public:
			inline MeshModel(unsigned int nvert, unsigned int nface)
				: vertices(nvert), faces(nface), v_data(nvert), f_data(nface), face_links(nvert)
			{
			}

			inline MeshModel(int nvert, int nface) : vertices(nvert), faces(nface)
			{
			}

			inline MeshModel *clone(MeshModel *into = NULL);

			inline unsigned int vert_count() const { return vertices.size(); }
			inline unsigned int face_count() const { return faces.size(); }

			inline MeshVertID add_vertex(double, double, double);
			inline MeshFaceID add_face(unsigned int, unsigned int, unsigned int, bool will_link = true);

			inline MeshVertID add_vertex(MeshVertex v) { return add_vertex(v[0], v[1], v[2]); }
			inline MeshFaceID add_face(MeshFace f) { return add_face(f[0], f[1], f[2]); }

			inline void remove_vertex(MeshVertID v);
			inline void remove_face(MeshFaceID f);

			inline MeshVertex& vertex(unsigned int i) { return vertices[i]; }
			inline MeshFace& face(unsigned int i) { return faces[i]; }
			inline MeshVertex& corner(MeshFaceID f, short i) { return vertex(face(f)[i]); }

			inline void compute_face_normal(MeshFaceID, Vector3 &n, bool will_unitize = true);
			inline double compute_face_plane(MeshFaceID, Vector3 &p, bool will_unitize = true);
			inline double compute_face_area(MeshFaceID);


			////////////////////////////////////////////////////////////////////////
			//  Tagging and marking
			//
			inline unsigned int vertex_is_valid(MeshVertID i) const
			{
				return v_check_tag(i, MX_VALID_FLAG);
			}
			inline void vertex_mark_valid(MeshVertID i) { v_set_tag(i, MX_VALID_FLAG); }
			inline void vertex_mark_invalid(MeshVertID i) { v_unset_tag(i, MX_VALID_FLAG); }

			inline unsigned int face_is_valid(MeshFaceID i) const { return f_check_tag(i, MX_VALID_FLAG); }
			inline void face_mark_valid(MeshFaceID i) { f_set_tag(i, MX_VALID_FLAG); }
			inline void face_mark_invalid(MeshFaceID i) { f_unset_tag(i, MX_VALID_FLAG); }

			inline unsigned int vertex_is_proxy(MeshVertID i) const
			{
				return v_check_tag(i, MX_PROXY_FLAG);
			}
			inline void vertex_mark_proxy(MeshVertID i) { v_set_tag(i, MX_PROXY_FLAG); }
			inline void vertex_mark_nonproxy(MeshVertID i) { v_unset_tag(i, MX_PROXY_FLAG); }

			//
			// Accessors for external tag and mark bits
			inline unsigned int vertex_check_tag(MeshVertID i, unsigned int tag) const
			{
				return v_data[i].user_tag&tag;
			}
			inline void vertex_set_tag(MeshVertID i, unsigned int tag) { v_data[i].user_tag |= tag; }
			inline void vertex_unset_tag(MeshVertID i, unsigned int tag) { v_data[i].user_tag &= ~tag; }
			inline unsigned char vertex_mark(MeshVertID i) { return v_data[i].user_mark; }
			inline void vertex_mark(MeshVertID i, unsigned char m) { v_data[i].user_mark = m; }

			inline unsigned int face_check_tag(MeshFaceID i, unsigned int tag) const
			{
				return f_data[i].user_tag&tag;
			}
			inline void face_set_tag(MeshFaceID i, unsigned int tag) { f_data[i].user_tag |= tag; }
			inline void face_unset_tag(MeshFaceID i, unsigned int tag) { f_data[i].user_tag &= ~tag; }
			inline unsigned char face_mark(MeshFaceID i) { return f_data[i].user_mark; }
			inline void face_mark(MeshFaceID i, unsigned char m) { f_data[i].user_mark = m; }


			////////////////////////////////////////////////////////////////////////
			//  Vertex proxy management and proxy-aware accessors
			//
			inline MeshVertID add_proxy_vertex(MeshVertID);
			inline MeshVertID& vertex_proxy_parent(MeshVertID);
			inline MeshVertID resolve_proxies(MeshVertID v);
			inline Vector3 *vertex_position(MeshVertID v);


			////////////////////////////////////////////////////////////////////////
			//  Neighborhood collection and management
			//
			inline void mark_neighborhood(MeshVertID, unsigned short mark = 0);
			inline void collect_unmarked_neighbors(MeshVertID, std::vector<unsigned int>& faces);
			inline void mark_neighborhood_delta(MeshVertID, short delta);
			inline void partition_marked_neighbors(MeshVertID, unsigned short pivot,
				std::vector<MeshFaceID>& below, std::vector<MeshFaceID>& above);

			inline void mark_corners(const std::vector<unsigned int>& faces, unsigned short mark = 0);
			inline void collect_unmarked_corners(const std::vector<unsigned int>& faces, std::vector<unsigned int>& verts);

			inline void collect_edge_neighbors(MeshVertID, MeshVertID, std::vector<unsigned int>&);
			inline void collect_vertex_star(MeshVertID v, std::vector<unsigned int>& verts);

			inline std::vector<unsigned int>& neighbors(MeshVertID v) { return face_links[v]; }
			inline const std::vector<unsigned int>& neighbors(MeshVertID v) const { return face_links[v]; }

			inline void collect_neighborhood(MeshVertID v, int depth, std::vector<unsigned int>& faces);

			////////////////////////////////////////////////////////////////////////
			// Primitive transformation operations
			//
			inline void remap_vertex(MeshVertID from, MeshVertID to);
			inline MeshVertID split_edge(MeshVertID v1, MeshVertID v2, double x, double y, double z);
			inline MeshVertID split_edge(MeshVertID v1, MeshVertID v2);

			inline void flip_edge(MeshVertID v1, MeshVertID v2);

			// split_face3
			inline void split_face4(MeshFaceID f, MeshVertID *newverts = NULL);

			inline void unlink_face(MeshFaceID f);

			////////////////////////////////////////////////////////////////////////
			// Contraction and related operations
			//
			inline void compact_vertices();
			inline void remove_degeneracy(std::vector<unsigned int>&);

			// Pair contraction interface
			inline void compute_contraction(MeshVertID, MeshVertID,
				PairContraction *, const Vector3 *vnew = NULL);
			inline void apply_contraction(const PairContraction&);
			inline void apply_expansion(const PairContraction&);
			inline void contract(MeshVertID v1, MeshVertID v2,
				const Vector3 *vnew, PairContraction *conx);
		};

		template<class T>
		inline bool varray_find(const std::vector<T>& A, const T& t, uint *index = NULL)
		{
			for (unsigned int i = 0; i<A.length(); i++)
			if (A[i] == t)
			{
				if (index) *index = i;
				return true;
			}
			return false;
		}
	}
}