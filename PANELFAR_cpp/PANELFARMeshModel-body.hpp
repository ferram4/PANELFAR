/* Model used for handling contraction, based on QSlim software package, GPL v2*/

#pragma once

#include "PANELFARMeshModel.hpp"


namespace panelfar_cpp
{
	namespace mesh_simplification
	{
		inline MeshModel *MeshModel::clone(MeshModel *m)
		{
			if (!m) m = new MeshModel(vert_count(), face_count());

			unsigned int i;

			for (i = 0; i < vert_count(); i++)
				m->add_vertex(vertex(i));
			for (i = 0; i<face_count(); i++)
				m->add_face(face(i)[0], face(i)[1], face(i)[2]);

			return m;
		}


		inline MeshVertID MeshModel::alloc_vertex(double x, double y, double z)
		{
			vertices.push_back(MeshVertex(x, y, z));
			unsigned int id = vertices.size - 1;

			v_data.push_back(vertex_data());
			v_data[id].tag = 0x0;
			v_data[id].user_tag = 0x0;
			vertex_mark_valid(id);

			face_links.push_back(std::vector<unsigned int>());

			return id;
		}

		inline MeshVertID MeshModel::add_vertex(double x, double y, double z)
		{
			MeshVertID id = alloc_vertex(x, y, z);
			init_vertex(id);
			return id;
		}


		inline void MeshModel::remove_vertex(MeshVertID v)
		{
			free_vertex(v);
			//remove vert
			vertices[v] = vertices.back();
		}

		inline void MeshModel::remove_face(MeshFaceID f)
		{
			free_face(f);
			//remove face
			faces[f] = faces.back();
		}

		inline MeshFaceID MeshModel::add_face(unsigned int v1,
			unsigned int v2,
			unsigned int v3,
			bool will_link)
		{
			MeshFaceID id = alloc_face(v1, v2, v3);
			if (will_link)  init_face(id);
			return id;
		}

		inline void MeshModel::free_vertex(MeshVertID v)
		{
			face_links[v] = face_links.back();
			v_data[v] = v_data.back();
		}

		inline MeshFaceID MeshModel::alloc_face(MeshVertID v1, MeshVertID v2, MeshVertID v3)
		{
			faces.push_back(MeshFace(v1, v2, v3));
			
			MeshFaceID id = faces.size - 1;
			f_data.push_back(face_data());
			f_data[id].tag = 0x0;
			f_data[id].user_tag = 0x0;
			face_mark_valid(id);

			return id;
		}

		inline void MeshModel::free_face(MeshFaceID f)
		{
			f_data[f] = f_data.back();
		}

		inline void MeshModel::init_face(MeshFaceID id)
		{
			neighbors(face(id).v[0]).push_back(id);
			neighbors(face(id).v[1]).push_back(id);
			neighbors(face(id).v[2]).push_back(id);
		}


		inline void MeshModel::mark_neighborhood(MeshVertID vid, unsigned short mark)
		{

			for (unsigned int i = 0; i<neighbors(vid).size(); i++)
			{
				unsigned int f = neighbors(vid)[i];
				fmark(f, mark);
			}
		}

		inline void MeshModel::collect_unmarked_neighbors(MeshVertID vid, std::vector<MeshFaceID>& faces)
		{

			for (unsigned int i = 0; i<neighbors(vid).size(); i++)
			{
				unsigned int fid = neighbors(vid)[i];
				if (!fmark(fid))
				{
					faces.push_back(fid);
					fmark(fid, 1);
				}
			}
		}

		inline void MeshModel::mark_neighborhood_delta(MeshVertID vid, short delta)
		{
			for (unsigned int i = 0; i<neighbors(vid).size(); i++)
			{
				unsigned int f = neighbors(vid)[i];
				fmark(f, fmark(f) + delta);
			}
		}

		inline void MeshModel::partition_marked_neighbors(MeshVertID v, unsigned short pivot,
			std::vector<MeshFaceID>& lo, std::vector<MeshFaceID>& hi)
		{
			for (unsigned int i = 0; i<neighbors(v).size(); i++)
			{
				unsigned int f = neighbors(v)[i];
				if (fmark(f))
				{
					if (fmark(f) < pivot)  lo.push_back(f);
					else  hi.push_back(f);
					fmark(f, 0);
				}
			}
		}


		inline void MeshModel::mark_corners(const std::vector<MeshFaceID>& faces, unsigned short mark)
		{
			for (unsigned int i = 0; i<faces.size(); i++)
			for (unsigned int j = 0; j<3; j++)
				vmark(face(faces[i])[j], mark);
		}

		inline void MeshModel::collect_unmarked_corners(const std::vector<MeshFaceID>& faces,
			std::vector<MeshVertID>& verts)
		{
			for (unsigned int i = 0; i<faces.size(); i++)
			for (unsigned int j = 0; j<3; j++)
			{
				MeshVertID v = face(faces[i])[j];
				if (!vmark(v))
				{
					verts.push_back(v);
					vmark(v, 1);
				}
			}
		}

		inline void MeshModel::collect_edge_neighbors(unsigned int v1, unsigned int v2,
			std::vector<MeshFaceID>& faces)
		{
			mark_neighborhood(v1, 1);
			mark_neighborhood(v2, 0);
			collect_unmarked_neighbors(v1, faces);
		}

		inline void MeshModel::collect_vertex_star(unsigned int v, std::vector<MeshVertID>& verts)
		{
			const std::vector<MeshFaceID>& N = neighbors(v);

			mark_corners(N, 0);
			vmark(v, 1); // Don't want to include v in the star
			collect_unmarked_corners(N, verts);
		}

		inline void MeshModel::collect_neighborhood(MeshVertID v, int depth,
			std::vector<MeshFaceID>& faces)
		{
			// TODO: This method is somewhat inefficient.  It will repeatedly touch
			// all the faces within the collected region at each iteration.  For now,
			// this is acceptable.  But ultimately it will need to be fixed.

			int i;

			faces.clear();

			// Initially copy immediate neighbors of v
			for (i = 0; i<neighbors(v).size(); i++)
				faces.push_back(neighbors(v)[i]);

			for (; depth>0; depth--)
			{
				// Unmark the neighbors of all vertices in region
				for (i = 0; i<faces.size(); i++)
				{
					mark_neighborhood(face(faces[i])[0], 0);
					mark_neighborhood(face(faces[i])[1], 0);
					mark_neighborhood(face(faces[i])[2], 0);
				}

				// Mark all faces already accumulated
				for (i = 0; i<faces.size(); i++)
					fmark(faces[i], 1);

				// Collect all unmarked faces
				unsigned int limit = faces.size();
				for (i = 0; i<limit; i++)
				{
					collect_unmarked_neighbors(face(faces[i])[0], faces);
					collect_unmarked_neighbors(face(faces[i])[1], faces);
					collect_unmarked_neighbors(face(faces[i])[2], faces);
				}
			}
		}
		inline void MeshModel::remap_vertex(unsigned int from, unsigned int to)
		{
			for (unsigned int i = 0; i<neighbors(from).size(); i++)
				face(neighbors(from)[i]).remap_vertex(from, to);

			mark_neighborhood(from, 0);
			mark_neighborhood(to, 1);
			collect_unmarked_neighbors(from, neighbors(to));

			vertex_mark_invalid(from);
			neighbors(from).clear();   // remove links in old vertex
		}

		inline unsigned int MeshModel::split_edge(unsigned int a, unsigned int b)
		{
			Vector3 v1 = vertex(a).vertex, v2 = vertex(b).vertex;

			return split_edge(a, b,
				(v1.x + v2.x) / 2.0,
				(v1.y + v2.y) / 2.0,
				(v1.z + v2.z) / 2.0);
		}

		inline static void remove_neighbor(std::vector<MeshFaceID>& faces, unsigned int f)
		{
			unsigned int j;
			if (varray_find(faces, f, &j))
				faces[j] = faces.back();

		}
		
		inline unsigned int MeshModel::split_edge(unsigned int v1, unsigned int v2,
			double x, double y, double z)
		{
			std::vector<MeshFaceID> faces;
			collect_edge_neighbors(v1, v2, faces);

			unsigned int vnew = add_vertex(x, y, z);

			for (unsigned int i = 0; i<faces.size(); i++)
			{
				unsigned int f = faces[i];
				unsigned int v3 = face(f).opposite_vertex(v1, v2);

				// in f, remap v2-->vnew
				face(f).remap_vertex(v2, vnew);
				neighbors(vnew).push_back(f);

				// remove f from neighbors(v2)
				remove_neighbor(neighbors(v2), f);

				// assure orientation is consistent
				if (face(f).is_inorder(vnew, v3))
					add_face(vnew, v2, v3);
				else
					add_face(vnew, v3, v2);
			}

			return vnew;
		}

		inline void MeshModel::flip_edge(unsigned int v1, unsigned int v2)
		{
			std::vector<MeshFaceID> faces;  collect_edge_neighbors(v1, v2, faces);
			if (faces.size() != 2) return;

			unsigned int f1 = faces[0];
			unsigned int f2 = faces[1];
			unsigned int v3 = face(f1).opposite_vertex(v1, v2);
			unsigned int v4 = face(f2).opposite_vertex(v1, v2);

			// ?? Should we check for convexity or assume thats been taken care of?

			remove_neighbor(neighbors(v1), f2);
			remove_neighbor(neighbors(v2), f1);
			neighbors(v3).push_back(f2);
			neighbors(v4).push_back(f1);

			face(f1).remap_vertex(v2, v4);
			face(f2).remap_vertex(v1, v3);
		}

		inline void MeshModel::split_face4(unsigned int f, unsigned int *newverts)
		{
			unsigned int v0 = face(f).v[0];
			unsigned int v1 = face(f).v[1];
			unsigned int v2 = face(f).v[2];

			unsigned int pivot = split_edge(v0, v1);
			unsigned int new1 = split_edge(v1, v2);
			unsigned int new2 = split_edge(v0, v2);

			if (newverts)
			{
				newverts[0] = pivot;
				newverts[1] = new1;
				newverts[2] = new2;
			}

			flip_edge(pivot, v2);
		}

		inline void MeshModel::compact_vertices()
		{
			MeshVertID oldID;
			MeshVertID newID = 0;

			for (oldID = 0; oldID<vert_count(); oldID++)
			{
				if (vertex_is_valid(oldID))
				{
					if (newID != oldID)
					{
						vertex(newID) = vertex(oldID);
						// Because we'll be freeing the link lists for the
						// old vertices, we actually have to swap values instead
						// of the simple copying in the block above.
						//
						std::vector<MeshFaceID> t = face_links[newID];
						face_links[newID] = face_links[oldID];
						face_links[oldID] = t;

						vertex_mark_valid(newID);

						for (unsigned int i = 0; i<neighbors(newID).size(); i++)
							face(neighbors(newID)[i]).remap_vertex(oldID, newID);
					}
					newID++;
				}
			}

			for (oldID = newID; newID < vert_count();)
				remove_vertex(oldID);
		}

		inline void MeshModel::unlink_face(MeshFaceID fid)
		{
			MeshFace& f = face(fid);
			face_mark_invalid(fid);

			unsigned int j; int found = 0;

			if (varray_find(neighbors(f[0]), fid, &j))
			{
				found++; neighbors(f[0])[j] = neighbors(f[0]).back();
			}
			if (varray_find(neighbors(f[1]), fid, &j))
			{
				found++; neighbors(f[1])[j] = neighbors(f[1]).back();
			}
			if (varray_find(neighbors(f[2]), fid, &j))
			{
				found++; neighbors(f[2])[j] = neighbors(f[2]).back();
			}
		}

		inline void MeshModel::remove_degeneracy(std::vector<MeshFaceID>& faces)
		{
			for (unsigned int i = 0; i<faces.size(); i++)
			{
				MeshFace& f = face(faces[i]);

				if (f[0] == f[1] || f[1] == f[2] || f[0] == f[2])
					unlink_face(faces[i]);
			}
		}

		inline void MeshModel::compute_contraction(MeshVertID v1, MeshVertID v2,
			PairContraction *conx,
			const Vector3 *vnew)
		{
			conx->v1 = v1;
			conx->v2 = v2;

			if (vnew)
			{
				conx->dv1 = *vnew - vertex(v1).vertex;
				conx->dv2 = *vnew - vertex(v2).vertex;
			}
			else
			{
				conx->dv1[0] = conx->dv1[1] = conx->dv1[2] = 0.0;
				conx->dv2[0] = conx->dv2[1] = conx->dv2[2] = 0.0;
			}

			conx->delta_faces.clear();
			conx->dead_faces.clear();


			// Mark the neighborhood of (v1,v2) such that each face is
			// tagged with the number of times the vertices v1,v2 occur
			// in it.  Possible values are 1 or 2.
			//
			mark_neighborhood(v2, 0);
			mark_neighborhood(v1, 1);
			mark_neighborhood_delta(v2, 1);


			// Now partition the neighborhood of (v1,v2) into those faces
			// which degenerate during contraction and those which are merely
			// reshaped.
			//
			partition_marked_neighbors(v1, 2, conx->delta_faces, conx->dead_faces);
			conx->delta_pivot = conx->delta_faces.size();
			partition_marked_neighbors(v2, 2, conx->delta_faces, conx->dead_faces);
		}

		inline void MeshModel::apply_contraction(const PairContraction& conx)
		{
			MeshVertID v1 = conx.v1, v2 = conx.v2;

			// Move v1 to new position
			vertex(v1).vertex += conx.dv1;

			unsigned int i;
			//
			// Remove dead faces
			for (i = 0; i<conx.dead_faces.size(); i++)
				unlink_face(conx.dead_faces[i]);

			//
			// Update changed faces
			for (i = conx.delta_pivot; i<conx.delta_faces.size(); i++)
			{
				MeshFaceID fid = conx.delta_faces[i];
				face(fid).remap_vertex(v2, v1);
				neighbors(v1).push_back(fid);
			}

			// Kill v2
			vertex_mark_invalid(v2);
			neighbors(v2).clear();
		}

		inline void MeshModel::apply_expansion(const PairContraction& conx)
		{
			MeshVertID v1 = conx.v1, v2 = conx.v2;

			vertex(v2).vertex = vertex(v1).vertex - conx.dv2;
			vertex(v1).vertex -= conx.dv1;

			unsigned int i, j;
			for (i = 0; i < conx.dead_faces.size(); i++)
			{
				MeshFaceID fid = conx.dead_faces[i];
				face_mark_valid(fid);
				neighbors(face(fid)[0]).push_back(fid);
				neighbors(face(fid)[1]).push_back(fid);
				neighbors(face(fid)[2]).push_back(fid);
			}

			for (i = conx.delta_pivot; i < conx.delta_faces.size(); i++)
			{
				MeshFaceID fid = conx.delta_faces[i];
				face(fid).remap_vertex(v1, v2);
				neighbors(v2).push_back(fid);
				bool found = varray_find(neighbors(v1), fid, &j);
				neighbors(v1)[j] = neighbors(v1).back();

				vertex_mark_valid(v2);
			}
		}



		inline void MeshModel::contract(MeshVertID v1, MeshVertID v2,
			const Vector3 *vnew, PairContraction *conx)
		{
			compute_contraction(v1, v2, conx);
			conx->dv1 = *vnew - vertex(v1).vertex;
			conx->dv2 = *vnew - vertex(v2).vertex;
			apply_contraction(*conx);
		}

		inline MeshVertID MeshModel::resolve_proxies(MeshVertID v)
		{
			while (vertex_is_proxy(v))
				v = vertex(v).as.proxy.parent;
			return v;
		}

		inline Vector3 *MeshModel::vertex_position(MeshVertID v)
		{
			return &vertex(resolve_proxies(v)).vertex;
		}

		MeshVertID& MeshModel::vertex_proxy_parent(MeshVertID v)
		{
			return vertex(v).as.proxy.parent;
		}

		MeshVertID MeshModel::add_proxy_vertex(MeshVertID parent)
		{
			MeshVertID v = alloc_vertex(0, 0, 0); // position will be ignored

			vertex_mark_proxy(v);
			vertex_proxy_parent(v) = parent;

			return v;
		}
		inline void MeshModel::compute_face_normal(MeshFaceID f, Vector3 &n, bool will_unitize)
		{
			MeshFace face = faces[f];
			Vector3 v1 = vertices[face.v[0]].vertex;
			Vector3 v2 = vertices[face.v[1]].vertex;
			Vector3 v3 = vertices[face.v[2]].vertex;

			Vector3 a, b;
			a = v2 - v1;
			b = v3 - v1;
			
			n = Vector3::Cross(a, b);
			if (will_unitize)
				n.Normalize();
		}
		
		inline double MeshModel::compute_face_plane(MeshFaceID f, Vector3 &p, bool will_unitize)
		{
			compute_face_normal(f, p, will_unitize);
			return -Vector3::Dot(p, corner(f, 0).vertex);
		}

		inline double MeshModel::compute_face_area(MeshFaceID f)
		{
			Vector3 n;

			compute_face_normal(f, n, false);
			return 0.5 * n.Magnitude;
		}
	}
}